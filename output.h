/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
/* Undo stack for the outputs of all previous statements  */
#include "num.h"

/* Output of the last statement stored here */
extern struct tNum output;

/* Read from, pop, push to the top of, and examine the stack*/
void output_get(void);
void output_pop(void);
void output_push(void);
void output_show(void);
