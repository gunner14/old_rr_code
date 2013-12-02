/**
 * @file utility.h
 * @author bfzhang
 * @brief some utility function used in this module
 */
#ifndef UTILITY_H_20120423
#define UTILITY_H_20120423

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "HASHMAP.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif 

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define GB_LEN 2
#define ASCII_LEN 1

#define ab_segmenter_seg ABU_WORDS_SEG_SL_getSegResultFromBuff 
#define ab_segmenter_free ABU_WORDS_SEG_SL_destroySegResult

void * ab_malloc(size_t size);
void * ab_realloc(void *ptr,size_t size);
void * ab_calloc(size_t nmemb,size_t size);
FILE * ab_fopen(const char *filename,const char *mode);
void ab_fclose(FILE *fp);

void getMapping(char *szSource, uint32_t *mapping);

unsigned char isGBKCharacter(const char *str);

unsigned char isdgc(const char *str);
unsigned char isfulldgc(const char *str);
unsigned char isaliasdgc(const char *str);

#endif
