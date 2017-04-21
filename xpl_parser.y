%{
// $Id: xpl_parser.y,v 1.7 2017/04/21 15:52:43 ist178013 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;  /* integer value */
  double                r;  /* real value */
  std::string          *s;  /* symbol name or string literal */
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;
};

%token <i> tINTEGER
%token <r> tREAL
%token <s> tIDENTIFIER tSTRING tUSE tPUBLIC
%token tWHILE tIF tSWEEP tNULL
%token tTYPEINT tTYPEREAL tTYPESTRING tPROCEDURE tNEXT tSTOP tRETURN

%nonassoc tIFX
%nonassoc tELSIF
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> block decl var func inst iter cond
%type <sequence> file decls insts args
%type <expression> expr lit 
%type <lvalue> lval
%type <type> type

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file  : decls         { compiler->ast($1); }
      ;

decls : decl          { $$ = new cdk::sequence_node(LINE, $1); }
      | decls decl    { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

decl  : var ';'       { $$ = $1; }
      | func          { $$ = $1; }
      ;

type  : tTYPESTRING   { $$ = new basic_type(4, basic_type::TYPE_STRING); } /* 4 bytes? */
      | tTYPEREAL     { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
      | tTYPEINT      { $$ = new basic_type(4, basic_type::TYPE_INT); }
      | '[' type ']'  { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
      ;

var   : type tIDENTIFIER          { $$ = new xpl::declaration_node(LINE, nullptr, $1, $2); }
      | tPUBLIC type tIDENTIFIER  { $$ = new xpl::declaration_node(LINE, $1, $2, $3); }
      | tUSE type tIDENTIFIER     { $$ = new xpl::declaration_node(LINE, $1, $2, $3); }
      ;

func  : type tIDENTIFIER '(' ')'                            { $$ = new xpl::function_declaration_node(LINE, nullptr, $1, $2, nullptr); }
      | type tIDENTIFIER '(' args ')'                       { $$ = new xpl::function_declaration_node(LINE, nullptr, $1, $2, $4); }
      | tUSE type tIDENTIFIER '(' ')'                       { $$ = new xpl::function_declaration_node(LINE, $1, $2, $3, nullptr); }
      | tUSE type tIDENTIFIER '(' args ')'                  { $$ = new xpl::function_declaration_node(LINE, $1, $2, $3, $5); }
      | tPUBLIC type tIDENTIFIER '(' ')'                    { $$ = new xpl::function_declaration_node(LINE, $1, $2, $3, nullptr); }
      | tPUBLIC type tIDENTIFIER '(' args ')'               { $$ = new xpl::function_declaration_node(LINE, $1, $2, $3, $5); }

      | type tIDENTIFIER '(' args ')' block                 { $$ = new xpl::function_definition_node(LINE, nullptr, $1, $2, $4, $6, nullptr); }
      | tPUBLIC type tIDENTIFIER '(' args ')' block         { $$ = new xpl::function_definition_node(LINE, $1, $2, $3, $5, $7, nullptr); }
      | type tIDENTIFIER '(' args ')' '=' lit block         { $$ = new xpl::function_definition_node(LINE, nullptr, $1, $2, $4, $8, $7); }
      | tPUBLIC type tIDENTIFIER '(' args ')' '=' lit block { $$ = new xpl::function_definition_node(LINE, $1, $2, $3, $5, $9, $8); }
      ;

args  : var                     { $$ = new cdk::sequence_node(LINE, $1); }
      | args ',' var            { $$ = new cdk::sequence_node(LINE, $3 ,$1); }
      ;

block : '{' decls '}'           { $$ = new xpl::block_node(LINE, $2, nullptr); }
      | '{' insts '}'           { $$ = new xpl::block_node(LINE, nullptr, $2); }
      | '{' decls insts '}'     { $$ = new xpl::block_node(LINE, $2, $3); }
      ;

insts : inst                    { $$ = new cdk::sequence_node(LINE, $1); }
      | insts inst              { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

inst  : expr ';'                { $$ = new xpl::evaluation_node(LINE, $1); }
      | expr '!'                { $$ = new xpl::print_node(LINE, $1); }
      | expr '!!'               { $$ = new xpl::print_node(LINE, $1); }
      | tNEXT                   { $$ = new xpl::next_node(LINE); }
      | tSTOP                   { $$ = new xpl::stop_node(LINE); }
      | tRETURN                 { $$ = new xpl::return_node(LINE); }
      | cond                    { $$ = $1; }
      | iter                    { $$ = $1; }
      | block                   { $$ = $1; }
      ;

cond  : tIF '(' expr ')' inst %prec tIFX          { $$ = new xpl::if_node(LINE, $3, $5); }
      | tIF '(' expr ')' inst tELSE inst          { $$ = new xpl::if_else_node(LINE, $3, $5, $7); }
      ;

iter  :  tWHILE '(' expr ')' inst                                 { $$ = new xpl::while_node(LINE, $3, $5); }
      | tSWEEP '+' '(' lval ':' expr ':' expr ')' inst            { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, nullptr, $10); }
      | tSWEEP '+' '(' lval ':' expr ':' expr ':' expr ')' inst   { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, $10, $12); }
      | tSWEEP '-' '(' lval ':' expr ':' expr ')' inst            { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, nullptr, $10); }
      | tSWEEP '-' '(' lval ':' expr ':' expr ':' expr ')' inst   { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, $10, $12); }
      ;

lit   : tREAL                     { $$ = new cdk::double_node(LINE, $1); }
      | tINTEGER                  { $$ = new cdk::integer_node(LINE, $1); }
      | tSTRING                   { $$ = new cdk::string_node(LINE, $1); }
      ;

expr  : lit                       { $$ = $1; }
      | '-' expr %prec tUNARY     { $$ = new cdk::neg_node(LINE, $2); }
      | expr '+' expr             { $$ = new cdk::add_node(LINE, $1, $3); }
      | expr '-' expr             { $$ = new cdk::sub_node(LINE, $1, $3); }
      | expr '*' expr             { $$ = new cdk::mul_node(LINE, $1, $3); }
      | expr '/' expr             { $$ = new cdk::div_node(LINE, $1, $3); }
      | expr '%' expr             { $$ = new cdk::mod_node(LINE, $1, $3); }
      | expr '<' expr             { $$ = new cdk::lt_node(LINE, $1, $3); }
      | expr '>' expr             { $$ = new cdk::gt_node(LINE, $1, $3); }
      | expr tGE expr             { $$ = new cdk::ge_node(LINE, $1, $3); }
      | expr tLE expr             { $$ = new cdk::le_node(LINE, $1, $3); }
      | expr tNE expr             { $$ = new cdk::ne_node(LINE, $1, $3); }
      | expr tEQ expr             { $$ = new cdk::eq_node(LINE, $1, $3); }
      | '(' expr ')'              { $$ = $2; }
      | lval                      { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
      | lval '=' expr             { $$ = new cdk::assignment_node(LINE, $1, $3); }
      | tIDENTIFIER '(' args ')'  { $$ = new xpl::function_call_node(LINE, $1, $3); }
      ;

lval  : tIDENTIFIER               { $$ = new cdk::identifier_node(LINE, $1); }
      ;

%%
