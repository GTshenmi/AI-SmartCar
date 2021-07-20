/*
 * app.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"
/*
 * @Brief:loop in the interrupt
 * */
void Ctrl_InterruptRun()
{
    MDecisionUnit.Run(MDecisionUnit.Self,&Data[data_pointer]);
    MDecisionUnit.Run(SDecisionUnit.Self,&Data[data_pointer]);

    SpecialElementCorrection(&Data[data_pointer]);

    MExecutionUnit.Run(MExecutionUnit.Self,&Data[data_pointer]);
    SExecutionUnit.Run(SExecutionUnit.Self,&Data[data_pointer]);
}
/*
 * @Brief:loop in the interrupt
 * */
void Sensor_InterruptRun()
{
    MSensorUnit.Run(MSensorUnit.Self,&Data[data_pointer]);
    SSensorUnit.Run(SSensorUnit.Self,&Data[data_pointer]);
}

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    GLED.ON(GLED.Self);
    while(1)
    {
        GLED.Toggle(GLED.Self);

        os.time.delayms(500);
    }
}
/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{
    BLED.ON(BLED.Self);
    while(1)
    {
        BLED.Toggle(BLED.Self);
        NeuralNetworkReasoning(&Data[data_pointer]);
    }
}
/*
 * @Brief:CPU2 Main Func
 *  This Core is for Debug.
 * */
void Core2_Main()
{

    while(1)
    {
        os.task.SoftTimerUpdate();

        os.task.KeyScan();

        os.task.UiUpdate();

        os.task.DebugConsole();
    }
}

