#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "nlohmann/json.hpp"

// clang
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Tooling/Tooling.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class ParserCpp {
 public:
  ParserCpp(const CompilationDatabase* compdb, std::string&& file_path)
      : _tool(*compdb, {std::move(file_path)}),                 //
        _macro_factory(std::make_unique<MacroFactory>(&_ctx)),  //
        _matcher(std::make_unique<Matcher>(&_ctx)) {

    static const auto class_matcher = cxxRecordDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("c");
    _finder.addMatcher(class_matcher, _matcher.get());

    static const auto enum_matcher = enumDecl(unless(isExpansionInSystemHeader())).bind("e");
    _finder.addMatcher(enum_matcher, _matcher.get());
  }

  std::vector<nlohmann::json> parse() {
    // handle macro attributes at first
    _tool.run(_macro_factory.get());

    if (_ctx.reflect_map.empty()) {
      return {};
    }

    // traverse AST, check attributes, build json objects and fill result field in context
    _tool.run(newFrontendActionFactory(&_finder).get());
    return _ctx.result;
  }

 private:
  ClangTool _tool;
  MatchFinder _finder;

  class MacroFactory;
  std::unique_ptr<MacroFactory> _macro_factory;
  class Matcher;
  std::unique_ptr<Matcher> _matcher;

  struct AttrReflect {
    bool withNonPublic = false;
    bool withBase = false;
  };

  struct Context {
    std::unordered_map<unsigned, AttrReflect> reflect_map;
    std::unordered_map<unsigned, std::string> alias_map;
    std::unordered_set<unsigned> excludes;

    std::vector<nlohmann::json> result;
  } _ctx;

  class MacroCallback : public PPCallbacks {
   public:
    explicit MacroCallback(SourceManager& sm, LangOptions& opts, Context* ctx) : _sm(sm), _opts(opts), _ctx(ctx) {
    }

    void MacroExpands(const Token& token, const MacroDefinition& /*def*/, SourceRange /*range*/,
                      const MacroArgs* /*args*/) final {

      if (_sm.isInSystemHeader(token.getLocation())) {
        return;
      }

      auto text = spell(&token, _sm);
      auto t = token;

      if (text == "ER_REFLECT") {
        AttrReflect attr;

        while (true) {
          auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
          t = t_opt.getValue();

          if (t.getKind() == tok::r_paren) {
            break;
          }
          if (!(t.getKind() == tok::raw_identifier || t.getKind() == tok::string_literal)) {
            continue;
          }

          auto spelling = spell(&t, _sm);
          if (spelling == "WithNonPublic") {
            attr.withNonPublic = true;
          } else if (spelling == "WithBase") {
            attr.withBase = true;
          }
        }

        std::string spelling;
        do {
          // skip ')' and keywords 'struct' 'class' or 'enum'
          auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
          t = t_opt.getValue();

          spelling = spell(&t, _sm);
        } while (spelling == "struct" || spelling == "class" || spelling == "enum" || spelling == "const");

        _ctx->reflect_map[_sm.getFileOffset(t.getLocation())] = attr;

      } else if (text == "ER_ALIAS") {
        std::string alias;

        // skip '('
        auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        // get the alias token
        t_opt = Lexer::findNextToken(t_opt.getValue().getLocation(), _sm, _opts);
        t = t_opt.getValue();
        alias = spell(&t, _sm);
        // skip ')'
        t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        t = t_opt.getValue();

        auto t_origin = t;
        while (true) {
          // skip all type parts
          t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
          t = t_opt.getValue();

          auto k = t.getKind();
          if (k == tok::coloncolon || k == tok::less || k == tok::greater) {
            continue;
          }

          if (k == tok::raw_identifier) {
            t_origin = t;
          } else {
            break;
          }
        }

        _ctx->alias_map[_sm.getFileOffset(t_origin.getLocation())] = alias;

      } else if (text == "ER_EXCLUDE") {

        auto t_origin = t;
        while (true) {
          // skip all type parts
          auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
          t = t_opt.getValue();

          auto k = t.getKind();
          if (k == tok::coloncolon || k == tok::less || k == tok::greater) {
            continue;
          }

          if (k == tok::raw_identifier) {
            t_origin = t;
          } else {
            break;
          }
        }

        _ctx->excludes.insert(_sm.getFileOffset(t_origin.getLocation()));
      }
    }

   private:
    SourceManager& _sm;
    LangOptions& _opts;
    Context* _ctx;
  };

  class MacroAction : public ASTFrontendAction {
   public:
    explicit MacroAction(Context* ctx) : _ctx(ctx) {
    }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& compiler, StringRef /*in_file*/) override {
      compiler.getPreprocessor().addPPCallbacks(std::make_unique<MacroCallback>(compiler.getSourceManager(),  //
                                                                                compiler.getLangOpts(),       //
                                                                                _ctx));
      return std::make_unique<ASTConsumer>();
    }

   private:
    Context* _ctx;
  };

  class MacroFactory : public FrontendActionFactory {
   public:
    explicit MacroFactory(Context* ctx) : _ctx(ctx) {
    }

    std::unique_ptr<FrontendAction> create() override {
      return std::make_unique<MacroAction>(_ctx);
    }

   private:
    Context* _ctx;
  };

  class Matcher : public MatchFinder::MatchCallback {
   public:
    explicit Matcher(Context* ctx) : _ctx(ctx) {
    }

    void run(const MatchFinder::MatchResult& result) final {

      if (const auto* c = result.Nodes.getNodeAs<CXXRecordDecl>("c")) {
        handle_class(c, false, *result.SourceManager, result.Context->getLangOpts());
      }
      if (const auto* e = result.Nodes.getNodeAs<EnumDecl>("e")) {
        handle_enum(e, *result.SourceManager, result.Context->getLangOpts());
      }
    }

   private:
    Context* _ctx;

    void handle_class(const CXXRecordDecl* c, bool is_force, const SourceManager& sm, const LangOptions& opts) {

      auto it = _ctx->reflect_map.find(get_offset(c, sm, opts));
      if (it != _ctx->reflect_map.end() || is_force) {
        llvm::outs() << "reflected ";
      } else {
        return;
      }

      llvm::outs() << c->getQualifiedNameAsString() << " : ";

      for (auto&& b : c->bases()) {
        auto n = b.getType()->getAsRecordDecl()->getQualifiedNameAsString();
        llvm::outs() << to_string(b.getAccessSpecifier()) << " " << n << ", ";
      }

      llvm::outs() << "\n";

      for (auto&& d : c->getPrimaryContext()->decls()) {
        if (const auto* f = dyn_cast<FieldDecl>(d)) {
          auto offset = get_offset(f, sm, opts);

          if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
            continue;
          }

          llvm::outs() << to_string(f->getAccess()) << " ";
          llvm::outs() << f->getNameAsString();

          auto it = _ctx->alias_map.find(offset);
          if (it != _ctx->alias_map.end()) {
            llvm::outs() << " as " << it->second << "\n";
          } else {
            llvm::outs() << "\n";
          }
          continue;
        }

        if (const auto* v = dyn_cast<VarDecl>(d)) {
          auto offset = get_offset(v, sm, opts);

          if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
            continue;
          }

          llvm::outs() << to_string(v->getAccess()) << " static ";
          llvm::outs() << v->getNameAsString();

          auto it = _ctx->alias_map.find(offset);
          if (it != _ctx->alias_map.end()) {
            llvm::outs() << " as " << it->second << "\n";
          } else {
            llvm::outs() << "\n";
          }
          continue;
        }

        if (const auto* nc = dyn_cast<CXXRecordDecl>(d)) {
          if (nc->isThisDeclarationADefinition()) {
            llvm::outs() << nc->getNameAsString() << " nested class\n";
            handle_class(nc, true, sm, opts);
          }
        }
      }
    }

    inline void handle_enum(const EnumDecl* e, const SourceManager& sm, const LangOptions& opts) {
      auto it = _ctx->reflect_map.find(get_offset(e, sm, opts));

      if (it != _ctx->reflect_map.end()) {
        llvm::outs() << "reflected ";
      } else {
        return;
      }

      llvm::outs() << e->getQualifiedNameAsString() << "\n";

      for (auto&& constant : e->enumerators()) {
        auto offset = get_offset(constant, sm, opts);

        if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
          continue;
        }

        llvm::outs() << constant->getNameAsString();

        auto it = _ctx->alias_map.find(offset);
        if (it != _ctx->alias_map.end()) {
          llvm::outs() << " as " << it->second << "\n";
        } else {
          llvm::outs() << "\n";
        }
      }
    }

    static std::string to_string(AccessSpecifier access) {
      switch (access) {
        case clang::AS_none:
          return "none";
        case clang::AS_public:
          return "public";
        case clang::AS_protected:
          return "protected";
        case clang::AS_private:
          return "private";
      }
    }
  };

  static inline StringRef spell(SourceRange range, const SourceManager& sm) {
    auto ch_range = CharSourceRange::getTokenRange(range);
    return Lexer::getSourceText(CharSourceRange::getTokenRange(range), sm, LangOptions(), nullptr);
  }

  static inline StringRef spell(const Token* token, const SourceManager& sm) {
    SourceLocation b;
    SourceLocation e;

    if (tok::isStringLiteral(token->getKind())) {
      b = token->getLocation().getLocWithOffset(1);
      e = token->getEndLoc().getLocWithOffset(-2);
    } else {
      b = token->getLocation();
      e = token->getEndLoc().getLocWithOffset(-1);
    }

    return spell(SourceRange(b, e), sm);
  }

  template <typename DeclT>
  static inline unsigned get_offset(const DeclT* decl, const SourceManager& sm, const LangOptions& opts) {
    Token token;
    Lexer::getRawToken(decl->getLocation(), token, sm, opts);

    return sm.getFileOffset(token.getLocation());
  }
};