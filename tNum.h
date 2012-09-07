/*****************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
*****************************************************************************/
#ifndef __tNum
#define __tNum

#define T_NATURAL 0
#define T_SIGNED  1
#define T_FLOAT   2

#define BASE_NA 0

struct tNum {
  union {
    unsigned int n;
    int          s;
    float        f;
  } val;
  char type;
  char base;
};

void tNum2type       (struct tNum *this, char type);
void tNumMatchType   (struct tNum *this, struct tNum *that);
struct tNum tNumOpIn (struct tNum src1, char op, struct tNum src2);
struct tNum tNumOpPre(char op, struct tNum src);
int  tNumParse       (struct tNum *this, char *yytext);
void tNumPrint       (struct tNum *this, int num, int prompt, char base);
#define tNumDisplay(this,num,prompt) tNumPrint(this,num,prompt,BASE_NA)

#endif
