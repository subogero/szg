/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
#include "num.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static void comma_strip(char *yytext);
static void comma_group(char *binary, unsigned int val, int ieee754);
static struct num num_power(struct num src1, struct num src2);

/* Value in actual type (can be lvalue) */
#define VALUE(this) ( (this)->type == T_NATURAL ? (this)->val.n \
                    : (this)->type == T_SIGNED  ? (this)->val.s \
                    :                             (this)->val.f )

/* Type Conversion */
void num_2type(struct num *this, char type) {
	switch (type) {
	case T_FLOAT:
		this->val.f = (float)VALUE(this);
		this->type = T_FLOAT;
		this->base = 10;
		break;
	case T_SIGNED:
	case T_NATURAL:
		if (this->type == T_FLOAT) this->val.s = (int)this->val.f;
		this->type = type;
		break;
	}
}

void num_matchtype(struct num *this, struct num *that) {
	if (this->type < that->type)
		num_2type(this, that->type);
	else if (that->type < this->type)
		num_2type(that, this->type);
}

/* Infix operators */
struct num num_infix(struct num src1, char op, struct num src2) {
	/* Division by zero check */
	if ((op == '/' || op == '%') && !src2.val.n) {
		fprintf(stderr, "division by zero\n");
		return src1;
	}
	/* Float mod check */
	if (op == '%' && (src1.type == T_FLOAT || src2.type == T_FLOAT)) {
		fprintf(stderr, "float modulo\n");
		return src1;
	}
	/* Automatic type match: *|^ never, +- if any float, /%* always */
	if (op != '&' && op != '|' && op != '^' &&
	    (src1.type == T_FLOAT || src2.type == T_FLOAT ||
	     op != '+' && op != '-'))
		num_matchtype(&src1, &src2);
	/* Do my teng... */
	switch (op) {
	case '&':
		src1.val.n &= src2.val.n;
		return src1;
	case '|':
		src1.val.n |= src2.val.n;
		return src1;
	case '+':
		if (src1.type == T_FLOAT)
			src1.val.f += src2.val.f;
		else
			src1.val.s += src2.val.s;
		return src1;
	case '-':
		if (src1.type == T_FLOAT)
			src1.val.f -= src2.val.f;
		else
			src1.val.s -= src2.val.s;
		return src1;
	case '*':
		if (src1.type == T_FLOAT)
			src1.val.f *= src2.val.f;
		else if (src1.type == T_SIGNED)
			src1.val.s *= src2.val.s;
		else
			src1.val.n *= src2.val.n;
		return src1;
	case '/':
		if (src1.type == T_FLOAT)
			src1.val.f /= src2.val.f;
		else if (src1.type == T_SIGNED)
			src1.val.s /= src2.val.s;
		else
			src1.val.n /= src2.val.n;
		return src1;
	case '%':
		if (src1.type == T_SIGNED)
			src1.val.s %= src2.val.s;
		else
			src1.val.n %= src2.val.n;
		return src1;
	case '^':
		return num_power(src1, src2);
	default:
		return src1;
	}
}

/* Prefix operators */
struct num num_prefix(char op, struct num src) {
	switch (op) {
	case '~':
		if (src.type == T_FLOAT)
			src.val.f = -src.val.f;
		else
			src.val.s = -src.val.s;
		break;
	default:
		num_2type(&src, T_FLOAT);
		if (src.val.f < 0.0 && (op == 'l' || op == 'r')) {
			fprintf(stderr, "n.a. for negative\n");
			return src;
		}
		if (src.val.f == 0.0 && op == 'l') {
			fprintf(stderr, "n.a. for zero\n");
			return src;
		}
		src.val.f = op == 's' ? sin (src.val.f)
		          : op == 'c' ? cos (src.val.f)
		          : op == 'a' ? atan(src.val.f)
		          : op == 'l' ? log (src.val.f)
		          : op == 'e' ? exp (src.val.f)
		          : op == 'r' ? sqrt(src.val.f)
		          : (fprintf(stderr, "unknown function\n"), src.val.f);
		break;
	}
	return src;
}

/* Parse */
int num_parse(struct num *this, char *yytext) {
	int success = 0;
	comma_strip(yytext);
	/* attempt actual type and base */
	if (this->type != T_FLOAT) {
		char *p_end;
		long long num = strtoll(yytext, &p_end, this->base);
		if (p_end - yytext == strlen(yytext)) {
			this->val.s = num;
			success = 1;
		}
	} else {
		char *p_end;
		float num = strtod(yytext, &p_end);
		if (p_end - yytext == strlen(yytext)) {
			this->val.f = num;
			success = 1;
		}
	}
	/* attampt float if failed */
	if (!success && this->type != T_FLOAT && strchr(yytext, '.')) {
		num_2type(this, T_FLOAT);
		success = num_parse(this, yytext);
	}
	if (!success)
		fprintf(stderr, "unable to parse number\n");
	return success;
}

/* Print */
void num_print(struct num *this, int num, int prompt, char base) {
	/* Print formats with optional prompt */
	char type;
	int index = 0;
	static const int bases[] = {
		2,        8,        10,       16
	};
	static const char *formats[] = {
		"%B\n",   "%o\n",   "%u\n",   "%x\n",   /* natural */
		"%B\n",   "%o\n",   "%d\n",   "%x\n",   /* signed */
		"%B\n",   "%o\n",   "%g\n",   "%x\n",   /* float */
	};
	static const char *prompts[] = {
		"B ",     "O ",     "D ",     "X ",
		"b ",     "o ",     "d ",     "x ",
		"f ",     "f ",     "f ",     "f ",
	};
	/* Find format */
	if (base == BASE_NA) base = this->base;
	while (bases[index] != base)
		index++;
	index += 4 * this->type;
	type = this->type;
	if (type == T_FLOAT && base != 10)
		type = T_SIGNED;
	/* Special code to print binary format */
	if (base == 2) {
		char *postfix = strchr(formats[index], '\n');
		char binary[40] = "";
		comma_group(binary, this->val.n, this->type == T_FLOAT);
		strcat(binary, postfix);
		printf("%s", binary);
	} else if (num) {
		switch (type) {
		case T_NATURAL:
			printf(formats[index], this->val.n);
			break;
		case T_SIGNED :
			printf(formats[index], this->val.s);
			break;
		case T_FLOAT  :
			printf(formats[index], this->val.f);
			break;
		}
	}
	if (prompt)
		printf(prompts[index]);
}

/* Set, or, print in, base */
int num_base(struct num *this, char base)
{
	if (base != 2 && base != 8 && base != 10 && base != 16)
		return 0;
	if (this->type == T_FLOAT) {
		num_print(this, 1, 0, base);
		return 0;
	} else {
		this->base = base;
		return 1;
	}
}

/* Comma functions: strip commas from input, comma group binary output */
static void comma_strip(char *yytext) 
{
	char *comma_group;
	while (comma_group = strchr(yytext, ','))
		memmove(comma_group, comma_group + 1, strlen(comma_group));
}

static void comma_group(char *binary, unsigned int val, int ieee754)
{
	int bit = 31;
	while (bit >= 0) {
		unsigned int mask = 1U << (unsigned int)bit;
		strcat(binary, val & mask ? "1" : "0");
		if (bit && (ieee754 ? bit == 31 || bit == 23 : bit % 8 == 0))
			strcat(binary, ",");
		bit--;
	}
}

/* Power operator */
static struct num num_power(struct num src1, struct num src2) {
	/* Convert to float if any operand float, or power negative */
	if (src2.type == T_SIGNED && src2.val.s < 0)
		num_2type(&src2, T_FLOAT);
	if (src1.type == T_FLOAT || src2.type == T_FLOAT) {
		num_matchtype(&src1, &src2);
		src1.val.f = pow(src1.val.f, src2.val.f);
		return src1;
	}
	/* Integer power */
	if (src2.val.n == 0) {
		src1.val.s = 1;
		return src1;
	}
	struct num tmp = src1;
	unsigned int i;
	for (i = 1; i < src2.val.n; ++i) {
		if (tmp.type == T_SIGNED)
			src1.val.s *= tmp.val.s;
		else
			src1.val.n *= tmp.val.n;
	}
	return src1;
}
