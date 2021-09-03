/*
 * dma.h
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_DMA_H_
#define DRIVER_SYSDRIVER_DMA_H_

#include "LQ_DMA.h"
#include "chipdatatype.h"

typedef struct
{
     uint32_t SrcStartAddr;
     uint32_t DstStartAddr;
     uint32_t Channel;
}dmax_t;

typedef struct
{
    uint8_t (*Init)(dmax_t *);
    uint8_t (*Start)(dmax_t *);
    uint8_t (*Stop)(dmax_t *);
}sdma_m;

extern sdma_m DMAx;

#endif /* DRIVER_SYSDRIVER_DMA_H_ */
