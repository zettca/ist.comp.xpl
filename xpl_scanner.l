%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
/* $Id: xpl_scanner.l,v 1.5 2017/04/21 15:06:00 ist178013 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

DEC     [0-9]
HEX     [0-9A-Fa-f]

%x X_STRING X_ESCAPE X_IGNORE X_COMMENT
%%

    { set_debug(1); }

"//".*                  ; /* ignore comments */
"/*"                    yy_push_state(X_COMMENT);
<X_COMMENT>"*/"         yy_pop_state();
<X_COMMENT>.            ;
<X_COMMENT>\n           ;

">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;

"if"                   return tIF;
"elsif"                return tELSIF;
"else"                 return tELSE;
"while"                return tWHILE;
"sweep"                return tSWEEP;

"null"                 return tNULL;
"int"                  return tTYPEINT;
"real"                 return tTYPEREAL;
"string"               return tTYPESTRING;
"procedure"            return tPROCEDURE;

"use"                  return tUSE;
"public"               return tPUBLIC;

"next"                 return tNEXT;
"stop"                 return tSTOP;
"return"               return tRETURN;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>\\           yy_push_state(X_ESCAPE);
<X_STRING>\\\"         *yylval.s += yytext + 1;
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           ;

<X_ESCAPE>00?          yy_pop_state(); yy_push_state(X_IGNORE);
<X_ESCAPE>r            yy_pop_state(); *yylval.s += '\r';
<X_ESCAPE>n            yy_pop_state(); *yylval.s += '\n';
<X_ESCAPE>t            yy_pop_state(); *yylval.s += '\t';
<X_ESCAPE>{HEX}{1,2}   yy_pop_state(); *yylval.s += '0' + strtol(yytext, nullptr, 16);
<X_ESCAPE>.            yy_pop_state();

<X_IGNORE>\"           yy_pop_state();
<X_IGNORE>.            ;
<X_IGNORE>\n           ;

{DEC}*\.{DEC}+([eE][-+]?{DEC}+)?    yylval.r = strtod(yytext, nullptr); return tREAL;
{HEX}+                              yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

[-()<>=+*/%;{}.]       return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }
