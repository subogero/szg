/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arg1.h"
#include "szg.h"
#include "output.h"
#include "grammar.h"
#include "patterns.h"
#include "vars.h"
extern void yyparse(void); /* interpreter main loop */

FILE *yyin;

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
	__na ,  /* A */
	bin  ,  /* B */
	__na ,  /* C */
	dec  ,  /* D */
	__na ,  /* E */
	fltn ,  /* F */
	__na ,  /* G */
	usage,  /* H */
	__na ,  /* I */
	__na ,  /* J */
	__na ,  /* K */
	__na ,  /* L */
	__na ,  /* M */
	natn ,  /* N */
	oct  ,  /* O */
	prmt ,  /* P */
	quit ,  /* Q */
	__na ,  /* R */
	sign ,  /* S */
	__na ,  /* T */
	undo ,  /* U */
	vars ,  /* V */
	__na ,  /* W */
	hex  ,  /* X */
	__na ,  /* Y */
	__na ,  /* Z */
};

static int print_req = 0;
static int prompt = 0;

/* main */
int main(int argc, char *argv[])
{
	char *filename = NULL;
	char *command  = NULL;
	char *expr     = NULL;

	/* Parse arguments */
	if (argc >= 2) {
		arg1_eval(argv[1]); /* catch and print help/version and exit */
		if (*(argv[1]) == '-') {
			command  = argv[1];
			if (argc >= 3)
				filename = argv[2];
		} else {
			filename = argv[1];
		}
	}

	/* Commands found */
	if (command != NULL) {
		int i;
		for (i = 'a'; i <= 'z'; ++i) {
			if (strchr(command, i) && commands[i - 'a'] != __na)
				(commands[i - 'a'])();
		}

		/* -e EXPR on command line, rest of args into buffer expr */
		if (strchr(command, 'e')) {
			int i;
			int len = 0;
			for (i = 2; i < argc; ++i)
				len += strlen(argv[i]) + 1;
			expr = malloc(len + 2);
			expr[0] = 0;
			for (i = 2; i < argc; ++i) {
				strcat(expr, argv[i]);
				strcat(expr, i < argc-1 ? " " : "\n");
			}
			strcat(expr, "\0\0");
			filename = NULL;
		}
	}

	/* Open input stream; Prompt: tty - yes, script/pipe - no */
	if (!filename) {
		yyin = stdin;
		if (!expr && isatty(fileno(yyin)))
			prmt();
	} else {
		yyin = fopen(filename, "r");
		if (!yyin) {
			yyerror("Cannot open input file");
			exit(1);
		}
	}

	/* Run the actual calculator */
	tNumDisplay(&output, 0, prompt);
	YY_BUFFER_STATE yybs;
	if (expr) {
		yybs = yy_scan_buffer(expr, strlen(expr) + 2);
		yy_switch_to_buffer(yybs);
	}
	yyparse();
	if (expr) {
		yy_delete_buffer(yybs);
		free(expr);
	}
	if (filename)
		fclose(yyin);
	quit();
}

/* Print a number in the actual base-system */
void print(void)
{
	tNumDisplay(&output, print_req, prompt);
	fflush(NULL);
	print_req = 0;
}

/* Request printing */
void print_on(void)
{
	print_req = 1;
}

/* yacc error callback to indicate syntax error */
int yyerror(const char *s)
{
	fprintf(stderr, s);
	fprintf(stderr, "\n");
	return 0;
}

/* command callbacks */
static void __na(void) { yyerror("unknown command");                   }
static void bin (void) { if (tNumBase(&output,  2))     output_push(); }
static void oct (void) { if (tNumBase(&output,  8))     output_push(); }
static void dec (void) { if (tNumBase(&output, 10))     output_push(); }
static void hex (void) { if (tNumBase(&output, 16))     output_push(); }
static void natn(void) { tNum2type(&output, T_NATURAL); output_push(); }
static void sign(void) { tNum2type(&output, T_SIGNED);  output_push(); }
static void fltn(void) { tNum2type(&output, T_FLOAT);   output_push(); }
static void prmt(void) { prompt = !prompt;                             }
static void quit(void) { printf("\r"); exit(0);                        }
static void vars(void) { vars_dbg();                                   }
static void undo(void) {                                output_pop();  }
