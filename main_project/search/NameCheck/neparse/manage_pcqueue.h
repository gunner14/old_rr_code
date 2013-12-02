/**
 *
 *
 **/
#ifndef _MANAGE_PCQUEUE_H_
#define _MANAGE_PCQUEUE_H_

#include <pthread.h>
#include "manage_queue.h"

typedef enum {
	QUEUE_INVALID,
	QUEUE_VALID,
	QUEUE_EXIT,
}PCQUEUE_STATE_E;

typedef struct PCQueue{
    int max_queue_len;
    PCQUEUE_STATE_E state;
    QUEUE_S *pQueue;
    pthread_cond_t pcqueue_cond;
    pthread_mutex_t pcqueue_mutex;
}PCQUEUE_S;

PCQUEUE_S *
Init_PCQueue(
        int max_queue_len
        );

void
Free_PCQueue(
        PCQUEUE_S *pPCQueue
        );

int 
PutItem_ToPCQueue(
        PCQUEUE_S *pPCQueue,
        void *pValue
        );

int
GetItem_FromPCQueue(
        PCQUEUE_S *pPCQueue,
        void *ppValue
        );

int 
SetPCQueueState(
        PCQUEUE_S *pPCQueue,
        PCQUEUE_STATE_E state
        );

#endif

