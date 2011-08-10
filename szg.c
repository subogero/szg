/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the GNU GPL v3 license
* There is absolutely no warranty.
*****************************************************************************/

// Dependencies
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "arg1/arg1.h"
#include "szg.h"
#include "output.h"
#include "grammar.h"
#include "vars.h"
extern void yyparse(void); // interpreter main loop

// Global definitions
FILE* yyin;

struct tNum regs[20];

static void __na(void);
static void bin (void);
static void hex (void);
static void dec (void);
static void oct (void);
static void natn(void);
static void sign(void);
static void fltn(void);
static void prmt(void);
static void quit(void);
static void vars(void);
static void undo(void);

const t_callback commands[26] = {
  __na ,  // a
  bin  ,  // b
  __na ,  // c
  dec  ,  // d
  __na ,  // e
  fltn ,  // f
  __na ,  // g
  usage,  // h
  __na ,  // i
  __na ,  // j
  __na ,  // k
  __na ,  // l
  __na ,  // m
  natn ,  // n
  oct  ,  // o
  prmt ,  // p
  quit ,  // q
  __na ,  // r
  sign ,  // s
  __na ,  // t
  undo ,  // u
  vars ,  // v
  __na ,  // w
  hex  ,  // x
  __na ,  // y
  __na ,  // z
};

// Local declarations and definitions
static int  print_req = 0;
static int  prompt = 0;

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
  char* filename = NULL;
  char* command  = NULL;
  int tmpfile = 0;

  // Parse arguments
  if (argc >= 2) {
    arg1Eval(argv[1]); // catch and print help/version and exit
    if (*(argv[1]) == '-') {
      if (argc >= 3) filename = argv[2];
                     command  = argv[1];
    }
    else             filename = argv[1];
  }

  // Commands found
  if (command != NULL) {
    if (strchr(command, 'b')) output.base =  2;
    if (strchr(command, 'o')) output.base =  8;
    if (strchr(command, 'd')) output.base = 10;
    if (strchr(command, 'x')) output.base = 16;
    if (strchr(command, 'n')) natn();
    if (strchr(command, 's')) sign();
    if (strchr(command, 'f')) fltn();
    if (strchr(command, 'p')) prmt();

    // -e EXPR on command line, copy to tmp file
    if (strchr(command, 'e')) {
      int i;
      filename = tmpnam(NULL);
      yyin = fopen(filename, "w");
      if (!yyin) {
        yyerror("Cannot create tmp file");
        exit(1);
      }
      for (i = 2; i < argc; ++i) fputs(argv[i], yyin);
      fputc('\n', yyin);
      fclose(yyin);
      tmpfile = 1;
    }
  }

  // Open input stream
  // Prompt: tty - yes, script/pipe - no
  if (!filename) {
    yyin = stdin;
    if (isatty(fileno(yyin))) prmt();;
  }
  else {
    yyin = fopen(filename, "r");
    if (!yyin) {
      yyerror("Cannot open input file");
      exit(1);
    }
  }

  // Run the actual calculator
  tNumDisplay(&output, 0, prompt);
  yyparse();

  // EOF, Cleanup
  fclose(yyin);
  if (tmpfile) remove(filename);
  quit();
}


////////////////////////////////////////////////////////////////////////////////
// Print a number in the actual base-system
////////////////////////////////////////////////////////////////////////////////
void print(void) {
  tNumDisplay(&output, print_req, prompt);
  print_req = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Request printing
////////////////////////////////////////////////////////////////////////////////
void print_on(void) {
  print_req = 1;
}

////////////////////////////////////////////////////////////////////////////////
// yacc error callback to indicate syntax error
////////////////////////////////////////////////////////////////////////////////
int yyerror(const char *s) {
  fprintf(stderr, s);
  fprintf(stderr, "\n");
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// command callbacks
////////////////////////////////////////////////////////////////////////////////
static void __na(void) { yyerror("unknown command");                  }
static void bin (void) { output.base =  2;              outputPush(); }
static void oct (void) { output.base =  8;              outputPush(); }
static void dec (void) { output.base = 10;              outputPush(); }
static void hex (void) { output.base = 16;              outputPush(); }
static void natn(void) { tNum2type(&output, T_NATURAL); outputPush(); }
static void sign(void) { tNum2type(&output, T_SIGNED);  outputPush(); }
static void fltn(void) { tNum2type(&output, T_FLOAT);   outputPush(); }
static void prmt(void) { prompt = !prompt;                            }
static void quit(void) { printf("\r"); exit(0);                       }
static void vars(void) { vars_dbg();                                  }
static void undo(void) {                                outputPop();  }
