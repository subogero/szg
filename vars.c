/******************************************************************************
* (c) SZABO Gergely, 2009
* Free software, distributed under the WTFPL license
* There is absolutely no warranty.
******************************************************************************/
#include "vars.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Hash-bucket element: pointer to next, key-val. */
struct list {
	struct list *next;
	char *key;
	struct num n;
	int defined;
};

/* hash function */
/* Hash table with 16 buckets */
#ifndef VARS_HASHSIZE
#define VARS_HASHSIZE 4
#endif
#define BUCKETS (1u << VARS_HASHSIZE)
static unsigned char hashfunc(char *key);
struct list *buckets[BUCKETS];
static struct list *vars_lookup(char *key, int append);

/* Allocate a variable */
char *vars_alloc(char *key)
{
	return vars_lookup(key, 1)->key;
}

/* Set or allocate variable */
void vars_set(char *key, struct num *n)
{
	struct list *this = vars_lookup(key, 1);
	if (this) {
		this->n = *n;
		this->defined = 1;
	} else {
		fprintf(stderr, "unable to allocate vaiable %s\n", key);
	}  
}

/* Get variable, n written if found in table, otherwise return 1 */
int  vars_get(char *key, struct num *n)
{
	struct list *this = vars_lookup(key, 0);
	if (this && this->defined) {
		*n = this->n;
		return 0;
	} else {
		fprintf(stderr, "variable %s undefined\n", key);
		return 1;
	}
}

/*
 * Calc hash, iterate list in hash bucket until
 * a) key found        : return key-val
 * b) end of list found: append and return key-val or return NULL
 */
static struct list *vars_lookup(char *key, int append)
{
	int hash = hashfunc(key);
	struct list *this = buckets[hash];
	struct list **parent = buckets + hash;
	while (this) {
		if (!strcmp(key, this->key)) {
			return this; /* Yesssssss!!! */
		} else {
			parent = &this->next;
			this = this->next;
		}
	}
	/*
	 * By now, this is NULL and parent points to the list tail's next pointer.
	 * Append new key-val to list if required, otherwise bye
	 */
	if (append) {
		*parent = malloc(sizeof(struct list));
		this = *parent;
		if (this) {
			this->next = NULL;
			this->defined = 0;
			/* Revert if unable to allocate memory for key */
			this->key = malloc(strlen(key) + 1);
			if (this->key) {
				strcpy(this->key, key);
			} else {
				free(this);
				this = NULL;
			}
		}
	}
	return this;
}

/* 4-bit hash functions for string */
static unsigned char hashfunc(char *key)
{
	#ifndef VARS_HASH
	#define VARS_HASH 2 /* Default hash function rotate */
	#endif
	/* 32-bit hash */
	unsigned long hash = 0;
	#if   VARS_HASH == 6 //"FNV1a"
	hash = 2166136261u;
	while (*key) {
		hash ^= *key++;
		hash *= 16777619u;
	}
	#elif VARS_HASH == 5 //"SAX"
	while (*key)
		hash ^= (hash << 5) + (hash >> 2) + *key++;
	#elif VARS_HASH == 4 //"BERNSTEINm"
	while (*key)
		hash = 33 * hash ^ *key++;
	#elif VARS_HASH == 3 //"BERNSTEIN"
	while (*key)
		hash = 33 * hash + *key++;
	#elif VARS_HASH == 2 //"ROT"
	while (*key)
		hash = (hash << 4) ^ (hash >> 28) ^ *key++;
	#elif VARS_HASH == 1 //"XOR"
	while (*key)
		hash ^= *key++;
	#else                //"ADD"
	while (*key)
		hash += *key++;
	#endif
	/* xor-folding for final hash-size */
	hash = ((hash >> VARS_HASHSIZE) ^ hash) & (BUCKETS - 1);
}

/* Print keys for each hash-bucket per line */
void vars_dbg(void)
{
	int i;
	for (i = 0; i < BUCKETS; ++i) {
		struct list *this = buckets[i];
		while (this) {
			if (this->defined) {
				printf("%s = ", this->key);
				num_print(&this->n, 1, 0, BASE_NA);
			}
			this = this->next;
		}
	}
}
