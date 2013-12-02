/*******************************************************************************
* File name: AB_UTILITY_CONF.c
* Author: zhangyong  Version: 0.2  Date: 13/01/2007
* Description: 
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>


#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_LOG.h"
#include "AB_UTILITY_CONF.h"

/*typedef struct {
char  szConfFileName[MAX_FILE_LEN];
FILE * fp;
}ABU_CONF_S;*/

static char * trim(char * src)
{
	char *begin, *end = NULL;

	//find the first non blank character
	while(*src != '\0' && (*src == ' ' || *src == '\t'))
		++src;
	if(*src == '\0')
		return src;
	begin = src;
	++src;
	//skip all non blank
	while(*src != '\0' && *src != ' ' && *src != '\t')
		++src;
	if(*src == '\0')
		return begin;
	end = src;
	++src;
	while(*src != '\0'){
		//
		while(*src != '\0' && (*src == ' ' || *src == '\t'))
			++src;
		if(*src == '\0')//end of string
			break;
		//find a non blank character
		end = NULL;
		++src;
		while(*src != '\0' && *src != ' ' && *src != '\t')
			++src;
		if(*src == '\0')//end of string
			break;
		end = src;
		++src;
	}
	if(end != NULL)
		*end = '\0';
	return begin;
}

/**
  @desc:  open conf file
  @return: sccc: pointer to ABU_CONF_S
           failed: NULL
  */
ABU_CONF_S  *ABU_CONF_open(char * szConfFile)
{
	//assert(szConfFile != NULL);
    if(NULL == szConfFile)
        return NULL;
    ABU_CONF_S *pConf = (ABU_CONF_S *)malloc(sizeof(ABU_CONF_S));

    if (pConf == NULL)
        return NULL;
    else {
		memset(pConf, 0, sizeof(ABU_CONF_S));
        if(strlen(szConfFile) > MAX_FILE_LEN) {
            free(pConf);
            return NULL;
        }
		strncpy(pConf->szConfFileName, szConfFile, MAX_FILE_LEN);
        pConf->fp = fopen(szConfFile, "r+");
        if(pConf->fp == NULL) {
            pConf->fp = fopen(szConfFile, "w+");
            if(pConf->fp == NULL) {
                free(pConf);
                return NULL;
            } else
                return pConf;
        } else
            return pConf;
    }
}

int ABU_CONF_writeString(ABU_CONF_S * pConf, const char * szSectionName, const char * szName, char * szValue)
{
	char buf[256];
	long  pos1, pos2;
	long res = 0;
	fpos_t pos;
	fpos_t pos_temp;
	char * file_left;
	int seclen;

	fseek(pConf->fp, 0, SEEK_SET);
	while(fgets(buf, sizeof(buf), pConf->fp) != NULL) {
		if (buf[0] != '[') {
			continue;
		} else {
			seclen = strlen(szSectionName);
			if(strncmp(buf + 1, szSectionName, seclen) == 0) {
				if( buf[seclen + 1] == ']') {
					//find section
					fgetpos(pConf->fp, &pos);
					while(fgets(buf, sizeof(buf), pConf->fp) != NULL) {
						if( buf[0] == '[' ) {
							fsetpos(pConf->fp,&pos);
							pos1 = ftell(pConf->fp);

							//insert into tail of section 
							fseek(pConf->fp,0,SEEK_END);
							pos2 = ftell(pConf->fp);

							fsetpos(pConf->fp,&pos);
							res = pos2 - pos1;
							file_left = (char *)malloc(res);

							fread(file_left,1,res,pConf->fp);
							fsetpos(pConf->fp,&pos);

							fprintf(pConf->fp,"%s=%s\n",szName,szValue);
							fwrite(file_left,1,res,pConf->fp);

							free(file_left);

							return 0;

						} else {
							if(strncmp(buf,szName,strlen(szName))== 0) {
								if(buf[strlen(szName)] == '=') {  
									pos_temp= pos;
									fgetpos(pConf->fp,&pos);
									pos1 = ftell(pConf->fp);

									//insert into tail of section 
									fseek(pConf->fp,0,SEEK_END);
									pos2 = ftell(pConf->fp);

									fsetpos(pConf->fp,&pos);
									res = pos2 - pos1;
									file_left = (char *)malloc(res);

									fread(file_left,1,res,pConf->fp);
									fsetpos(pConf->fp,&pos_temp);

									fprintf(pConf->fp,"%s=%s\n",szName,szValue);
									fwrite(file_left,1,res,pConf->fp);
									free(file_left);

									return 0;
								}
							}
						}

						fgetpos(pConf->fp,&pos);
						memset(buf,0,sizeof(buf));

					}
					//last sectionbuf
					fprintf(pConf->fp,"%s=%s\n",szName,szValue);
					return 0;
				} else {
					continue;
				}
			}
		} 
		memset(buf,0,sizeof(buf));
	}
    fseek(pConf->fp, 0, SEEK_END);
    fprintf(pConf->fp, "\n\n");
	fprintf(pConf->fp,"[%s]\n",szSectionName);
	fprintf(pConf->fp,"%s=%s\n",szName,szValue);
	return 0;
}

/**
 return: 0  succ.
        -1 fail
        -2  not find szName or szSection
 
*/
int ABU_CONF_readString(ABU_CONF_S * pConf, const char * szSectionName, const char * szName,  char * sBuf, UINT32 dwBufSize)
{
	//assert(sBuf != NULL);
    if(NULL == sBuf)
        return -1;

    char *buf;
    char * buf_trim;
	int seclen;

    buf = (char *)malloc(256);
    fseek(pConf->fp, 0, SEEK_SET);
    while(fgets(buf,256,pConf->fp) != NULL){
		buf_trim = trim(buf);
        if(*buf_trim == '\0')
            continue;
        if(buf_trim[0] == '#' || buf_trim[0] == ';')
            continue;
        if (buf_trim[0] != '[')
            continue;
        else {
			buf_trim = trim(buf_trim + 1);
			seclen = strlen(szSectionName);
            if((strncmp(buf_trim, szSectionName, seclen)) == 0) {
				//find section
                if(buf_trim[seclen] == ']') {
					while(fgets(buf, 256, pConf->fp) != NULL) {
						buf_trim = trim(buf);
						if(*buf_trim == '\0')
							continue;
						if((buf_trim[0] == '#') || (buf_trim[0] == ';'))//ignor notes
							continue;

						//not found in this section
						if( buf_trim[0] == '[' ){
							free(buf);
							return -2;
						} else {
							seclen = strlen(szName);	
							//find szname
							if(strncmp(buf_trim, szName, seclen)== 0) {
								buf_trim = trim(buf_trim + seclen);
								if(buf_trim[0] == '=') {
									buf_trim = trim(buf_trim + 1);
									seclen = strlen(buf_trim);
									if((buf_trim[seclen - 1] == '\n') ||(buf_trim[seclen - 1] == 'r') ) {
										buf_trim[seclen - 1] = '\0';
									}
									if(dwBufSize < seclen) {
										//length of sBuf is not enough
										free(buf); 
										return -1;
									}
									strncpy(sBuf, buf_trim, seclen);
									sBuf[seclen] = '\0';
									free(buf);
									return 0;
								}
							}
						}
					}//end of while
					free(buf);
					return -2;
				} else {
					continue;
				}
			}
		}//end if
	}//end while
	free(buf);
	return -2;
}

int ABU_CONF_writeInt(ABU_CONF_S *pConf, const char * szSectionName, const char * szName,int value)
{
	char buffer[256];
	int res;
	snprintf(buffer, 256, "%d", value);
	res = ABU_CONF_writeString(pConf, szSectionName, szName, buffer);
	return res;
}

/**
 return: 0  succ.
        -1 fail
        -2  not find szName or szSection
 
*/
int ABU_CONF_readInt(ABU_CONF_S *pConf, const char * szSectionName, const char * szName, int * value)
{
	char buffer[256];
	int res;
	res = ABU_CONF_readString(pConf, szSectionName, szName, buffer, 256);
	if(res < 0)
		return res;
	*value = strtol(buffer, NULL, 0);
	return res;
}

int ABU_CONF_close(ABU_CONF_S *pConf)
{
    if (pConf == NULL)
        return -1;

    if(pConf->fp == NULL)
        return -1;

    if(fclose(pConf->fp) != 0)
        return -1;
    free(pConf);
    return 0;
}

