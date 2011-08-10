/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/

#ifndef __szg
#define __szg

// Command callbacks
typedef void (*t_callback)(void);
extern const t_callback commands[26];

// Print result
void print(void);
void print_on(void);
int yyerror(const char *s);

// Number system and Variable storage
#include "tNum.h"
extern struct tNum regs[20];

#endif
