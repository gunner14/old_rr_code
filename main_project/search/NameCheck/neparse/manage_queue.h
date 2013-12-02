typedef struct queue_item{
	struct 	queue_item *pNext;
	void *pData;
}QUEUE_ITEM_S;

typedef struct queue{
	QUEUE_ITEM_S *pHead; //used in queue_pop()
	QUEUE_ITEM_S *pTail; //used in queue_add()
	int item_count;
    int item_max;
}QUEUE_S;

QUEUE_S* QueueCreate(int max_item_num);
int QueueItemAdd(QUEUE_S *pQueue, void *pData);
void *QueueItemPop(QUEUE_S *pQueue);

