/**
 * @file ab_hset.h
 * @author bfzhang
 * @date 2008-09-04
 * @desctription: ab hash set
 * @note: reconstruct in 2008-09-04
 */  
#include <stdio.h>
#include <stdlib.h>

#ifndef ABHSET_H
#define ABHSET_H

typedef struct _ab_hset_node
{
	char *key;
	struct _ab_hset_node * next;
}ab_hset_node;
typedef struct _ab_hset
{
    ab_hset_node * bucket;
	unsigned int size;
	unsigned int len;
}ab_hset;

ab_hset *ab_hset_init(const unsigned int size);

void ab_hset_destroy(ab_hset *pset);

void ab_hset_insert(ab_hset * pset, const char *key);

unsigned char ab_hset_haskey(const ab_hset *pset, const char *key);

void ab_hset_delete(ab_hset *pset, const char *key);

void ab_hset_output(const ab_hset *pset, FILE *fp);
#endif
