/*
 * interrupt.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include "include.h"

void STM1_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

    SSU.Run(SSU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    MSU.Run(MSU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));

    MDU.Run(MDU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    SDU.Run(SDU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));

    SEU.Run(SEU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));
    MEU.Run(MEU.Self,&Data[data_pointer],sizeof(Data[data_pointer]));

    //BLED.Toggle(BLED.Self);

    //Console.WriteLine("STM1 CH0 INT END.");
    //Screen.WriteXLine(Screen.Self,7,"STM1 CH0 INT END.");

}

