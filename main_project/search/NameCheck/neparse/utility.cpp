/**
 * @file utility.c
 * @author bfzhang
 * @brief some utility function used in this module
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "abutility.h"
#include "errno.h"
#include "HASHMAP.h"
#include <locale.h>

#define FALSE 0
#define TRUE 1
#define GB_LEN 2

static const int FULL_DGC_COUNT = 4;
static const char *FULL_DGC[] = 
{
    "北京市", "上海市", "天津市", "重庆市"    
};

static const int ALIAS_DGC_COUNT = 4;
static const char *ALIAS_DGC[] = 
{
    "北京", "上海", "天津", "重庆"
};

static void Mapping_SBCToDBC(char *szSource, uint32_t *mapping);
static void Mapping_removeTabAndNewline(char *szSource, uint32_t *mapping);
static void Mapping_combineMultipleSpaceToOne(char *szSource, uint32_t *mapping);

///public interface
void * ab_malloc(size_t size)
{/*{{{*/
	void *p;
	if( (p = malloc(size) ) == NULL)
	{
		fprintf(stderr,"Malloc error:can't malloc %u bytes\n",size);
		exit(0);
	}
	return p;
}/*}}}*/


void *ab_realloc(void *ptr,size_t size)
{/*{{{*/
	void *p;
	if( (p = realloc(ptr,size) ) == NULL)
	{
		fprintf(stderr,"Realloc error:can't relloc %u bytes\n",size);
		exit(0);
	}
	return p;
}/*}}}*/


void *ab_calloc(size_t nmemb,size_t size)
{/*{{{*/
	void *p;
	if( (p = calloc(nmemb,size)) == NULL)
	{
		fprintf(stderr,"Calloc error:can't calloc %u bytes\n",size);
		exit(0);
	}
	return p;
}/*}}}*/


void ab_fclose(FILE *fp)
{/*{{{*/
	if(fp != NULL && fclose(fp) != 0)
        perror("ab_fclose error\n");
}/*}}}*/


FILE *ab_fopen(const char *filename,const char *mode)
{/*{{{*/
	FILE *fp;
	
	if( (fp = fopen(filename,mode)) == NULL)
		fprintf(stderr,"ab_fopen error:%s is %s\n",filename,strerror(errno));
	return fp;
}/*}}}*/


void getMapping(char *szSource, uint32_t *mapping)
{/*{{{*/
    //assert(szSource != NULL);
    if(NULL == szSource)
        return;
    if(szSource[0] == 0)
        return;
    
    Mapping_SBCToDBC(szSource, mapping);

    Mapping_removeTabAndNewline(szSource, mapping); 

    Mapping_combineMultipleSpaceToOne(szSource, mapping);
}/*}}}*/


unsigned char isGBKCharacter(const char *str)
{/*{{{*/
    if(str == NULL || str[0] == 0 || strlen(str) != GB_LEN)
        return FALSE;

    unsigned char ch0 = str[0];
    unsigned char ch1 = str[1];;    
    if(ch0 >= 129 &&  ch1 >= 64)
        return TRUE;

    return FALSE; 
}/*}}}*/


///private interface
static void Mapping_SBCToDBC(char *szSource, uint32_t *mapping)
{/*{{{*/
    //assert(szSource != NULL);
    if(NULL == szSource)
        return;
    if(szSource[0] == 0)
        return;
    
    uint32_t cover = 0;
    uint32_t sourcelen = strlen(szSource);
    for(uint32_t i = 0;i < sourcelen;)
    {
        if(szSource[i] > 0)
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
            continue;
        }
        if(szSource[i] == -93)
        {
            mapping[cover] = mapping[i];
            i++;
            szSource[cover++] = szSource[i++] + 128;
        }
        else if(szSource[i] == -95 && szSource[i + 1] == -95)
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = ' ';
            i += 2;
        }
        else
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
        }
    }
    szSource[cover] = 0;
}/*}}}*/


static void Mapping_removeTabAndNewline(char *szSource, uint32_t *mapping)
{/*{{{*/
    //assert(szSource != NULL);
    if(NULL == szSource)
        return;
    if(szSource[0] == 0)
        return;
    
    uint32_t cover = 0;
    uint32_t sourcelen = strlen(szSource);
    for(uint32_t i = 0;i < sourcelen;)
    {
        if(szSource[i] == '\t' || szSource[i] == '\r' || szSource[i] == '\n')
        {
            mapping[cover] = mapping[i];
            szSource[cover] = ' ';
            cover++;
            i++;
        }
        else
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
        }
    }
    
    if(cover >= 1 && szSource[cover - 1] == ' ')
    {
        szSource[cover - 1] = 0;
    }
}/*}}}*/


static void Mapping_combineMultipleSpaceToOne(char *szSource, uint32_t *mapping)
{/*{{{*/
    //assert(szSource != NULL);
    if(NULL == szSource)
        return;
    if(szSource[0] == 0)
        return;
 
    uint32_t cover = 0;
    uint32_t sourcelen = strlen(szSource);
    for(uint32_t i = 0;i < sourcelen;)
    {
        if(szSource[i] != ' ')
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
            continue; 
        }
        if(i == 0)
        {
            mapping[cover] = mapping[i];
            szSource[cover++] = szSource[i++];
        }
        else
        {
            if(szSource[cover - 1] != ' ')
            {
                mapping[cover] = mapping[i];
                szSource[cover++] = szSource[i];
            }
            i++;
        }
    }  
    szSource[cover] = 0;    
}/*}}}*/


unsigned char isdgc(const char *str)
{/*{{{*/
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    unsigned char isdgc = FALSE;
    if(isfulldgc(str) || isaliasdgc(str))
        isdgc = TRUE;    
    return isdgc;
}/*}}}*/


unsigned char isfulldgc(const char *str)
{/*{{{*/
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    unsigned char isfulldgc = FALSE;

    for(int i = 0;i < FULL_DGC_COUNT;i++)
    {
        if(strcmp(FULL_DGC[i], str) == 0)
        {
            isfulldgc = TRUE;
            break;
        }
    }
 
    return isfulldgc;
}/*}}}*/


unsigned char isaliasdgc(const char *str)
{/*{{{*/
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    unsigned char isaliasdgc = FALSE;

    for(int i = 0;i < ALIAS_DGC_COUNT;i++)
    {
        if(strcmp(ALIAS_DGC[i], str) == 0)
        {
            isaliasdgc = TRUE;
            break;
        }
    }
 
    return isaliasdgc;
}/*}}}*/

