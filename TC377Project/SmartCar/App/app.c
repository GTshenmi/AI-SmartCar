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


model_info_struct Model1_Info;
sint16_t angle = 0;
sint16_t servo_value = 0;
sint16_t temp = 0;
sint8_t ad_data[7] = {-124,-107,-59,-14,30,-117,1};

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    TIMx.Init(&TIM_Resources[1].TIMN);

    Screen.WriteXLine(Screen.Self,1,"Core0 Run To Main.");
    Console.WriteLine("Core0 Run To Main.");

    GLED.ON(GLED.Self);

    Model_GetInfo(model1, &Model1_Info);

    Model_Run(model1,ad_data,&temp);

    //根据模型信息对转角进行位移
    servo_value = temp >> (Model1_Info.quant_bits - Model1_Info.frac_bits - 1);

    //训练模型时，转角是被压缩到了-128至127， 压缩时使用的公式为*128/1651，所以这里需要*1651/128进行还原，然后再用于转角控制
    angle = (int16_t)(servo_value * (int32_t)1651 / 128);

    Screen.WriteXLine(Screen.Self,3,"Core1 Begin While");
    Console.WriteLine("Core0 Begin While.");

   // os.time.delayms(500);

    //Screen.Clear(Screen.Self,WHITE);

    while(1)
    {
        GLED.Toggle(GLED.Self);
        os.task.UiUpdate(&UIData,sizeof(UIData));
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{
    Screen.WriteXLine(Screen.Self,2,"Core1 Init Finished.");
    Console.WriteLine("Core1 Run To Main.");

    KEY[0].SetShield(KEY[0].Self,true);

    Screen.WriteXLine(Screen.Self,4,"Core1 Begin While");
    Console.WriteLine("Core1 Begin While.");

    //Screen.Clear(Screen.Self,WHITE);

    //os.time.delayms(500);

    uint32_t times = 0;



    while(1)
    {
        times++;

        if(times % 50)
            BLED.Toggle(BLED.Self);

        os.task.KeyScan(NULL,0);

        os.time.delayms(20);
    }
}

/*
 * @Brief:CPU2 Main Func 暂时不用 可能会供电不足
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    //Screen.WriteXLine(Screen.Self,2,"Core2 Init Finished.");

    //Screen.WriteXLine(Screen.Self,5,"Core2 Begin While.");
    while(1)
    {
     //   os.task.SoftTimerUpdate(NULL,0);

       // os.task.KeyScan(NULL,0);

      //  os.task.DebugConsole(NULL,0);

      //  os.time.delayms(20);

    }
}

