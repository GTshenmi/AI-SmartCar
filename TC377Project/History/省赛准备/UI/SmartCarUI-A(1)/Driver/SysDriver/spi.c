/*
 * spi.c
 *
 *  Created on: 2020��12��13��
 *      Author: 936305695
 */
#include "spi.h"

uint8_t SPIx_Init(spix_t *spi)
{
    if(spi->Spi_Source == SPI)
    {
        SPI_InitConfig(spi->ClkPin,spi->MisoPin,spi->MosiPin,spi->CsPin,spi->BaudRate);
    }
    else if(spi->Spi_Source == QSPI)
    {
        QSPI_InitConfig(spi->ClkPin,spi->MisoPin,spi->MosiPin,spi->CsPin,spi->BaudRate,spi->Mode);
    }
    else
        return 1;
    return 0;
}

uint8_t SPIx_ReadWriteBytes(spix_t *spi,uint8_t *txData, uint8_t *rxData, uint32_t len,sint64_t time_out)
{
    if(time_out == TIME_INFINITE)
        time_out = TIME_INFINITE/len;

    if(spi->Spi_Source == SPI)
    {
        uint32_t start_time = 0;
        uint32_t end_time = 0;
        /* SPI����ṹ�� */
        IfxAsclin_Spi_exchange(&g_SpiConfig[spi->SPIn], txData, rxData, len);
        start_time = Systime.Get_Timeus();
        /* �ȴ��������  */
        while (IfxAsclin_Spi_getStatus(&g_SpiConfig[spi->SPIn]) == IfxAsclin_Spi_Status_busy)
        {
            end_time = Systime.Get_Timeus();
            if((end_time - start_time) > len * time_out)
                break;
        }

    }
    else if(spi->Spi_Source == QSPI)
    {
        uint32_t start_time = 0;
        uint32_t end_time = 0;
        /* ��ʼ���� */
        IfxQspi_SpiMaster_exchange(&g_QspiMasterChannel[spi->SPIn], txData, rxData,(unsigned short)len);
        start_time = Systime.Get_Timeus();
        /* �ȴ��������  */
        while (IfxQspi_SpiMaster_getStatus(&g_QspiMasterChannel[spi->SPIn]) == SpiIf_Status_busy)
        {
            end_time = Systime.Get_Timeus();
            if((end_time - start_time) > len * time_out)
                break;
        }

    }
    else
        return 1;
    return 0;
}

sspi_m SPIx =
{
        .Init = SPIx_Init,
        .ReadWriteBytes = SPIx_ReadWriteBytes,
        .Time_Infinite = SPIx_MAX_TIME_OUT,
};

