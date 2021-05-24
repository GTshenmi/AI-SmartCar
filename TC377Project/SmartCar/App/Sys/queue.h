/*
 * queue.h
 *
 *  Created on: 2021Äê4ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_QUEUE_H_
#define APP_SYS_QUEUE_H_

#include "platform.h"

#define MaxQueueLen 100
//#define MaxDataLen  9


typedef struct
{
    float *Data[MaxQueueLen];

    sint32_t MaxDataLen;

    sint32_t CurrentPos;
    sint32_t ZeroPos;

    sint32_t MaxPos;
}esensor_queue_t;

typedef struct
{
    void (*Init)(esensor_queue_t *queue,float *data,sint32_t len);

    float *(*SearchByIndex)(esensor_queue_t *queue,sint32_t index);
    float *(*SearchByZeroIndex)(esensor_queue_t *queue,sint32_t index);

    void (*Puts)(esensor_queue_t *queue,float *data,sint32_t start,sint32_t end);
    float *(*Gets)(esensor_queue_t *queue,sint32_t index,float *data,sint32_t start,sint32_t end);

    void (*Print)(esensor_queue_t *queue);
}squeue_m;

extern squeue_m EQueue;

#endif /* APP_SYS_QUEUE_H_ */
