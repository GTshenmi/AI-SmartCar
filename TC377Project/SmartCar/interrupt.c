/*
 * interrupt.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */
#include "include.h"

attitude_t attitude;
axis_t acc,gyro;
float dt = 0.0;

void STM0_CH1_IRQHandler(void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

    //BEEP.Toggle(BEEP.Self);

    // //dt = os.time.getTime(ms);

    // ICM20602_ReadAcc(&acc.x,&acc.y,&acc.z);

    // ICM20602_ReadGyro(&gyro.x,&gyro.y,&gyro.z);

    // //dt = os.time.getTime(ms) - dt;

    // AttitudeUpdate(&acc,&gyro,NULL,&attitude);
}

void STM1_CH0_IRQHandler(void)           /*Calculate Bias.*/
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

    data_t *data = &Data[data_pointer];

    GetESensorData(&Data[data_pointer]);

    ESensorDataProcess(&Data[data_pointer]);

    ElementDetermine(&Data[data_pointer]);

    SpecialElementHandler(&Data[data_pointer]);

//    float trackingState = data->TrackingState * 1.0;
//    EQueue.Puts(&data->EQueue,data->LESensor_NormalizedValue,0,7,false);
//    EQueue.Puts(&data->EQueue,&data->Bias,7,8,false);
//    EQueue.Puts(&data->EQueue,&trackingState,8,9,true);
}

void STM1_CH1_IRQHandler(void)       /*Servo Control.*/
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

    AngleControl(&Data[data_pointer]);
}

void CCU60_CH0_IRQHandler (void) /*Motor Control.*/
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* �û����� */
    SpeedControl(&Data[data_pointer]);
}

void CCU60_CH1_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);
}

void CCU61_CH0_IRQHandler(void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

}

void CCU61_CH1_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* �û����� */
}


/*************************************************************************
*  �������ƣ�PIN_INT0_IRQHandler�жϷ�����
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע���ⲿ�ж�0��ܽ� ʹ�õ��жϷ�����
*************************************************************************/
void PIN_INT0_IRQHandler(void)
{
    /* �û����� */
}

/*************************************************************************
*  �������ƣ�PIN_INT1_IRQHandler�жϷ�����
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע���ⲿ�ж�1��ܽ� ʹ�õ��жϷ�����
*************************************************************************/
void PIN_INT1_IRQHandler(void)
{
    /* �û����� */
}

/*************************************************************************
*  �������ƣ�PIN_INT2_IRQHandler�жϷ�����
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע���ⲿ�ж�2��ܽ� ʹ�õ��жϷ�����
*************************************************************************/
void PIN_INT2_IRQHandler(void)
{
    // �û�����
}

/*************************************************************************
*  �������ƣ�PIN_INT3_IRQHandler�жϷ�����
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע���ⲿ�ж�3��ܽ� ʹ�õ��жϷ�����
*************************************************************************/
void PIN_INT3_IRQHandler(void)
{
    // �û�����
    //DMA_CameraStart(PIN_INT2_PRIORITY);

}

/**
  * @brief    QSPI0_RX_IRQHandler�жϷ�����
  * @note     QSPI0 �����жϷ�����
  */
void QSPI0_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[0]);

    /* �û����� */
}

void QSPI0_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[0]);
    /* �û����� */
}

void QSPI0_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[0]);
    /* �û����� */
}

void QSPI1_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[1]);
    /* �û����� */
}

void QSPI1_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[1]);
    /* �û����� */
}

void QSPI1_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[1]);
    /* �û����� */
}

void QSPI2_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[2]);
    /* �û����� */
}

void QSPI2_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[2]);
    /* �û����� */
}

void QSPI2_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[2]);
    /* �û����� */
}

void QSPI3_RX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[3]);
    /* �û����� */
}

void QSPI3_TX_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[3]);
    /* �û����� */
}

void QSPI3_ER_IRQHandler(void)
{
    IfxQspi_SpiMaster_isrError(&g_QSPIConfig[3]);
    /* �û����� */
}


/**
  * @brief    SPI0_RX_IRQHandler�жϷ�����
  * @note     SPI0 �����жϷ�����
  */
void SPI0_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[0]);
    /* �û����� */
}

void SPI0_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[0]);
    /* �û����� */
}

void SPI0_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[0]);
    /* �û����� */
}

void SPI1_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[1]);
    /* �û����� */
}

void SPI1_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[1]);
    /* �û����� */
}

void SPI1_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[1]);
    /* �û����� */
}

void SPI2_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[2]);
    /* �û����� */
}

void SPI2_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[2]);
    /* �û����� */
}

void SPI2_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[2]);
    /* �û����� */
}

void SPI3_RX_IRQHandler(void)
{
    IfxAsclin_Spi_isrReceive(&g_SpiConfig[3]);
    /* �û����� */
}

void SPI3_TX_IRQHandler(void)
{
    IfxAsclin_Spi_isrTransmit(&g_SpiConfig[3]);
    /* �û����� */
}

void SPI3_ER_IRQHandler(void)
{
    IfxAsclin_Spi_isrError(&g_SpiConfig[3]);
    /* �û����� */
}

void UART0_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[0]);

    /* �û����� */
}

void UART0_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[0]);

    /* �û����� */
}

void UART0_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[0]);

    /* �û����� */
}

void UART1_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[1]);

    /* �û����� */
}

void UART1_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

    /* �û����� */
}

void UART1_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[1]);

    /* �û����� */
}

void UART2_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[2]);

    /* �û����� */
}

void UART2_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[2]);

    /* �û����� */
}

void UART2_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[2]);

    /* �û����� */
}

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);

    /* �û����� */
}

void UART3_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[3]);

    /* �û����� */
}

void UART3_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[3]);

    /* �û����� */
}

/**
 * @brief    CPU0_SOFT_IRQ0Handler�жϷ�����
 * @note     cpu0 ����ж�0�жϷ�����
 */
void CPU0_SOFT_IRQ0Handler (void)
{
    SRC_GPSR00.B.SRR = 0;
    /* �û����� */
}

void CPU0_SOFT_IRQ1Handler (void)
{
    SRC_GPSR01.B.SRR = 0;
    /* �û����� */
}

void CPU0_SOFT_IRQ2Handler (void)
{
    SRC_GPSR02.B.SRR = 0;
    /* �û����� */
}

void CPU0_SOFT_IRQ3Handler (void)
{
    SRC_GPSR03.B.SRR = 0;
    /* �û����� */
}

void CPU1_SOFT_IRQ0Handler (void)
{
    SRC_GPSR04.B.SRR = 0;
    /* �û����� */
}

void CPU1_SOFT_IRQ1Handler (void)
{
    SRC_GPSR05.B.SRR = 0;
    /* �û����� */
}

void CPU1_SOFT_IRQ2Handler (void)
{
    SRC_GPSR06.B.SRR = 0;
    /* �û����� */
}

void CPU1_SOFT_IRQ3Handler (void)
{
    SRC_GPSR07.B.SRR = 0;
    /* �û����� */
}

void CPU2_SOFT_IRQ0Handler (void)
{
    SRC_GPSR10.B.SRR = 0;
    /* �û����� */
}

void CPU2_SOFT_IRQ1Handler (void)
{
    SRC_GPSR11.B.SRR = 0;
    /* �û����� */
}

void CPU2_SOFT_IRQ2Handler (void)
{
    SRC_GPSR12.B.SRR = 0;
    /* �û����� */
}

void CPU2_SOFT_IRQ3Handler (void)
{
    SRC_GPSR13.B.SRR = 0;
    /* �û����� */
}
