/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
*****************************************************************************/
%{
#include "num.h"
%}

%union {
	char u8;
	char *id;
	struct tNum Num;
}

%{
#include "szg.h"
#include "output.h"
#include <stdio.h>

#ifdef VARS
#include "vars.h"
#else
#define vars_set(key,num) fprintf(stderr, "vars support not enabled\n")
#define vars_get(key,num) vars_set(key,num)
#endif

//#define DEBUG
static void dbg(char* term);
%}


%start list

%token <u8>  COMMAND OPADD OPMUL OPPOW OPPRE
%token <id>  VAR
%token <Num> NUMBER

%type <Num> expr

%left OPADD        // additive operators       +-|
%left OPMUL        // multiplicative operators */%&
%left OPPOW        // power operator           ^
%nonassoc OPPRE    // prefix operators         ~

%%
list : // empty
     | list stm '\n'         { dbg("ls");   print(); }
     | list cmd '\n'         { dbg("lc");   print(); }
     | list cmd stm '\n'     { dbg("lcs");  print(); }
     | list stm cmd '\n'     { dbg("lsc");  print(); }
     | list cmd stm cmd '\n' { dbg("lcsc"); print(); }
     | list error '\n'       { dbg("lerr"); yyerrok; print(); }
     ;
cmd  : COMMAND               { dbg("cmd");  commands[$1](); }
     ;
stm  : expr                  { dbg("stm");  output = $1; output_push(); }
     | VAR '=' expr          { dbg("var");  vars_set($1, &$3); }
     ;
expr :                       { dbg("non");  output_get(); $$ = output; }
     | '_'                   { dbg("unl");  output_get(); $$ = output; }
     | '(' expr ')'          { dbg("par");  $$ = $2; }
     | expr OPADD expr       { dbg("oad");  $$ = tNumOpIn ($1, $2, $3); }
     | expr OPMUL expr       { dbg("omu");  $$ = tNumOpIn ($1, $2, $3); }
     | expr OPPOW expr       { dbg("opo");  $$ = tNumOpIn ($1, $2, $3); }
     | OPPRE expr            { dbg("opr");  $$ = tNumOpPre($1, $2); }
     | VAR                   { dbg("var");  if (vars_get($1,&$$)) $$=output; }
     | NUMBER                { dbg("num");  $$ = $1; }
     ;
%%

static void dbg(char* term) {
#ifdef DEBUG
	printf("yacc %s\n", term);
#endif
}
