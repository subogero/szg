/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
*******.**********************************************************************/
#ifndef __szg
#define __szg

/* Command callbacks */
typedef void (*t_callback)(void);
extern const t_callback commands[26];

/* Print result */
void print(void);
void print_on(void);
int yyerror(const char *s);

#endif
