/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
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

static struct tStack* pStack = NULL;
static struct tNum DefaultData = { { 0 }, T_SIGNED, 10 };

// Functions

// Get
void outputGet(void) {
  output = pStack == NULL
         ? DefaultData
         : pStack->Data;
}

// Pop
void outputPop(void) {
  if (pStack != NULL) {
    struct tStack* pTop = pStack;
    pStack = pTop->pLnk;
    free(pTop);
  }
  output = pStack != NULL ? pStack->Data : DefaultData;
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
}

void outputShow(void) {
  struct tStack* this = pStack;
  while (this != NULL) {
    tNumDisplay(&this->Data, 1, 0);
    this = this->pLnk;
  }
}
