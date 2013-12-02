/******************************************************
* Filename: ab_stdio.h
* 
* Date: 2007-06-28
* Description: provide the wrappers for stdandard io functions
******************************************************/
#ifndef _AB_STDIO_H
#define _AB_STDIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void ab_fclose(FILE *fp);

FILE* ab_fdopen(int fd,const char *type);

char* ab_fgets(char *ptr,int n,FILE *stream);

FILE * ab_fopen(const char *filename,const char *mode);

void ab_fputs(const char *ptr,FILE *stream);

size_t ab_fread(void *ptr,size_t size,size_t nmemb,FILE *stream);

void ab_fwrite(const void *ptr,size_t size,size_t nmemb,FILE *stream);
#endif
