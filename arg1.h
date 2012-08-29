/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/
// Utility to display Usage/Version text and exit if arg1 is "-h"/"-V"
// Text shall be defined as string initializer-lists in usage.h and version.h
// Generate headers automatically from plain text files with a make rule:
// %.h: %.txt
//	sed -e 's/[ \t]*$$//g' -e 's/^/"/g' -e 's/$$/\\n",/g' <$< >$@

#ifndef __arg1
#define __arg1
void arg1Eval(const char* arg1);
void usage(void);
void version(void);
#endif
