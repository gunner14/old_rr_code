#ifndef _AB_UTILITY_LOG_H
#define _AB_UTILITY_LOG_H

/*
   $Id: AB_UTILITY_LOG.h,v 1.23 2007/03/02 04:11:55 jduan Exp $
@desc
   Output log into a specified file. The log will be rolled back from beginning after 
   the max size is met.
@changelog
   [2007-01-13 16:50 CST],e2524c|Jianbo,  Note: the log API is not safe for multiple processes so far.
*/
#include "AB_UTILITY_COMMON.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_FILE_CONTEN_LEN 1024*1024*1024 //1G per file
#define MAX_FILE_COUNT 2 //max file count
#define MAX_LINE_LENGTH 1024*1024 //max length of log

/** 
 * @brief 
 */
typedef struct _ABU_LOG{
    FILE * fp;
    int fileid;
    unsigned long int filesize;
    char szFilePrefix[MAX_FILE_LEN];
    char logbuffer[MAX_LINE_LENGTH];
    pthread_mutex_t mutex; 
} ABU_LOG_S;


/** 
  * @brief 4 kinds of error level.
    ABU_LOG_LEVEL_FATAL:  used for fatal errors.
    ABU_LOG_LEVEL_WARNING: used for warnings.
    ABU_LOG_LEVEL_INFO: used for information
    ABU_LOG_LEVEL_DEBUG: lowest level, used for debuging
  */
typedef enum{
    ABU_LOG_LEVEL_NONE=0,
    ABU_LOG_LEVEL_FATAL=1,
    ABU_LOG_LEVEL_WARNING,
    ABU_LOG_LEVEL_INFO,
    ABU_LOG_LEVEL_DEBUG
}ABU_LOG_LEVEL_E;

#ifndef ABU_LOG_LEVEL
#define ABU_LOG_LEVEL ABU_LOG_LEVEL_DEBUG
#endif

/*
 * use marco to output log by level
 */

#define _ABU_LOG_FORMAT_ "%25s{%-10lu}{%-1d}{PID %10d}{TID %10lu}{%-20s}{%-4d}{%-30s}\n"

#define ABU_LOG_DEBUG(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_LEVEL_DEBUG,fmt,##arg)

#define ABU_LOG_INFO(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_LEVEL_INFO,fmt,##arg)

#define ABU_LOG_WARN(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_LEVEL_WARNING,fmt,##arg)

#define ABU_LOG_FATAL(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_LEVEL_FATAL,fmt,##arg)
 
/*
*  MUST call ABU_LOG_prepare_r before using following functions
*/
#define ABU_LOG_DEBUG_r(fmt,arg...) \
        ABU_LOG_write_r(ABU_LOG_LEVEL_DEBUG,fmt,##arg)

#define ABU_LOG_INFO_r(fmt,arg...) \
        ABU_LOG_write_r(ABU_LOG_LEVEL_INFO,fmt,##arg)

#define ABU_LOG_WARN_r(fmt,arg...) \
        ABU_LOG_write_r(ABU_LOG_LEVEL_WARNING,fmt,##arg)

#define ABU_LOG_FATAL_r(fmt,arg...) \
        ABU_LOG_write_r(ABU_LOG_LEVEL_FATAL,fmt,##arg)
 

/*
* prototypes
*/
#define ABU_LOG_print(l,fmt,arg...) \
do {  \
    if ((l) <= ABU_LOG_LEVEL) { \
        _ABU_LOG_write_r(fmt"\n", ##arg);  \
    }\
}while(0)


#define ABU_LOG_write(l,fmt,arg...) \
do {  \
    if ((l) <= ABU_LOG_LEVEL) { \
         time_t now; time(&now); \
        _ABU_LOG_write(""_ABU_LOG_FORMAT_ fmt"\n", \
                ctime(&now), (unsigned int)now, l, getpid(), pthread_self(), (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
    }\
}while(0)

#define ABU_LOG_write_r(l,fmt,arg...) \
do {  \
    if ((l) <= ABU_LOG_LEVEL) { \
         time_t now; time(&now); \
         char buf[35]; \
        _ABU_LOG_write_r(""_ABU_LOG_FORMAT_ fmt"\n", \
                ctime_r(&now,buf), (unsigned int)now, l, getpid(), pthread_self(), (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
    } \
}while(0)

#define _AB_ERROR(fmt,arg...) \
do {  \
    if ( ABU_LOG_LEVEL_DEBUG == ABU_LOG_LEVEL) { \
        time_t now; time(&now); \
        fprintf(stderr,"%25s{%-10u}{PID %10d}{TID %lu}{%-20s}{%-4d}{%-30s}\n"fmt"\n\n", \
                         ctime(&now), (unsigned int)now,getpid(), pthread_self(),(char*) __FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
    }\
}while(0)

/** 
  * @brief To initialize mutex. must be called in the main process before forking threads 
  * 
  * @param pLog 
  * 
  * @return 0: succ; -1: failed
  */
int ABU_LOG_prepare_r() ;

/** 
  * @brief 
  * 
  * @param sInd 
  * 
  * @return  0: succ; -1 failure
  */
int ABU_LOG_init(char * sInd);

/** 
  * @brief 
  * 
  * @param  
  * @param args 
  * @param ... 
  * 
  * @return  number of log to write; -1 if failure
  */
int _ABU_LOG_write(const char * args,...);

/** 
  * @brief 
  * 
  * @param  
  * @param args 
  * @param ... 
  * 
  * @return  number of log to write; -1 if failure
  */
int _ABU_LOG_write_r(const char * args,...);  // MT safe version

/** 
  * @brief 
  * 
  * @param  
  * 
  * @return 
  */
void ABU_LOG_close();

#endif

