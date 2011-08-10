#include "tNum.h"
#include "vars.h"
#include <stdio.h>
#include <string.h>

struct tNum num = {
  .type = T_SIGNED, 
  .base = 10, 
};

int main(void)
{
  // Fill up 9 variables identified by 9 strings
  #define MAX 20
  char key[MAX] = { 0, };
  num.val.s = 24; vars_set("$X", &num);
  num.val.s = 25; vars_set("$Y", &num);
  num.val.s = 26; vars_set("$Z", &num);
  num.val.s = 1; vars_set("$A", &num);
  num.val.s = 2; vars_set("$B", &num);
  num.val.s = 3; vars_set("$C", &num);
  num.val.s = 1; vars_set("$FOO", &num);
  num.val.s = 2; vars_set("$BAR", &num);
  num.val.s = 3; vars_set("$BAZ", &num);
  num.val.s = 4; vars_set("$SPAM", &num);
  num.val.s = 5; vars_set("$EGGS", &num);
  num.val.s = 42; vars_set("$LUE", &num);
  num.val.s = 1948; vars_set("$IZRAEL", &num);
  num.val.s = 1967; vars_set("$HATNAPOS", &num);
  num.val.s = 1446; vars_set("$EXODUS", &num);
  num.val.s = 225; vars_set("$EGYPT", &num);
  num.val.s = 969; vars_set("$OLDEST", &num);
  num.val.s = 911; vars_set("$BESTCAR", &num);
  num.val.s = 1970; vars_set("$UNIX", &num);
  num.val.s = 1991; vars_set("$LINUX", &num);
  num.val.s = 2760; vars_set("$STELVIO", &num);
  num.val.s = 2650; vars_set("$GAVIA", &num);
  num.val.s = 2291; vars_set("$FOSCAGNO", &num);
  num.val.s = 2150; vars_set("$FUORN", &num);
  num.val.s = 1852; vars_set("$MORTIROLO", &num);
  num.val.s = 900; vars_set("$PRATO", &num);
  num.val.s = 1130; vars_set("$CEPINA", &num);
  num.val.s = 1800; vars_set("$LIVIGNO", &num);
  num.val.s = 3798; vars_set("$GLOCKNER", &num);
  num.val.s = 3905; vars_set("$ORTLER", &num);
  num.val.s = 3860; vars_set("$ZEBRU", &num);
  num.val.s = 4167; vars_set("$BREITHORN", &num);
  num.val.s = 4563; vars_set("$ZUMSTOA", &num);
  num.val.s = 4478; vars_set("$MATTERHORN", &num);
  num.val.s = 4505; vars_set("$WEISSHORN", &num);
  num.val.s = 4545; vars_set("$DOM", &num);
  num.val.s = 4810; vars_set("$MONTBLANC", &num);
  num.val.s = 8850; vars_set("$EVEREST", &num);
  num.val.s = 8611; vars_set("$KTWO", &num);
  num.val.s = 8554; vars_set("$KANCSENDZONGA", &num);
  num.val.s = 8540; vars_set("$LHOTSE", &num);
  vars_dbg();

  // Let user query variables by strings (chop off LF)
  while (fgets(key, MAX, stdin)) {
    char *lf = strchr(key, '\n');
    if (lf) *lf = 0;
    if (!vars_get(key, &num)) tNumDisplay(&num, 1, 0);
  }
  return 0;
}

