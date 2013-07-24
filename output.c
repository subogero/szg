/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
#include <stdlib.h>
#include "output.h"

/* Output of the last statement */
struct tNum output = { { 0 }, T_SIGNED, 10 };
/* Linked list element with data and link */
struct stack_t {
	struct tNum data;
	struct stack_t *link;
};
/* Pointer to top of stack */
static struct stack_t *stack_top = NULL;
/* Default data store used if dynamic memory allocation fails */
static struct tNum default_data = { { 0 }, T_SIGNED, 10 };

/* Get top of undo stack into output */
void output_get(void)
{
	output = stack_top == NULL ? default_data : stack_top->data;
}

/* Pop: discard top of undo stack, then get top of stack into output */
void output_pop(void)
{
	if (stack_top != NULL) {
		struct stack_t *old_top = stack_top;
		stack_top = old_top->link;
		free(old_top);
	}
	output_get();
}

/* Push output to top of undo stack */
void output_push(void)
{
	struct stack_t *new_top = malloc(sizeof(struct stack_t));
	if (new_top != NULL) {
		new_top->data = output;
		new_top->link = stack_top;
		stack_top = new_top;
	} else {
		while (stack_top)
			(void)output_pop();
		default_data = output;
	}
}

/* Show contents of entire undo stack */
void output_show(void)
{
	struct stack_t *this = stack_top;
	while (this != NULL) {
		tNumDisplay(&this->data, 1, 0);
		this = this->link;
	}
}
