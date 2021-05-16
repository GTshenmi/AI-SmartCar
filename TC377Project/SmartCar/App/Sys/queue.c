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

void EQueue_Init(esensor_queue_t *queue)
{
    for(int i = 0 ; i < queue->MaxPos ; i++)
    {
        for(int j = 0 ; j < MaxDataLen ; j++)
        {
            queue->Data[i][j] = 0.0;
        }
    }

    queue->CurrentPos = 0;
    queue->MaxPos = MaxQueueLen;
    queue->ZeroPos = 1;
}

void EQueue_RangeAssert(esensor_queue_t *queue)
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

float *EQueue_SearchByIndex(esensor_queue_t *queue,sint32_t index)
{  
    return index < 0 ? queue->Data[index % MaxQueueLen + MaxQueueLen] : queue->Data[index % MaxQueueLen];
}

float *EQueue_SearchByZeroIndex(esensor_queue_t *queue,sint32_t index)
{
    return EQueue_SearchByIndex(queue,index + queue->ZeroPos - 1);
}

float *EQueue_Gets(esensor_queue_t *queue,sint32_t index,float *data,sint32_t start,sint32_t end)
{
    sint32_t currentPos = queue->CurrentPos;

    float *srcAddr = EQueue_SearchByIndex(queue,index + currentPos - 1);

    if(data != NULL)
        memcpy(data,srcAddr + start *sizeof(float),(end - start) * sizeof(float));

    return srcAddr;
}

void EQueue_Puts(esensor_queue_t *queue,float *data,sint32_t start,sint32_t end)
{
    float *dstAddr = EQueue_SearchByIndex(queue,queue->CurrentPos);

    memcpy(dstAddr + start *sizeof(float),data,(end - start) * sizeof(float));

    queue->CurrentPos++;
    queue->ZeroPos++;

    EQueue_RangeAssert(queue);
}

void EQueue_Print(esensor_queue_t *queue)
{
    for(int i = 0 ; i < MaxQueueLen ; i++)
    {
        printf("[%d]:",i);

        printf("[");
        for(int j = 0; j < MaxDataLen ;j++)
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

squeue_m EQueue = 
{
    .Init = EQueue_Init,

    .Gets = EQueue_Gets,
    .Puts = EQueue_Puts,
    .Print = EQueue_Print,

    .SearchByIndex = EQueue_SearchByIndex,
    .SearchByZeroIndex = EQueue_SearchByZeroIndex,  
};
