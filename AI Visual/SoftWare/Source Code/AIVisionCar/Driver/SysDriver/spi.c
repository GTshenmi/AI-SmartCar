/*
 * spi.c
 *
 *  Created on: 2020年12月13日
 *      Author: 936305695
 */
#include "spi.h"

uint8_t SPIx_Init(spix_t *spi)
{
    SPI_MasterInitConfig(spi->Base,spi->Cs_num,spi->Mode,spi->BaudRate);
    return 0;
}

uint8_t SPIx_ReadWriteBytes(spix_t *spi,uint8_t *txData, uint8_t *rxData, uint32_t len,sint64_t time_out)
{
    lpspi_transfer_t spi_tranxfer;
    spi_tranxfer.txData = txData;   //发送数组
    spi_tranxfer.rxData = rxData;   //接收数组
    spi_tranxfer.dataSize = len;    //长度
    spi_tranxfer.configFlags = (spi->Cs_num)|kLPSPI_MasterPcsContinuous;     //片选 
    
    return LPSPI_MasterTransferBlocking(spi->Base, &spi_tranxfer);        //spi堵塞传输;
}

sspi_m SPIx =
{
        .Init = SPIx_Init,
        .ReadWriteBytes = SPIx_ReadWriteBytes,
        .Time_Infinite = SPIx_MAX_TIME_OUT,
};

