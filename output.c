/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/
// Dependencies
#include <stdlib.h>
#include "output.h"

// Global definitions
struct tNum output = { { 0 }, T_SIGNED, 10 };

// Local declarations
// - Linked list element with data and link
// - Pointer to top of stack
// - Default data store used if dynamic memory allocation fails
struct tStack {
  struct tNum Data;
  struct tStack* pLnk;
};

static char dirty = 0; // output != top-of-stack
static struct tStack* pStack = NULL;
static struct tNum DefaultData = { { 0 }, T_SIGNED, 10 };

// Functions

// Get
void outputGet(void) {
  output = pStack == NULL
         ? DefaultData
         : pStack->Data;
  dirty = 0;
}

// Pop
void outputPop(void) {
  // if output clean, pop twice
  if (!dirty) {
    dirty = 1;
    outputPop();
  }
  if (pStack == NULL) output = DefaultData;
  else {
    struct tStack* pTop = pStack;
    output = pTop->Data;
    pStack = pTop->pLnk;
    free(pTop);
  }
}

// Push
void outputPush(void) {
  struct tStack* pTop = malloc(sizeof(struct tStack));
  if (pTop != NULL) {
    pTop->Data = output;
    pTop->pLnk = pStack;
    pStack = pTop;
  }
  else {
    while (pStack) (void)outputPop();
    DefaultData = output;
  }
  dirty = 0;
}
