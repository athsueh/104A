%{
// $Id: parser.y,v 1.6 2015-04-16 17:17:22-07 - - $

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lyutils.h"
#include "astree.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#define YYPRINT yyprint
#define YYMALLOC yycalloc

static void* yycalloc (size_t size);

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%destructor { error_destructor ($$); } <>

%token  ROOT IDENT NUMBER

%right  '='
%left   '+' '-'
%left   '*' '/'
%right  '^'
%right  POS NEG

%start  program


%%

program : stmtseq               { $$ = $1; }
        ;

stmtseq : stmtseq expr ';'      { free_ast ($3); $$ = adopt1 ($1, $2); }
        | stmtseq error ';'     { free_ast ($3); $$ = $1; }
        | stmtseq ';'           { free_ast ($2); $$ = $1; }
        |                       { $$ = new_parseroot(); }
        ;

expr    : expr '=' expr         { $$ = adopt2 ($2, $1, $3); }
        | expr '+' expr         { $$ = adopt2 ($2, $1, $3); }
        | expr '-' expr         { $$ = adopt2 ($2, $1, $3); }
        | expr '*' expr         { $$ = adopt2 ($2, $1, $3); }
        | expr '/' expr         { $$ = adopt2 ($2, $1, $3); }
        | expr '^' expr         { $$ = adopt2 ($2, $1, $3); }
        | '+' expr %prec POS    { $$ = adopt1sym ($1, $2, POS); }
        | '-' expr %prec NEG    { $$ = adopt1sym ($1, $2, NEG); }
        | '(' expr ')'          { free_ast2 ($1, $3); $$ = $2; }
        | IDENT                 { $$ = $1; }
        | NUMBER                { $$ = $1; }
        ;

%%

const char* get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}

bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

static void* yycalloc (size_t size) {
   void* result = calloc (1, size);
   assert (result != NULL);
   return result;
}

RCSC("$Id: parser.y,v 1.6 2015-04-16 17:17:22-07 - - $")

