/*
 * spi.c
 *
 *  Created on: 2020��12��13��
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
    spi_tranxfer.txData = txData;   //��������
    spi_tranxfer.rxData = rxData;   //��������
    spi_tranxfer.dataSize = len;    //����
    spi_tranxfer.configFlags = (spi->Cs_num)|kLPSPI_MasterPcsContinuous;     //Ƭѡ 
    
    return LPSPI_MasterTransferBlocking(spi->Base, &spi_tranxfer);        //spi��������;
}

sspi_m SPIx =
{
        .Init = SPIx_Init,
        .ReadWriteBytes = SPIx_ReadWriteBytes,
        .Time_Infinite = SPIx_MAX_TIME_OUT,
};

