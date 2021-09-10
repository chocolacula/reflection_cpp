#include <linux/limits.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/LLVM.h"
#include "config.h"
// #include "parsing/parser_cpp.h"
#include "rr/serialization/yaml.h"
#include "self_generated/reflection.h"
#include "tclap/CmdLine.h"

// clang
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class Printer : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) final {

    if (const auto* c = result.Nodes.getNodeAs<CXXRecordDecl>("class")) {

      if (c->hasAttrs()) {
        llvm::outs() << "has attrs: ";

        for (auto&& a : c->getAttrs()) {
          llvm::outs() << a->getSpelling() << " " << a->getSpellingListIndex();
        }
      }

      llvm::outs() << c->getQualifiedNameAsString() << "\n";

      for (auto&& d : c->getPrimaryContext()->decls()) {
        if (auto* f = dyn_cast<FieldDecl>(d)) {
          if (f->hasAttrs()) {
            llvm::outs() << "has attrs: ";

            for (auto&& a : f->getAttrs()) {
              llvm::outs() << a->getSpelling() << " ";
            }
          }

          llvm::outs() << f->getNameAsString() << "\n";
        }

        if (auto* v = dyn_cast<VarDecl>(d)) {
          llvm::outs() << v->getNameAsString() << "\n";

          if (v->hasAttrs()) {
            llvm::outs() << "has attrs: ";

            for (auto&& a : v->getAttrs()) {
              llvm::outs() << a->getSpelling() << " ";
            }
          }
        }
      }
    }
    if (const auto* e = result.Nodes.getNodeAs<EnumDecl>("enum")) {
      if (e->hasAttrs()) {
        llvm::outs() << "has attrs: ";

        for (auto&& a : e->getAttrs()) {
          llvm::outs() << a->getSpelling() << " ";
        }
      }

      llvm::outs() << e->getQualifiedNameAsString() << "\n";

      for (auto&& constants : e->enumerators()) {
        if (constants->hasAttrs()) {
          llvm::outs() << "has attrs: ";

          for (auto&& a : constants->getAttrs()) {
            llvm::outs() << Lexer::getSourceText(CharSourceRange::getTokenRange(a->getRange()),
                                                 *result.SourceManager,  //
                                                 result.Context->getLangOpts())
                                .str();
          }
        }

        llvm::outs() << constants->getNameAsString() << "\n";
      }
    }
  }
};

std::filesystem::path current_dir() {
  char exe_path[PATH_MAX];
  ssize_t size = readlink("/proc/self/exe", exe_path, PATH_MAX);

  return std::filesystem::path(exe_path).remove_filename();
}

int main(int argc, const char** argv) {

  TCLAP::CmdLine cmd("Required Reflection code generator", ' ', VERSION);

  auto root = current_dir();

  root.append("config.yaml");
  TCLAP::ValueArg<std::string> c_arg("c", "config", "Explicitly specify path to the config file",  //
                                     false, root.string(), "path");
  root.remove_filename();

  TCLAP::ValueArg<int> j_arg("j", "jobs", "Allow N jobs at once, takes CPU core number by default",  //
                             false, std::thread::hardware_concurrency(), "N");

  cmd.add(c_arg);
  cmd.add(j_arg);
  cmd.parse(argc, argv);

  std::string c_path = c_arg.getValue();

  std::ifstream input;
  input.open(c_path);

  if (!input.is_open()) {
    std::cerr << "Cannot find the config file, aborted" << std::endl;
    return -1;
  }

  auto conf = rr::serialization::yaml::from_stream<Config>(input).unwrap();
  input.close();

  std::filesystem::path working_dir;

  if (conf.root_dir[0] == '/') {
    working_dir.assign(conf.root_dir);
  } else {
    working_dir = root;
    working_dir.append(conf.root_dir);
  }

  std::string err;
  std::string compdb_file = working_dir.string() + "/" + conf.compdb_dir;
  auto compdb = CompilationDatabase::autoDetectFromDirectory(compdb_file, err);

  for (auto&& file_name : conf.input) {

    auto file_path = working_dir;
    file_path.append(file_name);

    std::vector<std::string> source = {file_path.string()};

    ClangTool tool(*compdb, source);

    auto class_matcher = cxxRecordDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("class");
    auto enum_matcher = enumDecl(unless(isExpansionInSystemHeader())).bind("enum");

    Printer printer;
    MatchFinder finder;
    finder.addMatcher(class_matcher, &printer);
    finder.addMatcher(enum_matcher, &printer);

    tool.run(newFrontendActionFactory(&finder).get());
  }

  // std::unordered_map<std::string, Node> registry;
  // take one from input
  // process it multithreaded
  // put Node to the registry

  // convert Node to nlohman::json, pay attention to namespaces and parent classes
  // generate file

  return 0;
}