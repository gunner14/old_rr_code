/**
 * @file ab_hset.c
 * @author bfzhang
 * @date 2007-02-12
 * @description: ab hash set
 * @note: reconstruct in 200809-04
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ab_hset.h"

#define FREE(p) if(p){ free(p); p = NULL;}
#define FALSE 0
#define TRUE 1 
static void *ab_calloc(size_t nmemb,size_t size);
static char *ab_strdup(const char *str);
static unsigned int ab_getsign(const char *key,const unsigned int keylen,const unsigned int size);

ab_hset * ab_hset_init(const unsigned int size)
{/*{{{*/
    //assert(size > 0);
    if(size <= 0)
        return NULL;
    ab_hset *pset = (ab_hset *)ab_calloc(1, sizeof(ab_hset));
    pset->bucket = (ab_hset_node *)ab_calloc(size, sizeof(ab_hset_node));
    pset->size = size;
    pset->len = 0;
    return pset;        
}/*}}}*/


void ab_hset_insert(ab_hset *pset, const char *key)
{/*{{{*/
    //assert(pset != NULL && key != NULL);
    if(NULL == pset || NULL == key)
        return;
    unsigned int sign = ab_getsign(key, strlen(key), pset->size);
    ab_hset_node * currnode = pset->bucket + sign;
    ab_hset_node * prenode = NULL;
    currnode = pset->bucket + sign;
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

    if(!has_key)    
    {
        if(currnode)
        {
            //currnode == pset->bucket + sign
            currnode->key = ab_strdup(key);
            currnode->next = NULL;
            pset->len += 1;
        }
        else
        {
            //new node, add into linklist
            currnode = (ab_hset_node*)ab_calloc(1, sizeof(ab_hset_node));
            currnode->key = ab_strdup(key);
            currnode->next = NULL;
            prenode->next = currnode;            
            pset->len += 1;            
        }        
    }
}/*}}}*/


unsigned char ab_hset_haskey(const ab_hset *pset, const char *key)
{/*{{{*/
    //assert(pset != NULL && key != NULL);
    if(NULL == pset || NULL == key)
        return FALSE;

    unsigned int sign = ab_getsign(key, strlen(key), pset->size);
    ab_hset_node * currnode = pset->bucket + sign;    
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


void ab_hset_delete(ab_hset * pset, const char *key)    
{/*{{{*/
    //assert(pset != NULL && key != NULL);
    if(NULL == pset || NULL == key)
        return;
    unsigned int sign = ab_getsign(key, strlen(key), pset->size);    
    ab_hset_node * currnode = NULL;
    ab_hset_node * prenode = NULL;
    currnode = pset->bucket + sign;
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
        if(currnode != pset->bucket + sign)
        {
            //break the linklist
            prenode->next = currnode->next;
            FREE(currnode->key);
            FREE(currnode);
            pset->len -= 1; 
        }                
		else
		{
			if(currnode->next == NULL)
			{
				FREE(currnode->key);
	            pset->len -= 1;
			}
            else
            {
                *(pset->bucket + sign) = *currnode->next;
                FREE(currnode->key);
                FREE(currnode);                
                pset->len -= 1;
            }
		}
	}
}/*}}}*/	


void ab_hset_output(const ab_hset *pset, FILE *fp)
{/*{{{*/
    //assert(pset != NULL && fp != NULL);
    if(NULL == pset || NULL == fp)
        return;

	if(fp == NULL) return ;
	fprintf(fp,"-------begin---%d key\n", pset->len);
	
    for(unsigned int i = 0;i <pset->size;i++)        
    {
        ab_hset_node * currnode = pset->bucket + i;            
        while(currnode != NULL && currnode->key != NULL)
        {
            fprintf(fp, "<%s>\n", currnode->key);
		    fflush(fp);
            currnode = currnode->next;                
		}
	}
	fprintf(fp, "-------end---------\n");
}/*}}}*/

void ab_hset_destroy(ab_hset *pset)    
{/*{{{*/
	//assert(pset != NULL);
    if(NULL == pset)
        return;
    ab_hset_node *currnode = NULL;
    ab_hset_node *nextnode = NULL;

    //step 1 free hashnode in linklist
    for(unsigned int i = 0;i < pset->size;i++)
    {
        currnode = pset->bucket + i;
        FREE(currnode->key);
        currnode = currnode->next;
        while(currnode != NULL)
        {
            nextnode = currnode->next;
            FREE(currnode->key);
            FREE(currnode)            
            currnode = nextnode;
        }        
    }        

	//step2 free bucket
	FREE(pset->bucket);

	//step3 free hashset
	FREE(pset);
}/*}}}*/

static unsigned int ab_getsign(const char *key,const unsigned int keylen,const unsigned int size)
{/*{{{*/
    unsigned int h = 0;
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
