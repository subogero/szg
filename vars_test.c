#include "num.h"
#include "vars.h"
#include <stdio.h>
#include <string.h>

struct num n = {
	.type = T_SIGNED, 
	.base = 10, 
};

int main(void)
{
	/* Fill up variables identified by strings */
	#define MAX 20
	char key[MAX] = { 0, };
	n.val.s = 24; vars_set("$x", &n);
	n.val.s = 25; vars_set("$y", &n);
	n.val.s = 26; vars_set("$z", &n);
	n.val.s = 1; vars_set("$a", &n);
	n.val.s = 2; vars_set("$b", &n);
	n.val.s = 3; vars_set("$c", &n);
	n.val.s = 1; vars_set("$foo", &n);
	n.val.s = 2; vars_set("$bar", &n);
	n.val.s = 3; vars_set("$baz", &n);
	n.val.s = 4; vars_set("$spam", &n);
	n.val.s = 5; vars_set("$eggs", &n);
	n.val.s = 42; vars_set("$lue", &n);
	n.val.s = 1948; vars_set("$izrael", &n);
	n.val.s = 1967; vars_set("$hatnapos", &n);
	n.val.s = 1446; vars_set("$exodus", &n);
	n.val.s = 225;  vars_set("$egypt", &n);
	n.val.s = 969;  vars_set("$oldest", &n);
	n.val.s = 911;  vars_set("$bestcar", &n);
	n.val.s = 1970; vars_set("$unix", &n);
	n.val.s = 1991; vars_set("$linux", &n);
	n.val.s = 2760; vars_set("$stelvio", &n);
	n.val.s = 2650; vars_set("$gavia", &n);
	n.val.s = 2291; vars_set("$foscagno", &n);
	n.val.s = 2150; vars_set("$fuorn", &n);
	n.val.s = 1852; vars_set("$mortirolo", &n);
	n.val.s = 900;  vars_set("$prato", &n);
	n.val.s = 1130; vars_set("$cepina", &n);
	n.val.s = 1800; vars_set("$livigno", &n);
	n.val.s = 3798; vars_set("$glockner", &n);
	n.val.s = 3905; vars_set("$ortler", &n);
	n.val.s = 3860; vars_set("$zebru", &n);
	n.val.s = 4167; vars_set("$breithorn", &n);
	n.val.s = 4563; vars_set("$zumstoa", &n);
	n.val.s = 4478; vars_set("$matterhorn", &n);
	n.val.s = 4505; vars_set("$weisshorn", &n);
	n.val.s = 4545; vars_set("$dom", &n);
	n.val.s = 4810; vars_set("$montblanc", &n);
	n.val.s = 8850; vars_set("$everest", &n);
	n.val.s = 8611; vars_set("$ktwo", &n);
	n.val.s = 8554; vars_set("$kancsendzonga", &n);
	n.val.s = 8540; vars_set("$lhotse", &n);
	vars_dbg();

	/* Let user query variables by strings (chop off LF) */
	while (fgets(key, MAX, stdin)) {
		char *lf = strchr(key, '\n');
		if (lf)
			*lf = 0;
		if (!vars_get(key, &n))
			num_display(&n, 1, 0);
	}
	return 0;
}
