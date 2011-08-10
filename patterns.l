/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/

%{
#include <stdlib.h>
#include "output.h"
#include "grammar.h"
#include "szg.h"

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif
static void dbug(char* token);
%}

%%
#.*$        ;
[ \t]       ;
[a-z]       { print_on(); yylval.u8 = *yytext - 'a';                       dbug("COMMAND"); return COMMAND; }
[G-Z]       { print_on(); yylval.u8 = *yytext - 'G';                       dbug("LETTER");  return LETTER; }
[-+|]       { print_on(); yylval.u8 = *yytext;                             dbug("OPADD");   return OPADD; }
[*/%&]      { print_on(); yylval.u8 = *yytext;                             dbug("OPMUL");   return OPMUL; }
~           { print_on(); yylval.u8 = *yytext;                             dbug("OPPRE");   return OPPRE; }
@[a-z]      { print_on(); yylval.u8 = yytext[1];                           dbug("OPPRE");   return OPPRE; }
\$[A-Z]+    { print_on(); yylval.id = vars_alloc(yytext);                  dbug("VAR");     return VAR; }
[0-9A-F.,]+ { print_on(); tNumParse(&output, yytext); yylval.Num = output; dbug("NUMBER");  return NUMBER; }
.           { print_on();                                                  dbug("u8");      return *yytext; }
\n          {                                                              dbug("LF");      return *yytext; }

%%
int yywrap(void) {
  return 1;
}

static void dbug(char* token) {
#ifdef DEBUG
  printf("lex token %s : txt %s, u8 %d, num %X\n", token, yytext, yylval.u8, yylval.Num.val.n);
#endif
}
