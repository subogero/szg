#include "num.h"
int main(void) {
  struct tNum n1, n2;

  n1.val.n = 2700000000U;
  n1.type = T_NATURAL;
  n1.base = 16;

  n2.val.f = 30.0;
  n2.type = T_FLOAT;
  n2.base = 10;

  tNumDisplay(&n1, 1, 0);
  tNumDisplay(&n2, 1, 0);

  n1 = tNumOpIn(n1, '/', n2);
  tNumDisplay(&n1, 1, 0);

  tNumPrint(&n1, 1, 0, 2);
  tNumPrint(&n2, 1, 0, 2);

  n1 = tNumOpIn(n1, '|', n2);
  tNumPrint(&n1, 1, 0, 2);
}
