/*******************************************************************************
 * File name: AB_UTILITY_LOG.c
 * Author: zhangyong  Version: 0.2  Date: 13/01/2007
 * Description: 
 *
 *******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include "AB_UTILITY_LOG.h"

//a log object for global usage
static ABU_LOG_S *pLog = NULL;
/** 
 * @brief initialize log 
 * @log   updated by jianbo, [2007-01-12 18:00 CST]
 * 
 * @param sInd the log filename
 * 
 * @return  0: success; -1 failure
 */
int ABU_LOG_init(char * sInd)/*{{{*/
{
   if (!sInd) {
        _AB_ERROR("null file name.");
        return -1;
    }
    if(strlen(sInd) >= MAX_FILE_LEN - 4){//filename is prefix.NN
        _AB_ERROR("File name prefix is too long.");
        return -1;
    }

    if (pLog) {
        _AB_ERROR("Log handle existed!.");
        return -1;
    } else {
        pLog = (ABU_LOG_S *)malloc(sizeof(ABU_LOG_S));
    }

    if (pLog == NULL) {
        _AB_ERROR("can not calloc mem.\n");
        return -1;
    }
    memset(pLog, 0, sizeof(ABU_LOG_S));
    strncpy(pLog->szFilePrefix, sInd, MAX_FILE_LEN);

    int i = 1;
    struct stat fst;
    unsigned int latest = 0;
    char filename[MAX_FILE_LEN];

    pLog->fileid = 1;
    //find the latest file to write
    for(; i <= MAX_FILE_COUNT; ++i){/*{{{*/
        snprintf(filename, MAX_FILE_LEN, "%s.%d", sInd, i);
        if(access(filename, F_OK) < 0)//file not exsited
            continue;
        //for exsited file
        if(stat(filename, &fst) < 0){
            _AB_ERROR("Gather file stat %s failure. ", filename);
            free(pLog);
            pLog = NULL;
            return -1;
        }
        if(((unsigned int)fst.st_mtime) > latest){
            pLog->fileid = i;
            latest = fst.st_mtime;
        }
    }/*}}}*/

    //open log file
	if(access(pLog->szFilePrefix, F_OK) < 0){//file not exist
		pLog->filesize = 0;
	}else{//file exist
		if(stat(pLog->szFilePrefix, &fst) < 0){
			_AB_ERROR("Gather file stat %s failure.", filename);
			free(pLog);
			pLog = NULL;
			return -1;
		}
		pLog->filesize  = fst.st_size;
	}
    pLog->fp = fopen(pLog->szFilePrefix, "a");//open file for append
    if(pLog->fp == NULL) {
        _AB_ERROR("open file %s failed.", pLog->szFilePrefix);
        free(pLog);
        pLog = NULL;
        return -1;
    }
    return 0;
}/*}}}*/

/** 
 * @brief init mutex
 * 
 * @param pLog 
 * 
 * @return  0: succ; -1 failure
 */
int ABU_LOG_prepare_r() {/*{{{*/
    if (pLog != NULL) {
        pthread_mutex_init(&(pLog->mutex), NULL);
        return 0;
    }
    return -1;
}/*}}}*/

int _ABU_LOG_write_v(const char * args, va_list va)/*{{{*/
{
    if(pLog == NULL || pLog->fp == NULL){
        _AB_ERROR("Do not initialize log system.");
        return -1;
    }
    int i, writecount;

    memset(pLog->logbuffer, 0, MAX_LINE_LENGTH);

    i = vsnprintf(pLog->logbuffer, MAX_LINE_LENGTH, args, va);

    //file size too big, create a new file
    char filename[MAX_FILE_LEN];
    if(pLog->filesize + i > MAX_FILE_CONTEN_LEN){/*{{{*/
        fclose(pLog->fp);//close old file
        if(pLog->fileid == MAX_FILE_COUNT)
            pLog->fileid = 1;
        else
            pLog->fileid++;
        pLog->filesize = 0;//new file

        snprintf(filename, MAX_FILE_LEN, "%s.%d", pLog->szFilePrefix, pLog->fileid);
		if(rename(pLog->szFilePrefix, filename) < 0){
			_AB_ERROR("Can not rename file: %s", filename);
			return -1;
		}
        pLog->fp = fopen(pLog->szFilePrefix, "w");//if file exsited, truncated it
        if(pLog->fp == NULL){
            _AB_ERROR("Can not open file: %s.", pLog->szFilePrefix);
            return -1;
        }
    }/*}}}*/
    writecount = fwrite(pLog->logbuffer, 1, i, pLog->fp);
    if(writecount != i){
        _AB_ERROR("Write to file: %s error.", pLog->szFilePrefix);
        return -1;
    }
	fflush(pLog->fp);
    pLog->filesize += i;

#if DEBUG_OUTPUT_STDERR
    vfprintf(stderr,args,va);
#endif 
#if DEBUG_OUTPUT_STDOUT
    vfprintf(stdout,args,va);
#endif 
    return i;
}/*}}}*/

int _ABU_LOG_write(const char *args, ...)/*{{{*/
{
    int res;
    va_list arg_list;
    va_start(arg_list, args);
    res = _ABU_LOG_write_v(args, arg_list);
    va_end(arg_list);
    return res;
}/*}}}*/

int _ABU_LOG_write_r(const char * args,...)  // MT safe version/*{{{*/
{
    if (!pLog) return -1;
    va_list arg_list;
    int res;
    pthread_mutex_lock(&(pLog->mutex));
    va_start(arg_list, args);
    res = _ABU_LOG_write_v(args, arg_list);
    va_end(arg_list);
    pthread_mutex_unlock(&(pLog->mutex));
    return res;

}/*}}}*/

void ABU_LOG_close()/*{{{*/
{
    if(pLog == NULL){
        _AB_ERROR("Log has not been initialized. ");
        return;
    }
    fclose(pLog->fp);
    pthread_mutex_destroy(&pLog->mutex);
    ABU_FREE(pLog);
}/*}}}*/
