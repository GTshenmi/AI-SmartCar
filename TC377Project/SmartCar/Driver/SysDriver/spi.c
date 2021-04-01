/*
 * spi.c
 *
 *  Created on: 2020年12月13日
 *      Author: 936305695
 */
#include "spi.h"

/*
 *
 * SPI:
 * *  参数说明：
   * @param    clkPin      ： SPI  clk管脚
   * @param    misoPin     :  SPI  MISO管脚
   * @param    misoPin     :  SPI  MOSI管脚
   * @param    misoPin     :  SPI  CS管脚
   * @param    baudrate    ： SPI 波特率   注意最高10M
*  备    注：这里实际是将Asc口用作SPI，如果使用了Asc0当作SPI0，那么Asc0 就不可用作串口0
*
* QSPI:
* * @param    clkPin      ： QSPI  clk管脚
  * @param    misoPin     :  QSPI  MISO管脚
  * @param    misoPin     :  QSPI  MOSI管脚
  * @param    misoPin     :  QSPI  CS管脚
  * @param    baudrate    ： QSPI 波特率
  * @param      mode            0: CLK空闲时刻为低电平 在第一个CLK跳边沿 捕获数据
  * @param      mode            1: CLK空闲时刻为低电平 在第二个CLK跳边沿 捕获数据
  * @param      mode            2: CLK空闲时刻为高电平 在第一个CLK跳边沿 捕获数据
  * @param      mode            3: CLK空闲时刻为高电平 在第二个CLK跳边沿 捕获数据
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


        /* 关中断  */
        boolean              interruptState = IfxCpu_disableInterrupts();

        /* create module config */
        IfxAsclin_Spi_Config spiConfig;
        IfxAsclin_Spi_initModuleConfig(&spiConfig, IfxAsclin_Sclk->module);

        spiConfig.frame.shiftDir        = IfxAsclin_ShiftDirection_msbFirst;/* MSB 在前 */

        /* set the desired baudrate */
        spiConfig.baudrate.prescaler    = 1;
        spiConfig.baudrate.baudrate     = (float)spi->BaudRate;                 /* spi 速度 */
        spiConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_4;

        spiConfig.inputOutput.cpol      = IfxAsclin_ClockPolarity_idleHigh; /* clk 空闲状态高电平 */

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

        //接收，发送和错误中断配置
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
            spiMasterChannelConfig.base.mode.csLeadDelay     = 10;                    /* CS前置延时 */
            spiMasterChannelConfig.base.mode.csTrailDelay    = 1;
            spiMasterChannelConfig.base.mode.dataHeading = SpiIf_DataHeading_msbFirst;  /* MSB在前      */
            spiMasterChannelConfig.base.mode.dataWidth = 8;                             /* 8bit     */
            spiMasterChannelConfig.base.baudrate = (float)spi->BaudRate;                     /* 设置速度    */
            spiMasterChannelConfig.base.mode.csActiveLevel = Ifx_ActiveState_low;       /* CS为低有效*/

            if(spi->Mode / 2)
            {
                spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;  //时钟空闲时为高电平
            }
            else
            {
                spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;   //时钟空闲时为低电平
            }

            if(spi->Mode % 2)
            {
                spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;  //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
            }
            else
            {
                spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;  //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
            }
            //接收，发送和错误中断配置
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
        /* SPI传输结构体 */
        IfxAsclin_Spi_exchange(&g_SpiConfig[spi->SPIn], txData, rxData, len);

        /* 等待传输结束  */
        while (IfxAsclin_Spi_getStatus(&g_SpiConfig[spi->SPIn]) == IfxAsclin_Spi_Status_busy);
    }
    else if(spi->Spi_Source == QSPI)
    {
        /* 开始传输 */
        IfxQspi_SpiMaster_exchange(&g_QspiMasterChannel[spi->SPIn], txData, rxData,(unsigned short)len);

        /* 等待传输结束  */
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

