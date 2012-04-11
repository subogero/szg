/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL license
* There is absolutely no warranty.
*****************************************************************************/
#include "arg1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Perform Usage/Version display if 1st arg is -h/-V
void arg1Eval(const char* arg1) {
  if (strstr(arg1, "-h")
   || strstr(arg1, "--help"))    { usage();   exit(0); }
  if (strstr(arg1, "-V")
   || strstr(arg1, "--version")) { version(); exit(0); }
}

// Display usage when called with -h
void usage(void) {
  int i;
  static const char* Usage[] = {
    #include "usage.h"
  };
  for (i = 0; i < sizeof(Usage) / sizeof(char*); i++) printf(Usage[i]);
}


// Display version info when called with -v
void version(void) {
  int i;
  static const char* Version[] = {
    #include "version.h"
  };
  for (i = 0; i < sizeof(Version) / sizeof(char*); i++) printf(Version[i]);
}
