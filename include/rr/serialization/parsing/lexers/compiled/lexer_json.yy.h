// include/rr/serialization/parsing/lexers/compiled/lexer_json.yy.h generated by reflex 3.0.7 from include/rr/serialization/parsing/lexers/json.l

#define REFLEX_VERSION "3.0.7"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_dotall              true
#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               LexerJson
#define REFLEX_OPTION_outfile             "include/rr/serialization/parsing/lexers/compiled/lexer_json.yy.h"
#define REFLEX_OPTION_unicode             true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 18 "include/rr/serialization/parsing/lexers/json.l"

  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw

  #include "../../position.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class LexerJson : public reflex::AbstractLexer<reflex::Matcher> {
#line 26 "include/rr/serialization/parsing/lexers/json.l"

 public:
  rr::Position get_position() {
    return rr::Position{column: columno(), line_number: lineno()};
  }

 protected:
  std::string string; // token value for token '$' (string)

 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  LexerJson(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int STRING = 1;
  virtual int lex(void);
  int lex(const reflex::Input& input)
  {
    in(input);
    return lex();
  }
  int lex(const reflex::Input& input, std::ostream *os)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "include/rr/serialization/parsing/lexers/json.l"
// A fast JSON parser and JSON writer
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// . %o freespace to space regular expressions between concat and alternations
// . %o dotall mode: . matches \n
// . %o unicode mode: . mathes any Unicode character
// . %x STRING state with rules to convert a JSON string to std::wstring
// . \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character
// . JSON value types are indicated with tokens '0', 't', 'f', '#', '$'
// . JSONParser is a recursive descent parser for JSON
// . JSONParser inherits yyFlexLexer, for yyFlexLexer::yylex() and lexer state
// . JSONParser recurses 100 levels deep max (MAXLEVEL)
// . JSONParser accepts 1000 items per array/object max (MAXSIZE)


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

extern void reflex_code_INITIAL(reflex::Matcher&);
extern void reflex_code_STRING(reflex::Matcher&);

int LexerJson::lex(void)
{
  static const reflex::Pattern PATTERN_INITIAL(reflex_code_INITIAL);
  static const reflex::Pattern PATTERN_STRING(reflex_code_STRING);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule include/rr/serialization/parsing/lexers/json.l:50: [ \t\n\r]+ :
#line 50 "include/rr/serialization/parsing/lexers/json.l"
{ /* ignore white space */ }
            break;
          case 2: // rule include/rr/serialization/parsing/lexers/json.l:51: [\]\[}{,:] :
#line 51 "include/rr/serialization/parsing/lexers/json.l"
{ return text()[0]; }
            break;
          case 3: // rule include/rr/serialization/parsing/lexers/json.l:52: null :
#line 52 "include/rr/serialization/parsing/lexers/json.l"
{ return '0'; }
            break;
          case 4: // rule include/rr/serialization/parsing/lexers/json.l:53: true :
#line 53 "include/rr/serialization/parsing/lexers/json.l"
{ return 't'; }
            break;
          case 5: // rule include/rr/serialization/parsing/lexers/json.l:54: false :
#line 54 "include/rr/serialization/parsing/lexers/json.l"
{ return 'f'; }
            break;
          case 6: // rule include/rr/serialization/parsing/lexers/json.l:55: {number} :
#line 55 "include/rr/serialization/parsing/lexers/json.l"
{ string = text(); return '#'; }
            break;
          case 7: // rule include/rr/serialization/parsing/lexers/json.l:56: \" :
#line 56 "include/rr/serialization/parsing/lexers/json.l"
{ string.clear(); start(STRING); }

            break;
          case 8: // rule include/rr/serialization/parsing/lexers/json.l:71: . :
#line 71 "include/rr/serialization/parsing/lexers/json.l"
{ return '!'; /* error */ }

            break;
        }
        break;
      case STRING:
        matcher().pattern(PATTERN_STRING);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule include/rr/serialization/parsing/lexers/json.l:59: \" :
#line 59 "include/rr/serialization/parsing/lexers/json.l"
{ start(INITIAL); return '$'; }
            break;
          case 2: // rule include/rr/serialization/parsing/lexers/json.l:60: \\ ["\\/] :
#line 60 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back(text()[1]); }
            break;
          case 3: // rule include/rr/serialization/parsing/lexers/json.l:61: \\ b :
#line 61 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back('\b'); }
            break;
          case 4: // rule include/rr/serialization/parsing/lexers/json.l:62: \\ t :
#line 62 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back('\t'); }
            break;
          case 5: // rule include/rr/serialization/parsing/lexers/json.l:63: \\ n :
#line 63 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back('\n'); }
            break;
          case 6: // rule include/rr/serialization/parsing/lexers/json.l:64: \\ f :
#line 64 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back('\f'); }
            break;
          case 7: // rule include/rr/serialization/parsing/lexers/json.l:65: \\ r :
#line 65 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back('\r'); }
            break;
          case 8: // rule include/rr/serialization/parsing/lexers/json.l:66: \\ u [[:xdigit:]]{4} :
#line 66 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back(strtoul(text() + 2, NULL, 16)); }
            break;
          case 9: // rule include/rr/serialization/parsing/lexers/json.l:67: [\]-\x7f\x20-\[] :
#line 67 "include/rr/serialization/parsing/lexers/json.l"
{ string.push_back(text()[0]); }
            break;
          case 10: // rule include/rr/serialization/parsing/lexers/json.l:68: \p{Non_ASCII_Unicode} :
#line 68 "include/rr/serialization/parsing/lexers/json.l"
{ string.append(str()); }
            break;
          case 11: // rule include/rr/serialization/parsing/lexers/json.l:71: . :
#line 71 "include/rr/serialization/parsing/lexers/json.l"
{ return '!'; /* error */ }

            break;
        }
        break;
      default:
        start(0);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  TABLES                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

void reflex_code_INITIAL(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  c1 = m.FSM_CHAR();
  if (c1 == '}') goto S17;
  if (c1 == '{') goto S17;
  if (c1 == 't') goto S24;
  if (c1 == 'n') goto S20;
  if (c1 == 'f') goto S28;
  if (c1 == ']') goto S17;
  if (c1 == '[') goto S17;
  if (c1 == ':') goto S17;
  if ('1' <= c1 && c1 <= '9') goto S38;
  if (c1 == '0') goto S32;
  if (c1 == '-') goto S57;
  if (c1 == ',') goto S17;
  if (c1 == '"') goto S45;
  if (c1 == ' ') goto S51;
  if (c1 == '\r') goto S51;
  if ('\t' <= c1 && c1 <= '\n') goto S51;
  if (0 <= c1) goto S48;
  return m.FSM_HALT(c1);

S17:
  m.FSM_TAKE(2);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  return m.FSM_HALT(c1);

S20:
  m.FSM_TAKE(8);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == 'u') goto S62;
  return m.FSM_HALT(c1);

S24:
  m.FSM_TAKE(8);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == 'r') goto S64;
  return m.FSM_HALT(c1);

S28:
  m.FSM_TAKE(8);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == 'a') goto S66;
  return m.FSM_HALT(c1);

S32:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == 'e') goto S70;
  if (c1 == 'E') goto S70;
  if (c1 == '.') goto S68;
  return m.FSM_HALT(c1);

S38:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == 'e') goto S70;
  if (c1 == 'E') goto S70;
  if ('0' <= c1 && c1 <= '9') goto S74;
  if (c1 == '.') goto S68;
  return m.FSM_HALT(c1);

S45:
  m.FSM_TAKE(7);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  return m.FSM_HALT(c1);

S48:
  m.FSM_TAKE(8);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  return m.FSM_HALT(c1);

S51:
  m.FSM_TAKE(1);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if (c1 == ' ') goto S80;
  if (c1 == '\r') goto S80;
  if ('\t' <= c1 && c1 <= '\n') goto S80;
  return m.FSM_HALT(c1);

S57:
  m.FSM_TAKE(8);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S48;
  if ('1' <= c1 && c1 <= '9') goto S74;
  if (c1 == '0') goto S85;
  return m.FSM_HALT(c1);

S62:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S90;
  return m.FSM_HALT(c1);

S64:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S92;
  return m.FSM_HALT(c1);

S66:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S94;
  return m.FSM_HALT(c1);

S68:
  c1 = m.FSM_CHAR();
  if ('0' <= c1 && c1 <= '9') goto S96;
  return m.FSM_HALT(c1);

S70:
  c1 = m.FSM_CHAR();
  if ('0' <= c1 && c1 <= '9') goto S103;
  if (c1 == '-') goto S101;
  if (c1 == '+') goto S101;
  return m.FSM_HALT(c1);

S74:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S70;
  if (c1 == 'E') goto S70;
  if ('0' <= c1 && c1 <= '9') goto S74;
  if (c1 == '.') goto S68;
  return m.FSM_HALT(c1);

S80:
  m.FSM_TAKE(1);
  c1 = m.FSM_CHAR();
  if (c1 == ' ') goto S80;
  if (c1 == '\r') goto S80;
  if ('\t' <= c1 && c1 <= '\n') goto S80;
  return m.FSM_HALT(c1);

S85:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S70;
  if (c1 == 'E') goto S70;
  if (c1 == '.') goto S68;
  return m.FSM_HALT(c1);

S90:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S106;
  return m.FSM_HALT(c1);

S92:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S108;
  return m.FSM_HALT(c1);

S94:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S110;
  return m.FSM_HALT(c1);

S96:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S70;
  if (c1 == 'E') goto S70;
  if ('0' <= c1 && c1 <= '9') goto S96;
  return m.FSM_HALT(c1);

S101:
  c1 = m.FSM_CHAR();
  if ('0' <= c1 && c1 <= '9') goto S103;
  return m.FSM_HALT(c1);

S103:
  m.FSM_TAKE(6);
  c1 = m.FSM_CHAR();
  if ('0' <= c1 && c1 <= '9') goto S103;
  return m.FSM_HALT(c1);

S106:
  m.FSM_TAKE(3);
  return m.FSM_HALT();

S108:
  m.FSM_TAKE(4);
  return m.FSM_HALT();

S110:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S112;
  return m.FSM_HALT(c1);

S112:
  m.FSM_TAKE(5);
  return m.FSM_HALT();
}

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

void reflex_code_STRING(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S54;
  if (241 <= c1 && c1 <= 243) goto S51;
  if (c1 == 240) goto S47;
  if (238 <= c1 && c1 <= 239) goto S44;
  if (c1 == 237) goto S40;
  if (225 <= c1 && c1 <= 236) goto S37;
  if (c1 == 224) goto S33;
  if (194 <= c1 && c1 <= 223) goto S30;
  if (c1 == '\\') goto S15;
  if (c1 == '"') goto S12;
  if (' ' <= c1 && c1 <= 127) goto S27;
  if (0 <= c1) goto S58;
  return m.FSM_HALT(c1);

S12:
  m.FSM_TAKE(1);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S58;
  return m.FSM_HALT(c1);

S15:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S58;
  if (c1 == 'u') goto S73;
  if (c1 == 't') goto S65;
  if (c1 == 'r') goto S71;
  if (c1 == 'n') goto S67;
  if (c1 == 'f') goto S69;
  if (c1 == 'b') goto S63;
  if (c1 == '\\') goto S61;
  if (c1 == '/') goto S61;
  if (c1 == '"') goto S61;
  return m.FSM_HALT(c1);

S27:
  m.FSM_TAKE(9);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S58;
  return m.FSM_HALT(c1);

S30:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S33:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (160 <= c1 && c1 <= 191) goto S80;
  if (128 <= c1 && c1 <= 159) goto S58;
  return m.FSM_HALT(c1);

S37:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S83;
  return m.FSM_HALT(c1);

S40:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (160 <= c1 && c1 <= 191) goto S58;
  if (128 <= c1 && c1 <= 159) goto S86;
  return m.FSM_HALT(c1);

S44:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S89;
  return m.FSM_HALT(c1);

S47:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (144 <= c1 && c1 <= 191) goto S92;
  if (128 <= c1 && c1 <= 143) goto S58;
  return m.FSM_HALT(c1);

S51:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S54:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (144 <= c1 && c1 <= 191) goto S58;
  if (128 <= c1 && c1 <= 143) goto S98;
  return m.FSM_HALT(c1);

S58:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S58;
  return m.FSM_HALT(c1);

S61:
  m.FSM_TAKE(2);
  return m.FSM_HALT();

S63:
  m.FSM_TAKE(3);
  return m.FSM_HALT();

S65:
  m.FSM_TAKE(4);
  return m.FSM_HALT();

S67:
  m.FSM_TAKE(5);
  return m.FSM_HALT();

S69:
  m.FSM_TAKE(6);
  return m.FSM_HALT();

S71:
  m.FSM_TAKE(7);
  return m.FSM_HALT();

S73:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S101;
  if ('A' <= c1 && c1 <= 'F') goto S101;
  if ('0' <= c1 && c1 <= '9') goto S101;
  return m.FSM_HALT(c1);

S77:
  m.FSM_TAKE(10);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S58;
  return m.FSM_HALT(c1);

S80:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S83:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S86:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S89:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S92:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S105;
  return m.FSM_HALT(c1);

S95:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S108;
  return m.FSM_HALT(c1);

S98:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S111;
  return m.FSM_HALT(c1);

S101:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S114;
  if ('A' <= c1 && c1 <= 'F') goto S114;
  if ('0' <= c1 && c1 <= '9') goto S114;
  return m.FSM_HALT(c1);

S105:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S108:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S111:
  m.FSM_TAKE(11);
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S77;
  return m.FSM_HALT(c1);

S114:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S118;
  if ('A' <= c1 && c1 <= 'F') goto S118;
  if ('0' <= c1 && c1 <= '9') goto S118;
  return m.FSM_HALT(c1);

S118:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S122;
  if ('A' <= c1 && c1 <= 'F') goto S122;
  if ('0' <= c1 && c1 <= '9') goto S122;
  return m.FSM_HALT(c1);

S122:
  m.FSM_TAKE(8);
  return m.FSM_HALT();
}

