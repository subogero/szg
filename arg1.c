/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
*******************************************************************************/
#include "arg1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Perform Usage/Version display if 1st arg is -h/-V */
void arg1_eval(const char* arg1) {
	if (strstr(arg1, "-h") || strstr(arg1, "--help")) {
		usage();
		exit(0);
	}
	if (strstr(arg1, "-V") || strstr(arg1, "--version")) {
		version();
		exit(0);
	}
}

/* Display usage when called with -h */
void usage(void) {
	int i;
	static const char* lines[] = {
		#include "usage.h"
	};
	for (i = 0; i < sizeof lines / sizeof(char*); i++)
		printf(lines[i]);
}


/* Display version info when called with -v */
void version(void) {
	int i;
	static const char* lines[] = {
		#include "version.h"
	};
	for (i = 0; i < sizeof lines / sizeof(char*); i++)
		printf(lines[i]);
}
