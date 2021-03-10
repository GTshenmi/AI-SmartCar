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
    MDU.Run(MDU.Self,&Data[data_pointer],sizeof(data_t));
    SDU.Run(SDU.Self,&Data[data_pointer],sizeof(data_t));

    SpecialElementCorrection(&Data[data_pointer]);

    MEU.Run(MEU.Self,&Data[data_pointer],sizeof(data_t));
    SEU.Run(SEU.Self,&Data[data_pointer],sizeof(data_t));
}
/*
 * @Brief:loop in the interrupt
 * */
void Sensor_InterruptRun()
{
    MSU.Run(MSU.Self,&Data[data_pointer],sizeof(data_t));
    SSU.Run(SSU.Self,&Data[data_pointer],sizeof(data_t));
}

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    GLED.ON(GLED.Self);

//    uint8_t input[7];
//
//    for(uint8_t i = 0 ; i < 8 ; i++)
//    {
//        input[i] = 7 * i;
//    }
//
//    model_info_struct Model1;
//    Model_GetInfo(model1, &Model1);
//
//    sint16_t angle = 0;
//
//    Model_Run(model1,input,&angle);



    while(1)
    {
        GLED.Toggle(GLED.Self);
        os.time.delayms(200);
    }
}


void Key_PressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0;i<4;i++)
    {
        if(self == KEY[i].Self)
            CUART.WriteLine(CUART.Self,0,"KEY[%d] Pressed",i);
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
        os.time.delayms(1000);
    }
}
/*
 * @Brief:CPU2 Main Func
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    for(int i = 0 ; i < 4 ; i ++)
        KEY[i].PressedCallBack = Key_PressedCallBack;
    while(1)
    {
        ;
//        os.task.SoftTimerUpdate(NULL,0);
//
//        os.task.KeyScan(NULL,0);
//
//        os.task.UiUpdate(&UIData,sizeof(UIData));
//
//        os.task.DebugConsole(NULL,0);
    }
}

