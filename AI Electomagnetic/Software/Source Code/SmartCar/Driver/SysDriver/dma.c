/*
 * dma.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "dma.h"

uint8_t DMAx_Init(dmax_t *dma)
{
    return 0;
}
uint8_t DMAx_Start(dmax_t *dma)
{
    return 0;
}
uint8_t DMAx_Stop(dmax_t *dma)
{
    return 0;
}

sdma_m DMAx =
{
        .Init = DMAx_Init,
        .Start = DMAx_Start,
        .Stop = DMAx_Stop,
};

