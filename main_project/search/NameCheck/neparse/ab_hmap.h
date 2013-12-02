/**
 * @file ab_hmap.h
 * @author bfzhang
 * @date 2007-02-12
 * @description: ab hash map
 * @note: reconstruct in 2008-09-03
 */
#include <stdio.h>
#ifndef AB_HMAP_H
#define AB_HMAP_H

typedef struct _ab_hmap_node
{
	char *key;
	char *value;
	struct _ab_hmap_node * next;
}ab_hmap_node;
typedef struct _ab_hmap
{
    ab_hmap_node * bucket;
    unsigned int size;
    unsigned int len; 
}ab_hmap;

ab_hmap * ab_hmap_init(const unsigned int size);

void ab_hmap_destroy(ab_hmap * pmap);

void ab_hmap_insert(ab_hmap * pmap, const char *key, const char *value);

unsigned char ab_hmap_haskey(const ab_hmap *pmap, const char *key);

void ab_hmap_delete(ab_hmap *pmap, const char *key);

const char *ab_hmap_get(const ab_hmap *pmap, const char *key);

void ab_hmap_output(const ab_hmap *pmap, FILE *fp);
#endif
