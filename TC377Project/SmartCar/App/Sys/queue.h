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
}queue_t;

typedef struct
{
    void (*Init)(queue_t *queue,float *data,sint32_t len);

    float *(*SearchByIndex)(queue_t *queue,sint32_t index);
    float *(*SearchByZeroIndex)(queue_t *queue,sint32_t index);

    void (*Puts)(queue_t *queue,float *data,sint32_t start,sint32_t end);
    float *(*Gets)(queue_t *queue,sint32_t index,float *data,sint32_t start,sint32_t end);

    void (*Print)(queue_t *queue);
}squeue_m;

extern squeue_m Queue;

#endif /* APP_SYS_QUEUE_H_ */
