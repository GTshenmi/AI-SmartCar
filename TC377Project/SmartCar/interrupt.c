/*
 * interrupt.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */
#include "include.h"

void STM0_CH1_IRQHandler(void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

    /* �û����� */
    //LED_Ctrl(LED1,RVS);        //��ƽ��ת,LED��˸

}

void STM1_CH0_IRQHandler(void)
{
   // uint32_t SystimeNow = os.time.getTimems();

    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

    data_t *data = &Data[data_pointer];
//
//    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
//        data->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
//    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
//        data->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);
//
//    data->Actual_Speed = Motor.GetSpeed(Motor.Self);
//
//    /*��һ��*/
//    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
//        data->N_LADC[i] = 100.0 * NormalizeFloat(data->LADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
//    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
//        data->N_SADC[i] = 100.0 * NormalizeFloat(data->SADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
//
//    static float Kb[33] = {
//       0.004310506862, 0.006396806333,-0.008907295763,-0.006022877526,  0.01509535592,
//       0.002957194578, -0.02246881649,  0.00418050494,   0.0303422641, -0.01742442511,
//       -0.03784247488,  0.04116194695,  0.04404707253, -0.09106717259, -0.04814339057,
//         0.3107235134,    0.545322597,   0.3107235134, -0.04814339057, -0.09106717259,
//        0.04404707253,  0.04116194695, -0.03784247488, -0.01742442511,   0.0303422641,
//        0.00418050494, -0.02246881649, 0.002957194578,  0.01509535592,-0.006022877526,
//      -0.008907295763, 0.006396806333, 0.004310506862
//    };
//
//    static float dis[33] = {0.0};
//
//    data->_Bias = 100.0 * CalculateDistance(data);
//
//    data->Bias = FIR_Filter(Kb,dis,data->_Bias,33);

    //data->Bias = 100.0 * CalculateDistance(data);

    if(fabs(data->Bias) >= 20.0)
        data->S_PID.Kp = 2.0 + data->Bias * data->Bias * 0.00372; //����

    /*��̬PID�޷�*/
    if(data->S_PID.Kp > 7.2)        //����
        data->S_PID.Kp = 7.2;

    PID_Ctrl(&data->S_PID,0.0,data->Bias);

    static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};

    static float angle[5] = {0.0};

    data->Angle = (sint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

    data->Speed = 3500;

    Servo.SetAngle(Servo.Self,data->Angle);

    Motor.SetSpeed(Motor.Self,data->Speed);

    Motor.Update(Motor.Self);
    Servo.Update(Servo.Self);

    //uint32_t dt = os.time.getTimems() - SystimeNow;

    //Console.WriteLine("Time = %ld",dt);
}

void STM1_CH1_IRQHandler(void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

    data_t *data = &Data[data_pointer];

    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
            data->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
            data->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);

    data->Actual_Speed = Motor.GetSpeed(Motor.Self);

    /*��һ��*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->N_LADC[i] = 100.0 * NormalizeFloat(data->LADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->N_SADC[i] = 100.0 * NormalizeFloat(data->SADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
//
//        static float Kb[33] = {
//           0.004310506862, 0.006396806333,-0.008907295763,-0.006022877526,  0.01509535592,
//           0.002957194578, -0.02246881649,  0.00418050494,   0.0303422641, -0.01742442511,
//           -0.03784247488,  0.04116194695,  0.04404707253, -0.09106717259, -0.04814339057,
//             0.3107235134,    0.545322597,   0.3107235134, -0.04814339057, -0.09106717259,
//            0.04404707253,  0.04116194695, -0.03784247488, -0.01742442511,   0.0303422641,
//            0.00418050494, -0.02246881649, 0.002957194578,  0.01509535592,-0.006022877526,
//          -0.008907295763, 0.006396806333, 0.004310506862
//        };
//
//        static float dis[33] = {0.0};
//
     data->Bias = 100.0 * CalculateDistance(data);

        //data->Bias = FIR_Filter(Kb,dis,data->_Bias,33);




    /* �û����� */
    //    SSU.Run(SSU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    //    MSU.Run(MSU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    //
    //    MDU.Run(MDU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    //    SDU.Run(SDU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    //
    //    SEU.Run(SEU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    //    MEU.Run(MEU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
}

void CCU60_CH0_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* �û����� */


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
