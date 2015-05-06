%{
// $Id: scanner.l,v 1.2 2015-04-26 22:18:08-07 - - $

#include "auxlib.h"
#include "lyutils.h"

#define YY_USER_ACTION  { scanner_useraction (); }

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER          [A-Za-z_]
DIGIT           [0-9]
MANTISSA        ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)
EXPONENT        ([Ee][+-]?{DIGIT}+)
NUMBER          ({MANTISSA}{EXPONENT}?)
NOTNUMBER       ({MANTISSA}[Ee][+-]?)
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
CHARCON         ('([^\\'\n]|\\[\\'"0nt])')
STRINGCON       (\"([^\\"\n]|\\[\\'"0nt])*\")

%%

"#".*           { scanner_include(); }
[ \t]+          { }
\n              { scanner_newline(); }

void           { return yylval_token (TOK_VOID); }
bool           { return yylval_token (TOK_BOOL); }
char           { return yylval_token (TOK_CHAR); }
int            { return yylval_token (TOK_INT); }
string         { return yylval_token (TOK_STRING); }

if             { return yylval_token (TOK_IF); }
else           { return yylval_token (TOK_ELSE); }
while          { return yylval_token (TOK_WHILE); }
return         { return yylval_token (TOK_RETURN); }
struct         { return yylval_token (TOK_STRUCT); }

false          { return yylval_token (TOK_FALSE); }
true           { return yylval_token (TOK_TRUE); }
null           { return yylval_token (TOK_NULL); }
new            { return yylval_token (TOK_NEW); }
ord            { return yylval_token (TOK_ORD); }
chr            { return yylval_token (TOK_CHR); }
"[]"           { return yylval_token (TOK_ARRAY); }

"=="            { return yylval_token (TOK_EQ); }
"!="            { return yylval_token (TOK_NE); }
"<"             { return yylval_token (TOK_LT); }
"<="            { return yylval_token (TOK_LE); }
">"             { return yylval_token (TOK_GT); }
">="            { return yylval_token (TOK_GE); }

"("             { return yylval_token ('('); }
")"             { return yylval_token (')'); }
"["             { return yylval_token ('['); }
"]"             { return yylval_token (']'); }
"{"             { return yylval_token ('{'); }
"}"             { return yylval_token ('}'); }
";"             { return yylval_token (';'); }
","             { return yylval_token (','); }
"."             { return yylval_token ('.'); }
"="             { return yylval_token ('='); }
"+"             { return yylval_token ('+'); }
"-"             { return yylval_token ('-'); }
"*"             { return yylval_token ('*'); }
"/"             { return yylval_token ('/'); }
"%"             { return yylval_token ('%'); }
"!"             { return yylval_token ('!'); }

{NUMBER}        { return yylval_token (TOK_INTCON); }
{IDENT}         { return yylval_token (TOK_IDENT); }
{CHARCON}       { return yylval_token (TOK_CHARCON); }
{STRINGCON}     { return yylval_token (TOK_STRINGCON); }

{NOTNUMBER}     { scanner_badtoken (yytext);
                  return yylval_token (TOK_INTCON); }

.               { scanner_badchar (*yytext); }

%%

RCSC("$Id: scanner.l,v 1.2 2015-04-26 22:18:08-07 - - $")

