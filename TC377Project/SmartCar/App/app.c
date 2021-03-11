/*
 * app.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    GLED.ON(GLED.Self);

    Screen.WriteXLine(Screen.Self,1,"Core0");

    //Servo.SetPwmValue(Servo.Self,850);
    //Servo.SetPwmCentValue(Servo.Self,1750);
    //Servo.SetAngle(Servo.Self,0);

    //Servo.SetPwmValue(Servo.Self,700);

    //Motor.SetPwmValue(Motor.Self,2000);


    Screen.WriteXLine(Screen.Self,1,"Core0 Begin While");

    while(1)
    {
        for(int i = 0 ; i < CData.MaxLADCDeviceNum;i++)
        {
            Data[data_pointer].LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
        }
        for(int i = 0 ; i < CData.MaxSADCDeviceNum;i++)
        {
            Data[data_pointer].SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);
        }

        Screen.Clear(Screen.Self,WHITE);

        os.task.UiUpdate(&UIData,sizeof(UIData));

        os.time.delayms(200);
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{
    Screen.WriteXLine(Screen.Self,2,"Core1");

    KEY[0].SetShield(KEY[0].Self,true);
    BLED.ON(BLED.Self);

    Screen.WriteXLine(Screen.Self,2,"Core1 Begin While");

    while(1)
    {
        os.task.KeyScan(NULL,0);
        BLED.Toggle(BLED.Self);
        os.time.delayms(20);
    }
}

/*
 * @Brief:CPU2 Main Func 暂时不用 可能会供电不足
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    Screen.WriteXLine(Screen.Self,3,"Core2");

    while(1)
    {
        os.task.SoftTimerUpdate(NULL,0);

       // os.task.KeyScan(NULL,0);

        os.task.DebugConsole(NULL,0);

        os.time.delayms(20);

    }
}

