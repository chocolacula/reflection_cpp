// A fast JSON parser and JSON writer
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// Passes tests suggested by <http://seriot.ch/parsing_json.php>
//
// This example demonstrates:
// 1.  %o flex compatible actions yytext and BEGIN
// 2.  %o freespace to space regular expressions between concat and alternations
// 3.  %o dotall mode: . matches \n
// 4.  %o unicode mode: . mathes any Unicode character
// 5.  %x STRING state with rules to convert a JSON string to std::wstring
// 6.  \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character
// 7.  JSON value types are indicated with tokens '0', 't', 'f', '#', '$'
// 8.  C++ yyFlexLexer class members to store lexer values (MT-safe)
// 9.  JSONParser is a recursive descent parser for JSON
// 10. JSONParser inherits yyFlexLexer, for yyFlexLexer::yylex() and lexer state
// 11. JSONParser recurses 100 levels deep max (MAXLEVEL)
// 12. JSONParser accepts 1000 items per array/object max (MAXSIZE)
// 13. main() reads a FILE* for automatic UTF-16/32 normalization to UTF-8
// 14. JSON class stores parsed JSON data
// 15. JSON class instances are writable to std::ostream

%option lexer=LexerJson

%top{
  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw
  #include <vector>   // to store JSON arrays
  #include <map>      // to store JSON objects
}

// define yyFlexLexer class variables to collect values in the lexer rules
%class{
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
