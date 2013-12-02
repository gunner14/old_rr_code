/**
 *
 *
 *
 *
 */

#ifndef _MANAGE_TYPES_H_
#define _MANAGE_TYPES_H_

#define _ERROR_FORMAT_ "{%-20s}{%-4d}{%-30s}\n"

#define MY_ASSERT_WITH_ERRNO(x) \
    do { \
        if( !(x) ) { \
            fprintf(stderr, "Assert with Predicate: %s Failed. with ERRNO: %d.\n", #x, errno); \
            fprintf(stderr, _ERROR_FORMAT_,  \
                    (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__);  \
            exit(-1); \
        } \
    }while(0)

#define MY_ASSERT_WITHOUT_ERRNO(x) \
    do { \
        if( !(x) ) { \
            fprintf(stderr, "Assert with Predicate: %s Failed.\n", #x); \
            fprintf(stderr, _ERROR_FORMAT_,  \
                    (char*)__FILE__, (int)__LINE__,(char*)__FUNCTION__);  \
            exit(-1); \
        } \
    }while(0)

#define MY_EXIT(fmt, arg...) \
    do {  \
        fprintf(stderr, fmt, ##arg); \
        exit(-1); \
    }while(0) 


typedef void *( *_THREAD_FUNC_ ) (void *userData);
typedef int (* _THREAD_HANDLER_ )(void *);
typedef void (* _FREE_FUNC_ ) (void *pValue);
typedef int (* _GENERAL_FUNC_ ) (void *pValue, void *userData);

#endif
//end of header file


