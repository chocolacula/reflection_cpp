// A fast JSON parser and JSON writer
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// * %o freespace to space regular expressions between concat and alternations
// * %o dotall mode: . matches \n
// * %o unicode mode: . mathes any Unicode character
// * %x STRING state with rules to convert a JSON string to std::wstring
// * \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character
// * JSON value types are indicated with tokens '0', 't', 'f', '#', '$'
// * JSONParser is a recursive descent parser for JSON
// * JSONParser recurses 100 levels deep max (MAXLEVEL)
// * JSONParser accepts 1000 items per array/object max (MAXSIZE)

%option lexer=LexerJson

%top{
  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw

  #include "../../position.h"
}

%class{
 public:
  rr::Position get_position() {
    return rr::Position{column: columno(), line_number: lineno()};
  }

 protected:
  std::string string; // token value for token '$' (string)
}

%o fast freespace dotall unicode

digit   [0-9]
digit1  [1-9]
digits  {digit}+
int     -? {digit} | -? {digit1} {digits}
frac    \. {digits}
exp     [eE] [-+]? {digits}
number  {int} {frac}? {exp}?

%x STRING

%%

[ \t\n\r]+      { /* ignore white space */ }
[\]\[}{,:]      { return text()[0]; }
null            { return '0'; }
true            { return 't'; }
false           { return 'f'; }
{number}        { string = text(); return '#'; }
\"              { string.clear(); start(STRING); }

<STRING>{
\"                      { start(INITIAL); return '$'; }
\\ ["\\/]               { string.push_back(text()[1]); }
\\ b                    { string.push_back('\b'); }
\\ t                    { string.push_back('\t'); }
\\ n                    { string.push_back('\n'); }
\\ f                    { string.push_back('\f'); }
\\ r                    { string.push_back('\r'); }
\\ u [[:xdigit:]]{4}    { string.push_back(strtoul(text() + 2, NULL, 16)); }
[\]-\x7f\x20-\[]        { string.push_back(text()[0]); }
\p{Non_ASCII_Unicode}   { string.append(str()); }
}

<*>.            { return '!'; /* error */ }

%%
