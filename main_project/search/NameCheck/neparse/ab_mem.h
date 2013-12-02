/*****************************************************
* Filename:ab_mem.h
* 
* Date: 2007-06-28
* Description:wrappers for dynamic storage allocation functions
******************************************************/
#ifndef _AB_MEM_H
#define _AB_MEM_H

#include <stdlib.h>
#include <sys/types.h>

void * ab_malloc(size_t size);

void * ab_realloc(void *ptr,size_t size);

void * ab_calloc(size_t nmemb,size_t size);

void ab_free(void *ptr);

#endif
