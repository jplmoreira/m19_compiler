%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

// don't change this
#define yyerror LexerError

std::string real(const char *number);
%}

ENG [afpnumkMGTPE]
HEX [0-9a-fA-F]

%x X_STRING X_COMMENT X_HEX X_SPECIAL
%%

"//".*                 ; /* ignore comments */
"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|\n        ; /* ignore comments */

">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;

"<<"                   return tINIT;
">>"                   return tFIN;

"&&"                   return tAND;
"||"                   return tOR;

">>@"                  return tBREAK;
"@>>"                  return tCONTINUE;
">@<"                  return tRETURN;
"!!"                   return tPRINTLN;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                        yylval.s = new std::string(""); yy_push_state(X_STRING);
<X_STRING>\\\"            *yylval.s += yytext + 1;
<X_STRING>\\\\            *yylval.s += yytext + 1;
<X_STRING>\\n             *yylval.s += '\n';
<X_STRING>\\r             *yylval.s += '\r';
<X_STRING>\\t		      *yylval.s += '\t';
<X_STRING>\\{HEX}({HEX})? *yylval.s += strtoul(yytext+1, nullptr, 16);
<X_STRING>\"              yy_pop_state(); return tSTRING;
<X_STRING>.               *yylval.s += yytext;

"0"[xX]                yy_push_state(X_HEX);
<X_HEX>[a-fA-F0-9]+    try {
                         yylval.i = std::stoi(yytext, nullptr, 16); yy_pop_state(); 
                         return tINTEGER;
                       } catch (std::out_of_range&) {
                         yyerror("hexadecimal overflow");
                       }

[0-9]+\.[0-9]+         try {
                         yylval.d = std::stod(yytext);
                         return tREAL;
                       } catch (std::out_of_range&) {
                         yyerror("double overflow");
                       }

[0-9]+{ENG}[0-9]+      try {
                         yylval.d = std::stod(real(yytext));
                         return tREAL;
                       } catch (std::out_of_range&) {
                         yyerror("double overflow");
                       }

[0-9]+                 try {
                         yylval.i = strtol(yytext, nullptr, 10);
                         return tINTEGER;
                       } catch (std::out_of_range&) {
                         yyerror("integer overflow");
                       }


[-()<>=+*/%;{}#$@!?,\[\]~:] return *yytext;

[ \t\n\r]+               ; /* ignore whitespace */

.                      yyerror(("Unknown character: " + std::string(yytext)).c_str());

%%

std::string real(const char *input) {
  std::string number(input);
  std::string eng = "afpnumkMGTPE";
  std::string res = "";
  std::string p = "";
  for (char& c : number) {
      if (eng.find(c) != std::string::npos){
          switch(c) {
            case 'a': p = "e-18";
                      break;
            case 'f': p = "e-15";
                      break;

            case 'p': p = "e-12";
                      break;

            case 'n': p = "e-9";
                      break;

            case 'u': p = "e-6";
                      break;

            case 'm': p = "e-3";
                      break;

            case 'k': p = "e+3";
                      break;

            case 'M': p = "e+6";
                      break;

            case 'G': p = "e+9";
                      break;

            case 'T': p = "e+12";
                      break;

            case 'P': p = "e+15";
                      break;

            case 'E': p = "e+18";
                      break;
         }
         res += '.';

      } else
         res += c;
  }
  res += p;
  return res;
}
