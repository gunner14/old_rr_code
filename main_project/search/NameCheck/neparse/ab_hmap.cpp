/**
 * @file ab_hmap.c
 * @author bfzhang
 * @date 2007-02-12
 * @description: ab hash map
 * @note: reconstruct in 200809-03
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ab_hmap.h"

#define FREE(p) if(p){ free(p); p = NULL;}
#define FALSE 0
#define TRUE 1 
static void *ab_calloc(size_t nmemb,size_t size);
static char *ab_strdup(const char *str);
static unsigned int ab_getsign(const char *key,const unsigned int keylen,const unsigned int size);

ab_hmap * ab_hmap_init(const unsigned int size)
{/*{{{*/
    //assert(size > 0);
    if(size <= 0)
        return NULL;
    ab_hmap *pmap = (ab_hmap *)ab_calloc(1, sizeof(ab_hmap));
    pmap->bucket = (ab_hmap_node *)ab_calloc(size, sizeof(ab_hmap_node));
    pmap->size = size;
    pmap->len = 0;    
    return pmap; 
}/*}}}*/


void ab_hmap_insert(ab_hmap *pmap, const char *key, const char *value)
{/*{{{*/
    //assert(pmap != NULL && key != NULL && value != NULL);
    if(NULL == pmap || NULL == key || NULL == value)
    {
        return;
    }
    unsigned int sign = ab_getsign(key, strlen(key), pmap->size);
    ab_hmap_node * currnode = pmap->bucket + sign;
    ab_hmap_node * prenode = NULL;
    currnode = pmap->bucket + sign;
    unsigned char has_key = FALSE;

    while(currnode != NULL && currnode->key != NULL)
    {
        if(strcmp(currnode->key, key) == 0)
        {
            has_key = TRUE;            
            break;
        }
        prenode = currnode;
        currnode = currnode->next;
    }
    if(has_key)
    {
        if(strcmp(currnode->value, value) != 0)
        {
            FREE(currnode->value);
            currnode->value = ab_strdup(value); 
        }     
    }
    else
    {
        if(currnode)
        {
            //currnode == pmap->bucket + sign
            currnode->key = ab_strdup(key);
            currnode->value = ab_strdup(value);        
            currnode->next = NULL;
            pmap->len += 1;
        }
        else
        {
            //new node, add into linklist
            currnode = (ab_hmap_node*)ab_calloc(1, sizeof(ab_hmap_node));
            currnode->key = ab_strdup(key);
            currnode->value = ab_strdup(value);
            currnode->next = NULL;
            prenode->next = currnode;            
            pmap->len += 1;            
        }        
    }    
}/*}}}*/


unsigned char ab_hmap_haskey(const ab_hmap *pmap, const char *key)
{/*{{{*/
    //assert(pmap != NULL && key != NULL);
    if(NULL == pmap || NULL == key)
        return FALSE;
    unsigned int sign = ab_getsign(key, strlen(key), pmap->size);
    ab_hmap_node * currnode = pmap->bucket + sign;    
    unsigned char has_key = FALSE;
    while(currnode != NULL && currnode->key != NULL)
    {
        if(strcmp(currnode->key, key) == 0)
        {                        
            has_key = TRUE;
            break;            
        }
        currnode = currnode->next;            
    } 
    
    return has_key;    
}/*}}}*/


const char *ab_hmap_get(const ab_hmap * pmap, const char *key)    
{/*{{{*/
    //assert(pmap != NULL && key != NULL);
    if(NULL == pmap || NULL == key)
        return NULL;
    unsigned int sign = ab_getsign(key, strlen(key), pmap->size);
    ab_hmap_node * currnode = pmap->bucket + sign;
    char *value = NULL;    
    while(currnode != NULL && currnode->key != NULL) 
    {
        if(strcmp(currnode->key, key) == 0)
        {
            value = currnode->value;
            break;                                
        }
        currnode = currnode->next;             
    }
    return value;
}/*}}}*/


void ab_hmap_delete(ab_hmap * pmap, const char *key)    
{/*{{{*/
    //assert(pmap != NULL && key != NULL);
    if(NULL == pmap || NULL == key)
        return;
    
    unsigned int sign = ab_getsign(key, strlen(key), pmap->size);    
    ab_hmap_node * currnode = NULL;
    ab_hmap_node * prenode = NULL;
    currnode = pmap->bucket + sign;
    unsigned char has_key = FALSE;            
    while(currnode != NULL && currnode->key != NULL)         
    {
        if(strcmp(currnode->key, key) == 0)
        {
            has_key = TRUE;            
            break;
        }
        prenode = currnode;
        currnode = currnode->next;
    }
    if(has_key)
    {
        if(currnode != pmap->bucket + sign)
        {
            //break the linklist
            prenode->next = currnode->next;
            FREE(currnode->key);
            FREE(currnode->value);
            FREE(currnode);
            pmap->len -= 1; 
        }                
		else
		{
			if(currnode->next == NULL)
			{
				FREE(currnode->key);
				FREE(currnode->value);
	            pmap->len -= 1;
			}
            else
            {
                *(pmap->bucket + sign) = *currnode->next;
                FREE(currnode->key);
                FREE(currnode->value);
                FREE(currnode);                
                pmap->len -= 1;
            }
		}
	}
}/*}}}*/	


void ab_hmap_output(const ab_hmap *pmap, FILE *fp)
{/*{{{*/
    //assert(pmap != NULL && fp != NULL);
    if(NULL == pmap || NULL == fp)
        return;

	if(fp == NULL) return ;
	fprintf(fp,"-------begin---%d <key,value> pairs\n", pmap->len);
	
    for(unsigned int i = 0;i <pmap->size;i++)        
    {
        ab_hmap_node * currnode = pmap->bucket + i;            
        while(currnode != NULL && currnode->key != NULL)
        {
            fprintf(fp, "<%s,%s>\n", currnode->key, currnode->value);
		    fflush(fp);
            currnode = currnode->next;                
		}
	}
	fprintf(fp, "-------end---------\n");
}/*}}}*/

void ab_hmap_destroy(ab_hmap *pmap)    
{/*{{{*/
	//assert(pmap != NULL);
    if(NULL == pmap)
        return;
    ab_hmap_node *currnode = NULL;
    ab_hmap_node *nextnode = NULL;

    //step 1 free hashnode in linklist
    for(unsigned int i = 0;i < pmap->size;i++)
    {
        currnode = pmap->bucket + i;
        FREE(currnode->key);
        FREE(currnode->value);
        currnode = currnode->next;
        while(currnode != NULL)
        {
            nextnode = currnode->next;
            FREE(currnode->key);
            FREE(currnode->value);
            FREE(currnode)            
            currnode = nextnode;
        }        
    }        

	//step2 free bucket
	FREE(pmap->bucket);

	//step3 free hashmap
	FREE(pmap);
}/*}}}*/

static unsigned int ab_getsign(const char *key,const unsigned int keylen,const unsigned int size)
{/*{{{*/    unsigned int h = 0;
    char *p = (char *)key;
    int n = keylen;    
    while (n--)
    {
        h = (h << 5) - h + *p;
        p++;
    }
    return h % size;    
}/*}}}*/


static void *ab_calloc(size_t nmemb,size_t size)
{/*{{{*/
    void *p;
    if( (p = calloc(nmemb,size)) == NULL)
    {
        fprintf(stderr,"Calloc error:can't calloc %u bytes\n",size);
        exit(0);
    }
    return p;
}/*}}}*/


static char *ab_strdup(const char *str)
{/*{{{*/
    //assert(str != NULL);
    if(NULL == str)
        return NULL;
    
    unsigned int len = strlen(str);
    char *dst = (char*)ab_calloc(len + 1, sizeof(char));
    strncpy(dst, str, len);
    
    return dst;    
}/*}}}*/
