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
    LCD_Init(&LCD_Type);

    while(1)
    {
        for(uint8_t i = 0 ; i < 50 ;i++)
            _LCD_DrawPoint(i,i,0x0000);
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

//    BLED.ON(BLED.Self);
//
//    char input[8] = {0};
//
//    for(int i = 0 ; i < 8 ;i++)
//    {
//        input[i] = (char)(8 - i);
//    }
//
//    int16_t angle;
//
//    model_info_struct Model_Info_Model1;
//    Model_GetInfo(model1, &Model_Info_Model1);
//    Model_Run(model1,input,&angle);
//
//    BLED.OFF(BLED.Self);
//
//    CUART.WriteLine(CUART.Self,0,"angle = %d",angle);


    KEY[0].PressedCallBack = Key_PressedCallBack;
    KEY[1].PressedCallBack = Key_PressedCallBack;
    KEY[2].PressedCallBack = Key_PressedCallBack;
    KEY[3].PressedCallBack = Key_PressedCallBack;

    while(1)
    {

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

