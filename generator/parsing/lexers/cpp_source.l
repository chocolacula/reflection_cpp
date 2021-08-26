%option fast
%option noindent
%option lexer=LexerCpp
%option unicode

%top{
  #include "../../position.h"
  #include "../../tokens/cpp_source.h"
%}

%class{
 public:
  rr::Position get_position() {
    return rr::Position{column: columno(), line_number: lineno()};
  }

  bool is_equal(const char* str) {
    return strcmp(text(), str) == 0;
  }
%}

macro       #.*\n
whitespaces [ \t\r\n]
comment_il  "//"([^\n]|\\\n)*\n
comment_ml  "/*"(.|\n)*?"*/"
attribute   \<RR_[A-Z]+\>
begin       "{"
end         "}"
word        [a-zA-Z0-9_]+

%%

{macro}
{comment_il}
{comment_ml}
{whitespaces}
{begin}       return static_cast<int>(TokensCpp::kBraceL);
{end}         return static_cast<int>(TokensCpp::kBraceR);
{attribute}   return static_cast<int>(TokensCpp::kAttribute);
{word}        return static_cast<int>(TokensCpp::kWord);
.
<<EOF>>       return static_cast<int>(TokensCpp::kEof);

%%
