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
  // Fill up variables identified by strings
  #define MAX 20
  char key[MAX] = { 0, };
  num.val.s = 24; vars_set("$x", &num);
  num.val.s = 25; vars_set("$y", &num);
  num.val.s = 26; vars_set("$z", &num);
  num.val.s = 1; vars_set("$a", &num);
  num.val.s = 2; vars_set("$b", &num);
  num.val.s = 3; vars_set("$c", &num);
  num.val.s = 1; vars_set("$foo", &num);
  num.val.s = 2; vars_set("$bar", &num);
  num.val.s = 3; vars_set("$baz", &num);
  num.val.s = 4; vars_set("$spam", &num);
  num.val.s = 5; vars_set("$eggs", &num);
  num.val.s = 42; vars_set("$lue", &num);
  num.val.s = 1948; vars_set("$izrael", &num);
  num.val.s = 1967; vars_set("$hatnapos", &num);
  num.val.s = 1446; vars_set("$exodus", &num);
  num.val.s = 225;  vars_set("$egypt", &num);
  num.val.s = 969;  vars_set("$oldest", &num);
  num.val.s = 911;  vars_set("$bestcar", &num);
  num.val.s = 1970; vars_set("$unix", &num);
  num.val.s = 1991; vars_set("$linux", &num);
  num.val.s = 2760; vars_set("$stelvio", &num);
  num.val.s = 2650; vars_set("$gavia", &num);
  num.val.s = 2291; vars_set("$foscagno", &num);
  num.val.s = 2150; vars_set("$fuorn", &num);
  num.val.s = 1852; vars_set("$mortirolo", &num);
  num.val.s = 900;  vars_set("$prato", &num);
  num.val.s = 1130; vars_set("$cepina", &num);
  num.val.s = 1800; vars_set("$livigno", &num);
  num.val.s = 3798; vars_set("$glockner", &num);
  num.val.s = 3905; vars_set("$ortler", &num);
  num.val.s = 3860; vars_set("$zebru", &num);
  num.val.s = 4167; vars_set("$breithorn", &num);
  num.val.s = 4563; vars_set("$zumstoa", &num);
  num.val.s = 4478; vars_set("$matterhorn", &num);
  num.val.s = 4505; vars_set("$weisshorn", &num);
  num.val.s = 4545; vars_set("$dom", &num);
  num.val.s = 4810; vars_set("$montblanc", &num);
  num.val.s = 8850; vars_set("$everest", &num);
  num.val.s = 8611; vars_set("$ktwo", &num);
  num.val.s = 8554; vars_set("$kancsendzonga", &num);
  num.val.s = 8540; vars_set("$lhotse", &num);
  vars_dbg();

  // Let user query variables by strings (chop off LF)
  while (fgets(key, MAX, stdin)) {
    char *lf = strchr(key, '\n');
    if (lf) *lf = 0;
    if (!vars_get(key, &num)) tNumDisplay(&num, 1, 0);
  }
  return 0;
}

