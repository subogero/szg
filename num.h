/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
#ifndef __tNum
#define __tNum

#define T_NATURAL 0
#define T_SIGNED  1
#define T_FLOAT   2

#define BASE_NA 0

struct num {
	union {
		unsigned int n;
		int          s;
		float        f;
	} val;
	char type;
	char base;
};

void num_2type(struct num *this, char type);
void num_matchtype(struct num *this, struct num *that);

struct num num_infix(struct num src1, char op, struct num src2);
struct num num_prefix(char op, struct num src);

int num_parse(struct num *this, char *yytext);
const char *num_print(struct num *this, int num, char base);
#define num_display(this,num) num_print(this,num,BASE_NA)

int num_base(struct num *this, char base);

#endif
