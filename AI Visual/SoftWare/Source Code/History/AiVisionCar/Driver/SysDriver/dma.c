/*
 * dma.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "dma.h"
uint8_t DMAx_Init(dmax_t *dma)
{
    //DMA_CameraInitConfig(dma->SrcStartAddr,dma->DstStartAddr,dma->Channel);
    return 0;
}
uint8_t DMAx_Start(dmax_t *dma)
{
//    /* DMA ���ô���Ŀ�ĵ�ַ  */
//    IfxDma_setChannelDestinationAddress(&MODULE_DMA, dma->Channel,
//            (void *) IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), dma->SrcStartAddr));
//
//    /* ����DMA����  */
//    IfxDma_enableChannelTransaction(&MODULE_DMA, dma->Channel);
    return 0;
}
uint8_t DMAx_Stop(dmax_t *dma)
{
    /* ����DMA���� �����־λ */
  //  IfxDma_disableChannelTransaction(&MODULE_DMA, dma->Channel);
    return 0;
}

sdma_m DMAx =
{
        .Init = DMAx_Init,
        .Start = DMAx_Start,
        .Stop = DMAx_Stop,
};

