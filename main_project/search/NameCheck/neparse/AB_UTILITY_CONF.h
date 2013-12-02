#ifndef _AB_UTILITY_CONF_H
#define _AB_UTILITY_CONF_H

#include "AB_UTILITY_COMMON.h"
#include <stdio.h>


typedef struct {
char  szConfFileName[MAX_FILE_LEN];
FILE * fp;
}ABU_CONF_S ;

ABU_CONF_S  * ABU_CONF_open(char * szConfFile);


int ABU_CONF_writeString(ABU_CONF_S *, const char * szSectionName, const char * szName, char * szValue);
int ABU_CONF_readString(ABU_CONF_S *, const char * szSectionName, const char * szName,  char * sBuf, UINT32 dwBufSize);
int ABU_CONF_writeInt(ABU_CONF_S *, const char * szSectionName, const char * szName,int value);
int ABU_CONF_readInt(ABU_CONF_S *, const char * szSectionName, const char * szName, int * value);

int ABU_CONF_close(ABU_CONF_S *);

#endif
