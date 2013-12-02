#ifndef _MANAGE_LOG_H_
#define _MANAGE_LOG_H_

#include "AB_UTILITY_COMMON.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define LOG_FILE_CONTEN_LEN 512*1024*1024 //1G per file
#define MAX_LINE_LENGTH 1024*1024 //max length of log

/** 
 * @brief 
 */
typedef struct log_struct{
    FILE *fp;
    int curFileNum;
    int maxFileNum;
    unsigned long int filesize;
    char logDir[MAX_FILE_LEN];
    char logPrefix[MAX_FILE_LEN];
    char logBuffer[MAX_LINE_LENGTH];
    pthread_mutex_t file_mutex; 
}LOG_S;


/** 
  * @brief 4 kinds of error level.
    LOG_LEVEL_FATAL:  used for fatal errors.
    LOG_LEVEL_WARNING: used for warnings.
    LOG_LEVEL_INFO: used for information
    LOG_LEVEL_DEBUG: lowest level, used for debuging
  */
typedef enum{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_FATAL = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
}LOG_LEVEL_E;

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define LOG_PERMS (S_IRWXU|S_IRWXG)

/*
 * use marco to output log by level
 */

//time, filename, lineseqno, functionname
#define _LOG_FORMAT_ "%25s{%-20s}{%-4d}{%-30s}\n"

#define LOG_DEBUG(plog, fmt,arg...) \
        LOG_WRITE(LOG_LEVEL_DEBUG, plog, fmt,##arg)

#define LOG_INFO(plog, fmt,arg...) \
        LOG_WRITE(LOG_LEVEL_INFO, plog, fmt,##arg)

#define LOG_WARN(plog, fmt,arg...) \
        LOG_WRITE(LOG_LEVEL_WARN, plog, fmt,##arg)

#define LOG_FATAL(plog, fmt,arg...) \
        LOG_WRITE(LOG_LEVEL_FATAL, plog, fmt,##arg)
 
/*
*  MUST call LOG_prepare_r before using following functions
*/
#define LOG_DEBUG_r(plog, fmt,arg...) \
        LOG_WRITE_R(LOG_LEVEL_DEBUG, plog, fmt,##arg)

#define LOG_INFO_r(plog, fmt,arg...) \
        LOG_WRITE_R(LOG_LEVEL_INFO, plog, fmt,##arg)

#define LOG_WARN_r(plog, fmt,arg...) \
        LOG_WRITE_R(LOG_LEVEL_WARN, plog, fmt,##arg)

#define LOG_FATAL_r(plog, fmt,arg...) \
        LOG_WRITE_R(LOG_LEVEL_FATAL, plog, fmt,##arg)
 

/*
* prototypes
*/
#define LOG_WRITE(l, plog, fmt,arg...) \
    do {  \
        if ((l) <= LOG_LEVEL) { \
            time_t now; time(&now); \
            LOG_write(plog, ""_LOG_FORMAT_ fmt"\n", \
                    ctime(&now), (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
        }\
    }while(0)

#define LOG_WRITE_R(l, plog, fmt,arg...) \
    do {  \
        if ((l) <= LOG_LEVEL) { \
            time_t now; time(&now); \
            char buf[35]; \
            LOG_write_r(plog, ""_LOG_FORMAT_ fmt"\n", \
                    ctime_r(&now,buf), (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
        } \
    }while(0)

#define ERROR_R(fmt,arg...) \
    do {  \
        if ( LOG_LEVEL_DEBUG == LOG_LEVEL) { \
            time_t now; time(&now); \
            char buf[35]; \
            fprintf(stderr,""_LOG_FORMAT_ fmt"\n", \
                    ctime_r(&now, buf), (char*) __FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
        }\
    }while(0)

/** 
  * @return 0: succ; -1: failed
  */
int LOG_prepare_r(LOG_S *pLog) ;

/** 
  * @return  0: succ; -1 failure
  */
LOG_S *LOG_init(char *logDir, char *logPrefix, int maxFileNum);

/** 
  * @return  number of log to write; -1 if failure
  */
int LOG_write(LOG_S *pLog, const char * args,...);

/** 
  * @return  number of log to write; -1 if failure
  */
int LOG_write_r(LOG_S *pLog, const char * args,...);  // MT safe version

void LOG_close(LOG_S *pLog);

#endif
