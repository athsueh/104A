%{
// Dummy parser for scanner project.

#include "lyutils.h"
#include "astree.h"
#include <assert.h>
//fix variable
%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT

%token TOK_RETURNVOID TOK_PARAM TOK_PROTOTYPE TOK_DECLID
%token TOK_NEWSTRING TOK_VARDECL TOK_INDEX TOK_FUNCTION

%right TOK_IFELSE TOK_IF TOK_ELSE
%right '=' 
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_ORD TOK_CHR TOK_POS TOK_NEG '!'
%left TOK_ARRAY TOK_FIELD TOK_FUNCTION '.' '['
%nonassoc TOK_NEW
%nonassoc TOK_PARENS

%start start

%%
start    : program               {yyparse_astree = $1;}
         ;

program  : program structdef     {$$ = adopt1($1, $2)}
         | program function      {$$ = adopt1($1, $2)}
         | program statement     {$$ = adopt1($1, $2)}
         | program error '}'     {$$ = adopt1($1, $2)}
         | program error ';'     {$$ = adopt1($1, $2)}
         |                       {$$ = new_parseroot();}
         ;

structdef: structdh '}'          {$$ = $1;}

structdh : structdh fielddecl ';'
               {free_ast($3); $$ = adopt1($1, $2);}
         | TOK_STRUCT TOK_IDENT '{'            
               {free_ast($3); $$ = adopt1sym($1, $2, TOK_TYPEID);}

fielddecl: basetype TOK_ARRAY TOK_IDENT  
               {$$ = adopt1sym($2, $1, TOK_FIELD);}
         | basetype TOK_IDENT            
             {$$ = adopt1sym($1, $2, TOK_FIELD);}
         ;

basetype : TOK_VOID        {$$ = $1}
         | TOK_BOOL        {$$ = $1}
         | TOK_CHAR        {$$ = $1}
         | TOK_INT         {$$ = $1}
         | TOK_STRING      {$$ = $1}
         | TOK_TYPEID      {$$ = $1}
         | TOK_IDENT       {$$ = $1}
         ;   

function : identdecl paramhead ')' block        
               {free_ast($3); $$ = new_function($1, $2, $4);}

paramhead: paramhead ',' identdecl {$$ = adopt1($1, $3);}
         | '(' identdecl                  
               {$$ = adopt1sym($1, $2, TOK_PARAM);}
         | '('                            
               {$$ = change_sym($1, TOK_PARAM);}

identdecl: basetype TOK_ARRAY TOK_IDENT 
               {$1 = change_sym($1, TOK_DECLID);
               $$ = adopt2($2, $1, $3);}
         | basetype TOK_IDENT           
               {$$ = adopt1sym($1, $2, TOK_DECLID);}
         ;

block    : blockhead '}'         
               {free_ast($2); $$ = change_sym($1, TOK_BLOCK);}
         | '{' '}'               
               {free_ast($2);
               $$ = change_sym($1, TOK_BLOCK);}
         | ';'                   
               {$$ = change_sym($1, TOK_BLOCK); }
         ;

blockhead: blockhead statement   {adopt1($1, $2);}
         | '{' statement         {$$ = $1;}

statement: block        {$$ = $1}
         | vardecl      {$$ = $1}
         | while        {$$ = $1}
         | ifelse       {$$ = $1}
         | return       {$$ = $1}
         | expr ';'     {free_ast($2); 
                        $$ = $1}
         ;
      
vardecl  : identdecl '=' expr ';'      
               {free_ast($4);
               $2 = change_sym($2, TOK_VARDECL);
               $$ = adopt2($2, $1, $3);}
         ;

while    : whilehead statement  {$$ = adopt1($1, $2);}
         ;

whilehead: TOK_WHILE '(' expr ')'            {free_ast2($2, $4);
                                             $$ = adopt1($1, $3);}

ifelse   : TOK_IF '(' expr ')' statement %prec TOK_IF
               {free_ast2($2, $4); $$ = adopt2($1, $3, $5);}
         | TOK_IF '(' expr ')' statement TOK_ELSE statement
               {$1 = adopt1sym($1, $3, TOK_IFELSE); 
               $$ = adopt2($1, $5, $7);
               free_ast2($2, $4); free_ast($6);}
         
return   : TOK_RETURN expr ';'   {$$ = adopt1($1, $2);}
         | TOK_RETURN ';'        {$$ = adopt1sym(
                                 $1, $2, TOK_RETURNVOID);}
         ;

expr     : binop                          {$$ = $1;}
         | unop                           {$$ = $1;}
         | allocator %prec TOK_NEW        {$$ = $1;}
         | call                           {$$ = $1;}
         | '(' expr ')' %prec TOK_PARENS  {free_ast2($1, $3);
                                          $$ = $1;}
         | variable                       {$$ = $1;}
         | constant                       {$$ = $1;}
         ;

binop    : expr TOK_IFELSE expr  {$$ = adopt2($2, $1, $3);}    
         | expr TOK_EQ expr      {$$ = adopt2($2, $1, $3);}      
         | expr TOK_NE expr      {$$ = adopt2($2, $1, $3);}    
         | expr TOK_LT expr      {$$ = adopt2($2, $1, $3);}
         | expr TOK_LE expr      {$$ = adopt2($2, $1, $3);}
         | expr TOK_GT expr      {$$ = adopt2($2, $1, $3);}
         | expr TOK_GE expr      {$$ = adopt2($2, $1, $3);}
         | expr '+' expr         {$$ = adopt2($2, $1, $3);}
         | expr '-' expr         {$$ = adopt2($2, $1, $3);}
         | expr '*' expr         {$$ = adopt2($2, $1, $3);}
         | expr '/' expr         {$$ = adopt2($2, $1, $3);}
         | expr '%' expr         {$$ = adopt2($2, $1, $3);}
         | expr '=' expr         {$$ = adopt2($2, $1, $3);}


unop     : '!' expr                 {$$ = adopt1($1, $2);}
         | '-' expr %prec TOK_NEG   
               {$$ = adopt1sym($1, $2, TOK_NEG);}
         | '+' expr %prec TOK_POS         
               {$$ = adopt1sym($1, $2, TOK_POS);}
         | TOK_ORD expr         {$$ = adopt1($1, $2);}
         | TOK_CHR expr         {$$ = adopt1($1, $2);}

allocator: TOK_NEW TOK_IDENT '(' ')'         
               {$$ = adopt1sym($1, $2, TOK_TYPEID);}
         | TOK_NEW TOK_STRING '(' expr ')'   
               {free_ast2($3, $5); 
               $$ = adopt1sym($1, $4, TOK_NEWSTRING);}
         | TOK_NEW basetype '[' expr ']'     
               {free_ast2($3, $5); 
               $$ = adopt2sym($1, $2, $4, TOK_NEWARRAY);}
         ;

call     : TOK_IDENT '(' ')'        
               {$$ = adopt1sym($1, $2, TOK_CALL);}
         | TOK_IDENT callargs ')'   
               {free_ast($3);
               $$ = adopt1sym($2, $1, TOK_CALL);}
         ;

callargs : '(' expr           {$$ = adopt1($1, $2);}
         | callargs ',' expr  {free_ast($2);
                              $$ = adopt1($1, $3);}
         ;

variable : TOK_IDENT             {$$ = $1;} 
         | expr '.' expr          
               {$3 = change_sym($3, TOK_FIELD);
               $$ = adopt2($2, $1, $3);}
         | expr '[' expr ']'    
               {$2 = change_sym($2, TOK_INDEX);
               $$ = adopt2($2, $1, $3);}
         ;   

constant : TOK_INTCON      {$$ = $1;}
         | TOK_CHARCON     {$$ = $1;}
         | TOK_STRINGCON   {$$ = $1;}
         | TOK_FALSE       {$$ = $1;}
         | TOK_TRUE        {$$ = $1;}
         | TOK_NULL        {$$ = $1;}
         ;
%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

