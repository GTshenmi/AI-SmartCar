/*
 * queue.h
 *
 *  Created on: 2021Äê4ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_QUEUE_H_
#define APP_SYS_QUEUE_H_

#include "platform.h"

typedef struct
{
    float Data[100][7];

    sint32_t CurrentPos;
    sint32_t ZeroPos;

    sint32_t MaxPos;
}esensor_queue_t;

typedef struct
{
    void (*Init)(esensor_queue_t *queue);

    float *(*SearchByIndex)(esensor_queue_t *queue,sint32_t index);
    float *(*SearchByZeroIndex)(esensor_queue_t *queue,sint32_t index);

    void (*Put)(esensor_queue_t *queue,float *data);
    float *(*Get)(esensor_queue_t *queue,sint32_t index,float *data);

    void (*Print)(esensor_queue_t *queue);
}squeue_m;

extern squeue_m EQueue;

#endif /* APP_SYS_QUEUE_H_ */
