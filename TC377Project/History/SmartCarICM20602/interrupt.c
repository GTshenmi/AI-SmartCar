/*
 * interrupt.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include "include.h"

void STM0_CH1_IRQHandler(void) /*Motor Control.*/
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

    data_t *data = &Data[data_pointer];

    float formatedSpeed = 0.0;

    data->Speed = 3500.0;

    formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0;

    data->Actual_Speed = Motor.GetSpeed(Motor.Self);

    Motor.SetSpeed(Motor.Self,formatedSpeed);

    Motor.Update(Motor.Self);
}

void STM1_CH0_IRQHandler(void)  /*Servo Control.*/
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

    data_t *data = &Data[data_pointer];

    if(fabs(data->Bias) >= 20.0)
        data->S_PID.Kp = 2.0 + data->Bias * data->Bias * 0.00372; //待调

    /*动态PID限幅*/
    if(data->S_PID.Kp > 7.2)        //待调
        data->S_PID.Kp = 7.2;

    PID_Ctrl(&data->S_PID,0.0,data->Bias);

    static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};

    static float angle[5] = {0.0};

    data->Angle = (sint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

    Servo.SetAngle(Servo.Self,data->Angle);

    Servo.Update(Servo.Self);
}

void STM1_CH1_IRQHandler(void)          /*Calculate Bias.*/
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

    data_t *data = &Data[data_pointer];

    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);

    /*归一化*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->N_LADC[i] = 100.0 * NormalizeFloat(data->LADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->N_SADC[i] = 100.0 * NormalizeFloat(data->SADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);

     data->Bias = 100.0 * CalculateDistance(data);
}

void CCU60_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */


}

void CCU60_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);



}

void CCU61_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

}

void CCU61_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
}


/*************************************************************************
*  函数名称：PIN_INT0_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断0组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT0_IRQHandler(void)
{
    /* 用户代码 */
}

/*************************************************************************
*  函数名称：PIN_INT1_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断1组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT1_IRQHandler(void)
{
    /* 用户代码 */
}

/*************************************************************************
*  函数名称：PIN_INT2_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断2组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT2_IRQHandler(void)
{
    // 用户代码
}

/*************************************************************************
*  函数名称：PIN_INT3_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断3组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT3_IRQHandler(void)
{
    // 用户代码
    //DMA_CameraStart(PIN_INT2_PRIORITY);

}

/**
  * @brief    QSPI0_RX_IRQHandler中断服务函数
  * @note     QSPI0 接收中断服务函数
  */
void QSPI0_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[0]);

    /* 用户代码 */
}

void QSPI0_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[0]);
    /* 用户代码 */
}

void QSPI0_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[0]);
    /* 用户代码 */
}

void QSPI1_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI1_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI1_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[1]);
    /* 用户代码 */
}

void QSPI2_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI2_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI2_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[2]);
    /* 用户代码 */
}

void QSPI3_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[3]);
    /* 用户代码 */
}

void QSPI3_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[3]);
    /* 用户代码 */
}

void QSPI3_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[3]);
    /* 用户代码 */
}


/**
  * @brief    SPI0_RX_IRQHandler中断服务函数
  * @note     SPI0 接收中断服务函数
  */
void SPI0_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[0]);
    /* 用户代码 */
}

void SPI0_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[0]);
    /* 用户代码 */
}

void SPI0_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[0]);
    /* 用户代码 */
}

void SPI1_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[1]);
    /* 用户代码 */
}

void SPI1_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[1]);
    /* 用户代码 */
}

void SPI1_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[1]);
    /* 用户代码 */
}

void SPI2_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[2]);
    /* 用户代码 */
}

void SPI2_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[2]);
    /* 用户代码 */
}

void SPI2_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[2]);
    /* 用户代码 */
}

void SPI3_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[3]);
    /* 用户代码 */
}

void SPI3_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[3]);
    /* 用户代码 */
}

void SPI3_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[3]);
    /* 用户代码 */
}

void UART0_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[0]);

    /* 用户代码 */
}

void UART0_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[0]);

    /* 用户代码 */
}

void UART0_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[0]);

    /* 用户代码 */
}

void UART1_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART1_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART1_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART2_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART2_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART2_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);

    /* 用户代码 */
}

void UART3_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[3]);

    /* 用户代码 */
}

void UART3_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[3]);

    /* 用户代码 */
}

/**
 * @brief    CPU0_SOFT_IRQ0Handler中断服务函数
 * @note     cpu0 软件中断0中断服务函数
 */
void CPU0_SOFT_IRQ0Handler (void)
{
    SRC_GPSR00.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ1Handler (void)
{
    SRC_GPSR01.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ2Handler (void)
{
    SRC_GPSR02.B.SRR = 0;
    /* 用户代码 */
}

void CPU0_SOFT_IRQ3Handler (void)
{
    SRC_GPSR03.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ0Handler (void)
{
    SRC_GPSR04.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ1Handler (void)
{
    SRC_GPSR05.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ2Handler (void)
{
    SRC_GPSR06.B.SRR = 0;
    /* 用户代码 */
}

void CPU1_SOFT_IRQ3Handler (void)
{
    SRC_GPSR07.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ0Handler (void)
{
    SRC_GPSR10.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ1Handler (void)
{
    SRC_GPSR11.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ2Handler (void)
{
    SRC_GPSR12.B.SRR = 0;
    /* 用户代码 */
}

void CPU2_SOFT_IRQ3Handler (void)
{
    SRC_GPSR13.B.SRR = 0;
    /* 用户代码 */
}
