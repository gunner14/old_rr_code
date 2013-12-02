/**
 *@file ab_binaryheap.h
 *@author bfzhang
 *@brief binarayheap implementation $Id: ab_binaryheap.h,v 1.7 2007/07/17 08:58:52 bfzhang Exp $
 */

#ifndef _AB_BINARYHEAP_H
#define _AB_BINARYHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@brief the struct defination of binarayheap
 */
typedef struct _ab_bheap_s
{
    ///element array
    void **elem_array;
    
    ///element count
    unsigned int elem_count;

    ///is max heap is_max_heap = 1;else is min heap is_max_heap = 0;
    unsigned char is_max_heap;
    
    ///compare callback function    
    int (*compare)(const void *element1,const void *element2);    
    
    ///destroy the element callback function 
    void (*destroy)(void *element);

    ///element pool size(private field)
    unsigned int elem_pool_size;
    
}ab_bheap_s;

/**
 *@brief create the binaray heap
 *@param size the init pool size of binaray heap
 *@param is_max_heap is max heap 1;else is min heap 0;
 *@param compare compare call back function function
 *@param destroy destroy the element(call back functioni)
 * destroy should be NULL if element doesn't include dynamic memory or 
 * you don't need ab_bheap_s manage memory for you!
 *@retval fail NULL;
 */
ab_bheap_s *ab_bheap_new(const unsigned int size,
                         const unsigned char is_max_heap,
                         int (*compare)(const void *,const void *),
                         void (*destroy)(void *));

/**
 *@brief get the count of binaray heap
 *@retval the count of elements in binaray heap
 */
unsigned int ab_bheap_count(const ab_bheap_s *bheap);

/**
 *@brief judge the count of binary heap is zero
 *@reval count == 0 1;else 0
 */
unsigned char ab_bheap_isempty(const ab_bheap_s *bheap);

/**
 *@brief add element into binary heap
 *@retval 0 success;fail -1
 */
int ab_bheap_add(ab_bheap_s *bheap,const void *element);

/**
 *@brief get the top fo binaray heap
 */
void * ab_bheap_top(const ab_bheap_s *bheap);

/**
 *@brief the same syntax as ab_bheap_top
 */
void *ab_bheap_peek(const ab_bheap_s *bheap);

/**
 *@brief return the element on top of the heap and remove it
 * if(bheap->destroy != NULL) free the memory of ab_bheap(top);
 */
void ab_bheap_remove(ab_bheap_s *bheap);

/**
 *@brief the same syntax as ab_bheap_remove
 */
void ab_bheap_pop(ab_bheap_s *bheap);

/**
 *@brief free the memory of binaray heap
 */
void ab_bheap_clear(ab_bheap_s *bheap);

#endif
