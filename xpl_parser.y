%{
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
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
%token <s> tIDENTIFIER tSTRING tUSE tPUBLIC tPLINE tSTRG "+" '-'
%token <type> tTYPEINT tTYPEREAL tTYPESTRING tVOID
%token tWHILE tIF tELSIF tELSE tSWEEP tNULL
%token tNEXT tSTOP tRETURN

%nonassoc tIFX tELSE tSTRCAT
%nonassoc tINTEGER tREAL tSTRING tIDENTIFIER
%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '|' '&'
%right tSTRG
%nonassoc tUNARY '~'
%nonassoc tPUBLIC tUSE tVOID
%nonassoc tSTOP tNEXT tRETURN tNULL tTYPEINT tTYPEREAL tTYPESTRING
%left tBOP '{' '('
%right tBCL '}' ')'

%type <node> block decl var func inst iter cond
%type <sequence> file decls insts vars args
%type <expression> expr lit
%type <lvalue> lval
%type <type> type
%type <s> qual sdir strg

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

type  : tTYPESTRING     { $$ = new basic_type(4, basic_type::TYPE_STRING); }
      | tTYPEREAL       { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
      | tTYPEINT        { $$ = new basic_type(4, basic_type::TYPE_INT); }
      | tVOID           { $$ = new basic_type(4, basic_type::TYPE_VOID); }
      | tBOP type tBCL  { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
      ;

qual  : tUSE          { $$ = $1; }
      | tPUBLIC       { $$ = $1; }
      |               { $$ = nullptr; }
      ;

var   : qual type tIDENTIFIER           { $$ = new xpl::declaration_node(LINE, $1, $2, $3, nullptr); }
      | qual type tIDENTIFIER '=' expr  { $$ = new xpl::declaration_node(LINE, $1, $2, $3, $5); }
      ;

func  : qual type tIDENTIFIER '(' args ')'                  { $$ = new xpl::function_declaration_node(LINE, $1, $2, $3, $5); }

      | qual type tIDENTIFIER '(' args ')' block            { $$ = new xpl::function_definition_node(LINE, $1, $2, $3, $5, $7, nullptr); }
      | qual type tIDENTIFIER '(' args ')' '=' lit          { $$ = new xpl::function_definition_node(LINE, $1, $2, $3, $5, nullptr, $8); }
      | qual type tIDENTIFIER '(' args ')' '=' lit block    { $$ = new xpl::function_definition_node(LINE, $1, $2, $3, $5, $9, $8); }
      ;

args  : vars    { $$ = $1; }
      |         { $$ = nullptr; }
      ;

vars  : var                   { $$ = new cdk::sequence_node(LINE, $1); }
      | vars ',' var          { $$ = new cdk::sequence_node(LINE, $3 ,$1); }
      ;

block : '{' '}'               { $$ = new xpl::block_node(LINE, nullptr, nullptr); }
      | '{' decls '}'         { $$ = new xpl::block_node(LINE, $2, nullptr); }
      | '{' insts '}'         { $$ = new xpl::block_node(LINE, nullptr, $2); }
      | '{' decls insts '}'   { $$ = new xpl::block_node(LINE, $2, $3); }
      ;

insts : inst                  { $$ = new cdk::sequence_node(LINE, $1); }
      | insts inst            { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

inst  : expr ';'              { $$ = new xpl::evaluation_node(LINE, $1); }
      | expr '!'              { $$ = new xpl::print_node(LINE, $1); }
      | expr tPLINE           { $$ = new xpl::print_node(LINE, $1+'\n'); }
      | tNEXT                 { $$ = new xpl::next_node(LINE); }
      | tSTOP                 { $$ = new xpl::stop_node(LINE); }
      | tRETURN               { $$ = new xpl::return_node(LINE); }
      | cond                  { $$ = $1; }
      | iter                  { $$ = $1; }
      | block                 { $$ = $1; }
      ;

cond  : tIF '(' expr ')' inst %prec tIFX          { $$ = new xpl::if_node(LINE, $3, $5); }
      | tIF '(' expr ')' inst tELSE inst          { $$ = new xpl::if_else_node(LINE, $3, $5, $7); }
      ;

sdir  : '+'  { $$ = $1; }
      | '-'  { $$ = $1; }
      ;

iter  : tWHILE '(' expr ')' inst                                    { $$ = new xpl::while_node(LINE, $3, $5); }
      | tSWEEP sdir '(' lval ':' expr ':' expr ')' inst             { $$ = new xpl::sweep_node(LINE, $2, $4, $6, $8, nullptr, $10); }
      | tSWEEP sdir '(' lval ':' expr ':' expr ':' expr ')' inst    { $$ = new xpl::sweep_node(LINE, $2, $4, $6, $8, $10, $12); }
      ;

strg  : tSTRING         { $$ = $1; }
      | strg tSTRING    { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
      ;

lit   : tREAL                     { $$ = new cdk::double_node(LINE, $1); }
      | tINTEGER                  { $$ = new cdk::integer_node(LINE, $1); }
      | strg %prec tSTRCAT        { $$ = new cdk::string_node(LINE, $1); }
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
      | '~' expr                  { $$ = new cdk::neg_node(LINE, $2); }
      | expr '|' expr             { $$ = new cdk::or_node(LINE, $1, $3);}
      | expr '&' expr             { $$ = new cdk::or_node(LINE, $1, $3);}
      | '(' expr ')'              { $$ = $2; }
      | lval                      { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
      | lval '=' expr             { $$ = new cdk::assignment_node(LINE, $1, $3); }
      | tIDENTIFIER '(' args ')'  { $$ = new xpl::function_call_node(LINE, $1, $3); }
      ;

lval  : tIDENTIFIER               { $$ = new cdk::identifier_node(LINE, $1); }
      | tIDENTIFIER '?'           { /* TODO */ }
      | tIDENTIFIER '[' expr ']'  { $$ = new xpl::index_node(LINE, $1 ,$3); }
      ;

%%
