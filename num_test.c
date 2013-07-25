#include "num.h"
int main(void) {
  struct num n1, n2;

  n1.val.n = 2700000000U;
  n1.type = T_NATURAL;
  n1.base = 16;

  n2.val.f = 30.0;
  n2.type = T_FLOAT;
  n2.base = 10;

  num_display(&n1, 1, 0);
  num_display(&n2, 1, 0);

  n1 = num_infix(n1, '/', n2);
  num_display(&n1, 1, 0);

  num_print(&n1, 1, 0, 2);
  num_print(&n2, 1, 0, 2);

  n1 = num_infix(n1, '|', n2);
  num_print(&n1, 1, 0, 2);
}
