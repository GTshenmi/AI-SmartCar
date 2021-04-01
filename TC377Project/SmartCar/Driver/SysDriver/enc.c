/*
 * enc.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <enc.h>

uint8_t ENCx_Init(encx_t *enc)
{
    ENC_InputPin_t InputPin = enc->InputPin;
    ENC_DirPin_t DirPin = enc->DirPin;

    //初始化全局时钟
    IfxGpt12_enableModule(&MODULE_GPT120);
    if(ENC_GetTimer(InputPin) != ENC_GetTimer(DirPin))
    {
  #pragma warning 557         // 屏蔽警告
      while(1); //请检查 ENC脉冲输入管脚和方向管脚是否为同一个 ENC
  #pragma warning default     // 打开警告
    }
    switch(ENC_GetTimer(InputPin))
    {
    case 2:
      //设置T2 计数模式
      IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
      //设置T2 计数模式   TxIn 上升沿计数
      IfxGpt12_T2_setIncrementalInterfaceInputMode(&MODULE_GPT120, IfxGpt12_IncrementalInterfaceInputMode_bothEdgesTxIN);
      IfxGpt12_T2_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
      //设置T2 辅助管脚 A
      IfxGpt12_T2_setEudInput(&MODULE_GPT120, (IfxGpt12_EudInput)(DirPin & 0x0f));
      IfxGpt12_T2_setInput(&MODULE_GPT120, (IfxGpt12_Input)(InputPin & 0x0f));
      //计数方向  使用外部管脚控制
      IfxGpt12_T2_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
      //设置向上计数
      IfxGpt12_T2_setTimerDirection(&MODULE_GPT120,IfxGpt12_TimerDirection_up);
      IfxGpt12_T2_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
      break;
    case 3:
      //设置T3 计数模式
      IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
      //设置T2 计数模式   TxIn 上升沿计数
      IfxGpt12_T3_setIncrementalInterfaceInputMode(&MODULE_GPT120, IfxGpt12_IncrementalInterfaceInputMode_bothEdgesTxIN);
      IfxGpt12_T3_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
      //设置T2 辅助管脚 A
      IfxGpt12_T3_setEudInput(&MODULE_GPT120, (IfxGpt12_EudInput)(DirPin & 0x0f));
      IfxGpt12_T3_setInput(&MODULE_GPT120, (IfxGpt12_Input)(InputPin & 0x0f));
      //计数方向  使用外部管脚控制
      IfxGpt12_T3_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
      //设置向上计数
      IfxGpt12_T3_setTimerDirection(&MODULE_GPT120,IfxGpt12_TimerDirection_up);
      IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
      break;
    case 4:
      //设置T4 计数模式
      IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
      //设置T4 计数模式   TxIn 上升沿计数
      IfxGpt12_T4_setIncrementalInterfaceInputMode(&MODULE_GPT120, IfxGpt12_IncrementalInterfaceInputMode_bothEdgesTxIN);
      IfxGpt12_T4_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
      //设置T4 辅助管脚 A
      IfxGpt12_T4_setEudInput(&MODULE_GPT120, (IfxGpt12_EudInput)(DirPin & 0x0f));
      IfxGpt12_T4_setInput(&MODULE_GPT120, (IfxGpt12_Input)(InputPin & 0x0f));
      //计数方向  使用外部管脚控制
      IfxGpt12_T4_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
      //设置向上计数
      IfxGpt12_T4_setTimerDirection(&MODULE_GPT120,IfxGpt12_TimerDirection_up);
      IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
      break;
    case 5:
      //设置T5计数模式
      IfxGpt12_T5_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
      //设置T5 计数模式   TxIn 上升沿计数
      IfxGpt12_T5_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
      //设置T5 辅助管脚 A
      IfxGpt12_T5_setEudInput(&MODULE_GPT120, (IfxGpt12_EudInput)(DirPin & 0x0f));
      IfxGpt12_T5_setInput(&MODULE_GPT120, (IfxGpt12_Input)(InputPin & 0x0f));
      //计数方向  使用外部管脚控制
      IfxGpt12_T5_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
      //设置向上计数
      IfxGpt12_T5_setTimerDirection(&MODULE_GPT120,IfxGpt12_TimerDirection_up);
      IfxGpt12_T5_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
      break;
    case 6:
      //设置T6 计数模式
      IfxGpt12_T6_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
      //设置T6 计数模式   TxIn 上升沿计数
      IfxGpt12_T6_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
      //设置T6 辅助管脚 A
      IfxGpt12_T6_setEudInput(&MODULE_GPT120, (IfxGpt12_EudInput)(DirPin & 0x0f));
      IfxGpt12_T6_setInput(&MODULE_GPT120, (IfxGpt12_Input)(InputPin & 0x0f));
      //计数方向  使用外部管脚控制
      IfxGpt12_T6_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
      //设置向上计数
      IfxGpt12_T6_setTimerDirection(&MODULE_GPT120,IfxGpt12_TimerDirection_up);
      IfxGpt12_T6_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
      break;
    }

    return 0;
}
sint16_t ENCx_Read(encx_t *enc)
{
    sint16_t value = 0;

    switch(ENC_GetTimer(enc->InputPin))
    {
        case 2:
            value = (signed short)MODULE_GPT120.T2.U;
            MODULE_GPT120.T2.U = 0;
            break;

        case 3:
          value = (signed short)MODULE_GPT120.T3.U;
          MODULE_GPT120.T3.U = 0;
          break;

        case 4:
          value = (signed short)MODULE_GPT120.T4.U;
          MODULE_GPT120.T4.U = 0;
          break;

        case 5:
          value = (signed short)MODULE_GPT120.T5.U;
          MODULE_GPT120.T5.U = 0;
          break;

        case 6:
          value = (signed short)MODULE_GPT120.T6.U;
          MODULE_GPT120.T6.U = 0;
          break;
    }

    return value;
}

senc_m ENCx =
{
        .Init = ENCx_Init,
        .Read = ENCx_Read,
};



