/*
 * interrupt.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include "include.h"



void STM0_CH1_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

    /* 用户代码 */
    //LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁

}

void STM1_CH0_IRQHandler(void)
{
   // uint32_t SystimeNow = os.time.getTimems();

    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

    data_t *data = &Data[data_pointer];

    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);

    data->Actual_Speed = Motor.GetSpeed(Motor.Self);

    /*归一化*/
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
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

    /* 用户代码 */
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



