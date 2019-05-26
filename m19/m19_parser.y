%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  double                d;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;       /* expression type */

  m19::section_initial_node *init;
  m19::section_final_node *finl;
  m19::body_node *bdy;
  m19::block_node *blck;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING

%token tREAD tOR tAND
%token tINIT tFIN
%token tBREAK tCONTINUE tRETURN tPRINTLN

%nonassoc tIFX
%nonassoc ':'

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%nonassoc '[' ']'

%type <node> stmt vardecl fundef declaration condition innerdecl
%type <sequence> stmts file declarations params vardecls conditions innerdecls innerdeclc forvars exprs exprl
%type <expression> expr defret literal
%type <lvalue> lval
%type <type> data_type
%type <init> initial
%type <finl> fin
%type <blck> block
%type <bdy> body
%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file           : /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); }
               | declarations { compiler->ast($$ = $1); }
               ;

declarations   : declaration              { $$ = new cdk::sequence_node(LINE, $1);     }
               | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

declaration    : vardecl ';' { $$ = $1; }
               | fundef      { $$ = $1; }
               ;

vardecl : data_type tIDENTIFIER '=' expr     { $$ = new m19::var_decl_node(LINE, 0, $1, *$2, $4); delete $2;      }
        | data_type tIDENTIFIER              { $$ = new m19::var_decl_node(LINE, 0, $1, *$2, nullptr); delete $2; }
        | data_type tIDENTIFIER '!' '=' expr { $$ = new m19::var_decl_node(LINE, 1, $1, *$2, $5); delete $2;      }
        | data_type tIDENTIFIER '!'          { $$ = new m19::var_decl_node(LINE, 1, $1, *$2, nullptr); delete $2; }
        | data_type tIDENTIFIER '?'          { $$ = new m19::var_decl_node(LINE, 2, $1, *$2, nullptr); delete $2; }
        ;

data_type : '#'               { $$ = new basic_type(4, basic_type::TYPE_INT);    }
          | '%'               { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
          | '$'               { $$ = new basic_type(4, basic_type::TYPE_STRING); }
          | '<' data_type '>' { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
          ;

fundef    : data_type tIDENTIFIER params     { $$ = new m19::function_declaration_node(LINE, 0, $1, *$2, $3); delete $2; }
          | '!' tIDENTIFIER params           { $$ = new m19::function_declaration_node(LINE, 0, *$2, $3); delete $2;     }
          | data_type tIDENTIFIER '!' params { $$ = new m19::function_declaration_node(LINE, 1, $1, *$2, $4); delete $2; }
          | '!' tIDENTIFIER '!' params       { $$ = new m19::function_declaration_node(LINE, 1, *$2, $4); delete $2;     }
          | data_type tIDENTIFIER '?' params { $$ = new m19::function_declaration_node(LINE, 2, $1, *$2, $4); delete $2; }
          | '!' tIDENTIFIER '?' params       { $$ = new m19::function_declaration_node(LINE, 2, *$2, $4); delete $2;     }

          | data_type tIDENTIFIER params defret body     { $$ = new m19::function_definition_node(LINE, 0, $1, *$2, $3, $5, $4); delete $2; }
          | '!' tIDENTIFIER params body                  { $$ = new m19::function_definition_node(LINE, 0, *$2, $3, $4); delete $2;         }
          | data_type tIDENTIFIER '!' params defret body { $$ = new m19::function_definition_node(LINE, 1, $1, *$2, $4, $6, $5); delete $2; }
          | '!' tIDENTIFIER '!' params body              { $$ = new m19::function_definition_node(LINE, 1, *$2, $4, $5); delete $2;         }
          ;

params   : '(' ')'          { $$ = new cdk::sequence_node(LINE); }
         | '(' vardecls ')' { $$ = $2; }
         ;

vardecls : vardecl              { $$ = new cdk::sequence_node(LINE, $1);     }
         | vardecls ',' vardecl { $$ = new cdk::sequence_node(LINE, $3, $1); }
         ;

defret   : /* empty */          { $$ = nullptr; }
         | '=' literal          { $$ = $2; }
         ;

body     : initial                { $$ = new m19::body_node(LINE, $1, new cdk::sequence_node(LINE), nullptr); }
         | fin                    { $$ = new m19::body_node(LINE, nullptr, new cdk::sequence_node(LINE), $1); }
         | conditions             { $$ = new m19::body_node(LINE, nullptr, $1, nullptr); }
         | initial conditions     { $$ = new m19::body_node(LINE, $1, $2, nullptr); }
         | conditions fin         { $$ = new m19::body_node(LINE, nullptr, $1, $2); }
         | initial conditions fin { $$ = new m19::body_node(LINE, $1, $2, $3); }
         ;

initial  : tINIT block          { $$ = new m19::section_initial_node(LINE, $2); }
         ;

fin      : tFIN block           { $$ = new m19::section_final_node(LINE, $2); }
         ;

conditions : condition            { $$ = new cdk::sequence_node(LINE, $1);     }
           | conditions condition { $$ = new cdk::sequence_node(LINE, $2, $1); }
           ;

condition  : block                { $$ = new m19::section_conditional_node(LINE, 0, nullptr, $1); }
           | '(' ')' block        { $$ = new m19::section_conditional_node(LINE, 0, nullptr, $3); }
           | '(' expr ')' block   { $$ = new m19::section_conditional_node(LINE, 0, $2, $4);      }
           | '[' ']' block        { $$ = new m19::section_conditional_node(LINE, 1, nullptr, $3); }
           | '[' expr ']' block   { $$ = new m19::section_conditional_node(LINE, 1, $2, $4);      }
           ;

block : '{' '}'                  { $$ = new m19::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
      | '{' innerdecls '}'       { $$ = new m19::block_node(LINE, $2, new cdk::sequence_node(LINE)); }
      | '{' stmts '}'            { $$ = new m19::block_node(LINE, new cdk::sequence_node(LINE), $2); }
      | '{' innerdecls stmts '}' { $$ = new m19::block_node(LINE, $2, $3); }
      ;

innerdecls : innerdecl ';'            { $$ = new cdk::sequence_node(LINE, $1); }
           | innerdecls innerdecl ';' { $$ = new cdk::sequence_node(LINE, $2, $1); }
           ;

innerdecl  : data_type tIDENTIFIER '=' expr     { $$ = new m19::var_decl_node(LINE, 0, $1, *$2, $4); delete $2;      }
           | data_type tIDENTIFIER              { $$ = new m19::var_decl_node(LINE, 0, $1, *$2, nullptr); delete $2; }
           ;

innerdeclc : innerdecl                       { $$ = new cdk::sequence_node(LINE, $1);     }
           | innerdeclc ',' innerdecl        { $$ = new cdk::sequence_node(LINE, $3, $1); }
           ;

forvars : /* empty */      { $$ = new cdk::sequence_node(LINE); }
        | innerdeclc       { $$ = $1; }
        | exprs            { $$ = $1; }
        ;

stmts : stmt         { $$ = new cdk::sequence_node(LINE, $1);     }
	    | stmts stmt   { $$ = new cdk::sequence_node(LINE, $2, $1); }
	    ;

stmt : expr ';'                                          { $$ = new m19::evaluation_node(LINE, $1); }
     | expr '!'                                          { $$ = new m19::print_node(LINE, $1, false); }
     | expr tPRINTLN                                     { $$ = new m19::print_node(LINE, $1, true); }
     | tBREAK                                            { $$ = new m19::break_node(LINE); }
     | tCONTINUE                                         { $$ = new m19::continue_node(LINE); }
     | tRETURN                                           { $$ = new m19::return_node(LINE); }
     | '[' forvars ';' exprl ';' exprl ']' stmt          { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
     | '[' expr ']' '#' stmt                             { $$ = new m19::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' stmt %prec tIFX                  { $$ = new m19::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' stmt ':' stmt                    { $$ = new m19::if_else_node(LINE, $2, $5, $7); }
     | block                                             { $$ = $1; }
     ;

expr : literal                   { $$ = $1; }
     | lval                      { $$ = new cdk::rvalue_node(LINE, $1); }
     | lval '=' expr             { $$ = new cdk::assignment_node(LINE, $1, $3); }
     | '@' '=' expr              { $$ = new cdk::assignment_node(LINE, new cdk::variable_node(LINE, "@"), $3); }
     | '@'                       { $$ = new m19::read_node(LINE); }
     | '~' expr                  { $$ = new cdk::not_node(LINE, $2); }
     | '-' expr %prec tUNARY     { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY     { $$ = new m19::identity_node(LINE, $2); }
     | lval '?' %prec tUNARY     { $$ = new m19::address_of_node(LINE, $1); }
     | expr '+' expr	           { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	           { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	           { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	           { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	           { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	           { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	           { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	           { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr             { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	           { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	           { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr tOR expr             { $$ = new cdk::or_node(LINE, $1, $3); }
     | expr tAND expr            { $$ = new cdk::and_node(LINE, $1, $3); }
     | '(' expr ')'              { $$ = $2; }
     | '[' expr ']'              { $$ = new m19::mem_alloc_node(LINE, $2); }
     | tIDENTIFIER '(' ')'       { $$ = new m19::function_call_node(LINE, *$1); delete $1; }
     | tIDENTIFIER '(' exprs ')' { $$ = new m19::function_call_node(LINE, *$1, $3); delete $1; }
     | '@' '(' exprs ')'         { $$ = new m19::function_call_node(LINE, "@", $3); }
     ;

exprs : expr              { $$ = new cdk::sequence_node(LINE, $1);     }
      | exprs ',' expr    { $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;

exprl : /* empty */      { $$ = new cdk::sequence_node(LINE); }
      | exprs            { $$ = $1; }
      ;

literal : tINTEGER       { $$ = new cdk::integer_node(LINE, $1); }
        | tREAL          { $$ = new cdk::double_node(LINE, $1); }
        | string         { $$ = new cdk::string_node(LINE, $1); }
        ;

string  :        tSTRING { $$ = $1; }
        | string tSTRING { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
        ;

lval    : tIDENTIFIER        { $$ = new cdk::variable_node(LINE, $1); }
        | expr '[' expr ']'  { $$ = new m19::index_node(LINE, $1, $3); }
        ;

%%
