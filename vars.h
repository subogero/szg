/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
/* Hash table for variable storage */
#ifndef __vars
#define __vars

#include "num.h"

char *vars_alloc(char *key);
void vars_set(char *key, struct num *num);
int  vars_get(char *key, struct num *num);
void vars_dbg(void);

#endif
