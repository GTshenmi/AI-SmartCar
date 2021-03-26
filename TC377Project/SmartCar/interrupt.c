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

    data->Bias = 100.0 * CalculateDistance(data);

    PID_Ctrl(&data->S_PID,0,data->Bias);

    data->Angle = (sint16_t)(-data->S_PID.Result);

    data->Speed = 2500;

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



