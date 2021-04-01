/*
 * spi.c
 *
 *  Created on: 2020��12��13��
 *      Author: 936305695
 */
#include "spi.h"

/*
 *
 * SPI:
 * *  ����˵����
   * @param    clkPin      �� SPI  clk�ܽ�
   * @param    misoPin     :  SPI  MISO�ܽ�
   * @param    misoPin     :  SPI  MOSI�ܽ�
   * @param    misoPin     :  SPI  CS�ܽ�
   * @param    baudrate    �� SPI ������   ע�����10M
*  ��    ע������ʵ���ǽ�Asc������SPI�����ʹ����Asc0����SPI0����ôAsc0 �Ͳ�����������0
*
* QSPI:
* * @param    clkPin      �� QSPI  clk�ܽ�
  * @param    misoPin     :  QSPI  MISO�ܽ�
  * @param    misoPin     :  QSPI  MOSI�ܽ�
  * @param    misoPin     :  QSPI  CS�ܽ�
  * @param    baudrate    �� QSPI ������
  * @param      mode            0: CLK����ʱ��Ϊ�͵�ƽ �ڵ�һ��CLK������ ��������
  * @param      mode            1: CLK����ʱ��Ϊ�͵�ƽ �ڵڶ���CLK������ ��������
  * @param      mode            2: CLK����ʱ��Ϊ�ߵ�ƽ �ڵ�һ��CLK������ ��������
  * @param      mode            3: CLK����ʱ��Ϊ�ߵ�ƽ �ڵڶ���CLK������ ��������
 * */
uint8_t SPIx_Init(spix_t *spi)
{
    if(spi->Spi_Source == SPI)
    {
        unsigned char spiNum = SPI_GetModule(spi->ClkPin);

        IfxAsclin_Sclk_Out * IfxAsclin_Sclk = IfxAsclin_Sclk_Out_pinTable[spiNum][SPI_GetIndex(spi->ClkPin)];
        IfxAsclin_Rx_In    * IfxAsclin_MISO = IfxAsclin_Rx_In_pinTable[spiNum][SPI_GetIndex(spi->MisoPin)];
        IfxAsclin_Tx_Out   * IfxAsclin_MOSI = IfxAsclin_Tx_Out_pinTable[spiNum][SPI_GetIndex(spi->MosiPin)];
        IfxAsclin_Slso_Out * IfxAsclin_CS   = IfxAsclin_Slso_Out_pinTable[spiNum][SPI_GetIndex(spi->CsPin)];


        /* ���ж�  */
        boolean              interruptState = IfxCpu_disableInterrupts();

        /* create module config */
        IfxAsclin_Spi_Config spiConfig;
        IfxAsclin_Spi_initModuleConfig(&spiConfig, IfxAsclin_Sclk->module);

        spiConfig.frame.shiftDir        = IfxAsclin_ShiftDirection_msbFirst;/* MSB ��ǰ */

        /* set the desired baudrate */
        spiConfig.baudrate.prescaler    = 1;
        spiConfig.baudrate.baudrate     = (float)spi->BaudRate;                 /* spi �ٶ� */
        spiConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_4;

        spiConfig.inputOutput.cpol      = IfxAsclin_ClockPolarity_idleHigh; /* clk ����״̬�ߵ�ƽ */

        /* ISR priorities and interrupt target */
        spiConfig.interrupt.rxPriority    = SpiIrqPriority[spiNum * 3];     /* see also \ref IfxLld_Asclin */
        spiConfig.interrupt.txPriority    = SpiIrqPriority[spiNum * 3 + 1]; /* see also \ref IfxLld_Asclin */
        spiConfig.interrupt.erPriority    = SpiIrqPriority[spiNum * 3 + 2]; /* see also \ref IfxLld_Asclin */
        spiConfig.interrupt.typeOfService = (IfxSrc_Tos)SpiIrqVectabNum[spiNum];

        /* pin configuration */
        const IfxAsclin_Spi_Pins pins = {
                IfxAsclin_Sclk, IfxPort_OutputMode_pushPull, /* Clock out pin */
                IfxAsclin_MISO,   IfxPort_InputMode_pullUp,    /* Rx pin */
                IfxAsclin_MOSI,   IfxPort_OutputMode_pushPull, /* Tx pin */
                IfxAsclin_CS, IfxPort_OutputMode_pushPull, /* Slave select pin */
            IfxPort_PadDriver_cmosAutomotiveSpeed1
        };
        spiConfig.pins = &pins;

        /* initialize module */
        IfxAsclin_Spi_initModule(&g_SpiConfig[spiNum], &spiConfig);

        //���գ����ͺʹ����ж�����
        IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3],     (unsigned long)SpiIrqPriority[spiNum * 3]);
        IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3 + 1], (unsigned long)SpiIrqPriority[spiNum * 3 + 1]);
        IfxCpu_Irq_installInterruptHandler((void*)SpiIrqFuncPointer[spiNum * 3 + 2], (unsigned long)SpiIrqPriority[spiNum * 3 + 2]);

        /* enable interrupts again */
        IfxCpu_restoreInterrupts(interruptState);
    }
    else if(spi->Spi_Source == QSPI)
    {
        unsigned char QSPINum   = QSPI_GetModule(spi->ClkPin);

        IfxQspi_Sclk_Out   * QSPI_Sclk = IfxQspi_Sclk_Out_pinTable[QSPINum][QSPI_GetIndex(spi->ClkPin)];
        IfxQspi_Mrst_In    * QSPI_MISO = IfxQspi_Mrst_In_pinTable[QSPINum][QSPI_GetIndex(spi->MisoPin)];
        IfxQspi_Mtsr_Out   * QSPI_MOSI = IfxQspi_Mtsr_Out_pinTable[QSPINum][QSPI_GetIndex(spi->MosiPin)];
        IfxQspi_Slso_Out   * QSPI_CS   = IfxQspi_Slso_Out_pinTable[QSPINum][QSPI_GetIndex(spi->CsPin)][0];

        IfxQspi_SpiMaster_Config        spiMasterConfig;
        IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;


        {
            /* create module config */
            IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, IfxQspi_getAddress((IfxQspi_Index)QSPINum));

            /* set the maximum baudrate */
            spiMasterConfig.base.maximumBaudrate = 50000000;

            /* ISR priorities and interrupt target */
            spiMasterConfig.base.rxPriority  = QSpiIrqPriority[QSPINum * 3];
            spiMasterConfig.base.txPriority  = QSpiIrqPriority[QSPINum * 3 + 1];
            spiMasterConfig.base.erPriority  = QSpiIrqPriority[QSPINum * 3 + 2];
            spiMasterConfig.base.isrProvider = (IfxSrc_Tos)QSpiIrqVectabNum[QSPINum];

            /* pin configuration */
            const IfxQspi_SpiMaster_Pins pins = {QSPI_Sclk,                               /* SCLK */
                                                 IfxPort_OutputMode_pushPull,
                                                 QSPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                                 QSPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
                                                 IfxPort_PadDriver_cmosAutomotiveSpeed3                   /* pad driver mode */
            };
            spiMasterConfig.pins = &pins;

            /* initialize module */
            IfxQspi_SpiMaster_initModule(&g_QSPIConfig[QSPINum], &spiMasterConfig);
        }

        {
            /* create channel config */
            IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
                &g_QSPIConfig[QSPINum]);




            const IfxQspi_SpiMaster_Output slsOutput = {QSPI_CS,
                                                        IfxPort_OutputMode_pushPull,
                                                        IfxPort_PadDriver_cmosAutomotiveSpeed1};

            spiMasterChannelConfig.sls.output.pin    = slsOutput.pin;
            spiMasterChannelConfig.sls.output.mode   = slsOutput.mode;
            spiMasterChannelConfig.sls.output.driver = slsOutput.driver;
            spiMasterChannelConfig.base.mode.csInactiveDelay = 1;
            spiMasterChannelConfig.base.mode.csLeadDelay     = 10;                    /* CSǰ����ʱ */
            spiMasterChannelConfig.base.mode.csTrailDelay    = 1;
            spiMasterChannelConfig.base.mode.dataHeading = SpiIf_DataHeading_msbFirst;  /* MSB��ǰ      */
            spiMasterChannelConfig.base.mode.dataWidth = 8;                             /* 8bit     */
            spiMasterChannelConfig.base.baudrate = (float)spi->BaudRate;                     /* �����ٶ�    */
            spiMasterChannelConfig.base.mode.csActiveLevel = Ifx_ActiveState_low;       /* CSΪ����Ч*/

            if(spi->Mode / 2)
            {
                spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;  //ʱ�ӿ���ʱΪ�ߵ�ƽ
            }
            else
            {
                spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;   //ʱ�ӿ���ʱΪ�͵�ƽ
            }

            if(spi->Mode % 2)
            {
                spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;  //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
            }
            else
            {
                spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;  //����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
            }
            //���գ����ͺʹ����ж�����
            IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3],     (unsigned long)QSpiIrqPriority[QSPINum * 3]);
            IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3 + 1], (unsigned long)QSpiIrqPriority[QSPINum * 3 + 1]);
            IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3 + 2], (unsigned long)QSpiIrqPriority[QSPINum * 3 + 2]);


            IfxQspi_SpiMaster_initChannel(&g_QspiMasterChannel[QSPINum],
                &spiMasterChannelConfig);
        }
    }
    else
        return 1;
    return 0;
}

uint8_t SPIx_ReadWriteBytes(spix_t *spi,uint8_t *txData, uint8_t *rxData, uint32_t len,sint64_t time_out)
{
//    if(time_out == TIME_INFINITE)
//        time_out = TIME_INFINITE/len;

    if(spi->Spi_Source == SPI)
    {
        /* SPI����ṹ�� */
        IfxAsclin_Spi_exchange(&g_SpiConfig[spi->SPIn], txData, rxData, len);

        /* �ȴ��������  */
        while (IfxAsclin_Spi_getStatus(&g_SpiConfig[spi->SPIn]) == IfxAsclin_Spi_Status_busy);
    }
    else if(spi->Spi_Source == QSPI)
    {
        /* ��ʼ���� */
        IfxQspi_SpiMaster_exchange(&g_QspiMasterChannel[spi->SPIn], txData, rxData,(unsigned short)len);

        /* �ȴ��������  */
        while (IfxQspi_SpiMaster_getStatus(&g_QspiMasterChannel[spi->SPIn]) == SpiIf_Status_busy);

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

