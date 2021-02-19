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
    MDecisionUnit.Run(MDecisionUnit.Self,&Data[data_pointer],sizeof(data_t));
    SDecisionUnit.Run(SDecisionUnit.Self,&Data[data_pointer],sizeof(data_t));

    SpecialElementCorrection(&Data[data_pointer]);

    MExecutionUnit.Run(MExecutionUnit.Self,&Data[data_pointer],sizeof(data_t));
    SExecutionUnit.Run(SExecutionUnit.Self,&Data[data_pointer],sizeof(data_t));
}
/*
 * @Brief:loop in the interrupt
 * */
void Sensor_InterruptRun()
{
    MSensorUnit.Run(MSensorUnit.Self,&Data[data_pointer],sizeof(data_t));
    SSensorUnit.Run(SSensorUnit.Self,&Data[data_pointer],sizeof(data_t));
}

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    GLED.ON(GLED.Self);

    float input140_0[13] = {13.83,1.65,2.60,17.2,94.0,2.45,2.99,0.22,2.29,5.600000,1.24,3.37,1265.0}; //output = 0
//    float input141_0[13] = {13.05,1.65,2.55,18.0,98.0,2.45 ,2.43,0.29,1.44,4.250000,1.12,2.51,1105.0};//output = 0
//    float input142_1[13] = {12.43,1.53,2.29,21.5,86.0,2.74,3.15,0.39,1.77,3.940000,0.69,2.84,352.0};  //output = 1

    sint16_t output = 0;



    while(1)
    {
        GLED.Toggle(GLED.Self);

        output = TrackingModelRun(input140_0);

        CUART.WriteLine(CUART.Self,0,"output = %d",output);

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
        //NeuralNetworkReasoning(&Data[data_pointer]);

        os.task.KeyScan(NULL,0);

        os.time.delayms(20);
    }
}
/*
 * @Brief:CPU2 Main Func
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    data_t *pdata = &Data[data_pointer];
    uint16_t size = sizeof(Data[data_pointer]);
    while(1)
    {
        os.task.SoftTimerUpdate(pdata,size);

        os.task.KeyScan(pdata,size);

        os.task.UiUpdate(pdata,size);

        os.task.DebugConsole(pdata,size);
    }
}

