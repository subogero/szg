/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
*****************************************************************************/
%option always-interactive
%{
#include <stdlib.h>
#include "output.h"
#include "grammar.h"
#include "szg.h"
%}

%%
#.*$         ;
[ \t]        ;
[A-Z]        { print_on(); yylval.u8 = yytext[0] - 'A';                     return COMMAND;}
\\[a-z]      { print_on(); yylval.u8 = yytext[1] - 'a';                     return COMMAND;}
[g-z]        { print_on(); yylval.u8 = yytext[0] - 'a';                     return COMMAND; }
[-+|]        { print_on(); yylval.u8 = *yytext;                             return OPADD; }
[*/%&]       { print_on(); yylval.u8 = *yytext;                             return OPMUL; }
\^           { print_on(); yylval.u8 = *yytext;                             return OPPOW; }
~            { print_on(); yylval.u8 = *yytext;                             return OPPRE; }
@[a-z]       { print_on(); yylval.u8 = yytext[1];                           return OPPRE; }
\$[a-z0-9_]+ { print_on(); yylval.id = vars_alloc(yytext);                  return VAR; }
[0-9a-f.,]+  { print_on(); tNumParse(&output, yytext); yylval.Num = output; return NUMBER; }
.            { print_on();                                                  return *yytext;}
\n           {                                                              return *yytext;}
%%

int yywrap(void) {
  return 1;
}
