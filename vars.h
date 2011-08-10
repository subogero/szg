/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/
#ifndef __vars
#define __vars

#include "tNum.h"

char *vars_alloc(char *key);
void vars_set(char *key, struct tNum *num);
int  vars_get(char *key, struct tNum *num);
void vars_dbg(void);

#endif
