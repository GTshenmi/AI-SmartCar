/*
 * queue.c
 *
 *  Created on: 2021Äê4ÔÂ26ÈÕ
 *      Author: 936305695
 */
#include "queue.h"

/*
    Queue
    [0 1 2 3 4 5 6 7 8 9 10 ... 90 91 92 93 94 95 96 97 98 99 100]
     c z                                                         
 */

void Queue_Init(queue_t *queue,float *data,sint32_t len)
{
    queue->MaxDataLen = len;

    queue->MaxPos = MaxQueueLen;

    for(int i = 0 ; i < queue->MaxPos ; i++)
    {
        queue->Data[i] = (data + i * queue->MaxDataLen);
    }

    queue->CurrentPos = 0;

    queue->MaxPos = MaxQueueLen;

    queue->ZeroPos = 1;
}

void Queue_RangeAssert(queue_t *queue)
{
    if(queue->CurrentPos >= queue->MaxPos)
        queue->CurrentPos = 0;
    
    if(queue->ZeroPos >= queue->MaxPos)
        queue->ZeroPos = 0;

    if(queue->CurrentPos < 0)
        queue->CurrentPos = queue->MaxPos - 1;

    if(queue->ZeroPos < 0)
        queue->ZeroPos = queue->MaxPos - 1;
}

float *Queue_SearchByIndex(queue_t *queue,sint32_t index)
{  
    return index < 0 ? queue->Data[index % MaxQueueLen + MaxQueueLen] : queue->Data[index % MaxQueueLen];
}

float *Queue_SearchByZeroIndex(queue_t *queue,sint32_t index)
{
    return Queue_SearchByIndex(queue,index + queue->ZeroPos - 1);
}

float *Queue_Gets(queue_t *queue,sint32_t index,float *data,sint32_t start,sint32_t end)
{
    sint32_t currentPos = queue->CurrentPos;

    float *srcAddr = Queue_SearchByIndex(queue,index + currentPos - 1);

    if(data != NULL)
        memcpy(data,srcAddr + start *sizeof(float),(end - start) * sizeof(float));

    return srcAddr;
}

void Queue_Puts(queue_t *queue,float *data,sint32_t start,sint32_t end)
{
    float *dstAddr = Queue_SearchByIndex(queue,queue->CurrentPos);

    memcpy(dstAddr + start *sizeof(float),data,(end - start) * sizeof(float));

    queue->CurrentPos++;
    queue->ZeroPos++;

    Queue_RangeAssert(queue);
}

void Queue_Print(queue_t *queue)
{
    for(int i = 0 ; i < MaxQueueLen ; i++)
    {
        printf("[%d]:",i);

        printf("[");
        for(int j = 0; j < queue->MaxDataLen ;j++)
        {
            printf("%f",queue->Data[i][j]);

            if(j == 7)
            {

            }
            else
            {
                printf(",");
            }
        }
        printf("]\n");
    }
}

squeue_m Queue =
{
    .Init = Queue_Init,

    .Gets = Queue_Gets,
    .Puts = Queue_Puts,
    .Print = Queue_Print,

    .SearchByIndex = Queue_SearchByIndex,
    .SearchByZeroIndex = Queue_SearchByZeroIndex,
};
