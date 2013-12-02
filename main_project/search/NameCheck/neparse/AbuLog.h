#ifndef __AbuLog_H__
#define __AbuLog_H__

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum{
    ABU_LOG_NONE=0,
    ABU_LOG_FATAL=1,
    ABU_LOG_WARN=2,
    ABU_LOG_INFO=3,
    ABU_LOG_DEBUG=4
}ABU_LOG_LEVEL_E;

#ifndef ABU_LOG_LEVEL
#define ABU_LOG_LEVEL ABU_LOG_DEBUG
#endif

#define _ABU_LOG_FORMAT_ "%25s{%-10lu}{%-1d}{PID %10d}{%-20s}{%-4d}{%-30s}\n"

#define ABU_LOG_DEBUG(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_DEBUG,fmt,##arg)

#define ABU_LOG_INFO(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_INFO,fmt,##arg)

#define ABU_LOG_WARN(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_WARN,fmt,##arg)

#define ABU_LOG_FATAL(fmt,arg...) \
        ABU_LOG_write(ABU_LOG_FATAL,fmt,##arg)
 
/*
* prototypes
*/

#define ABU_LOG_write(l,fmt,arg...) \
do {  \
    if ((l) <= ABU_LOG_LEVEL) { \
         time_t now; time(&now); \
        _ABU_LOG_write(""_ABU_LOG_FORMAT_ fmt"\n", \
                ctime(&now), (unsigned int)now, l, getpid(), (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
    }\
}while(0)

#define ABU_ERROR(fmt,arg...) \
do {  \
    time_t now; time(&now); \
    fprintf(stderr, ""_ABU_LOG_FORMAT_ fmt"\n", \
            ctime(&now), (unsigned int)now, ABU_LOG_FATAL, getpid(), (char*) __FILE__, (int)__LINE__,(char*)__FUNCTION__, ##arg);  \
}while(0)

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

#endif //__AbuLog_H__
