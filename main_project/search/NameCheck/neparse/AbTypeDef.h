#ifndef __AB_TYPE_DEF_H__
#define __AB_TYPE_DEF_H__

#include <pthread.h>
/**
define the data type
*/

#ifndef INT64
#define  INT64   long long
#endif

#ifndef INT32
#define  INT32   int
#endif

#ifndef INT16
#define  INT16   short
#endif

#ifndef UINT64
#define  UINT64   unsigned long long
#endif

#ifndef UINT32
#define  UINT32   unsigned int
#endif

#ifndef UINT16
#define  UINT16   unsigned short
#endif

#ifndef UINT8
#define  UINT8   unsigned char
#endif

/*
#ifndef bool
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
*/

#ifndef LONGLONG
#define LONGLONG long long
#endif

#define AB_PTHREAD_RWLOCK_W_PREFER \
    { __LOCK_INITIALIZER, 0, NULL, NULL, NULL, \
    PTHREAD_RWLOCK_PREFER_WRITER_NP, PTHREAD_PROCESS_PRIVATE }

#endif
