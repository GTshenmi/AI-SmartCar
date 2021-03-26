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


void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0 ; i < 6 ; i++)
    {
        if(self == KEY[i].Self)
            Screen.WriteXLine(Screen.Self,0,"KEY[%d] Pressed.",i);
    }
}
/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    //TIMx.Init(&TIM_Resources[1].TIMN);

    //Screen.Init(Screen.Self);

    GLED.ON(GLED.Self);

    //Screen.Test(Screen.Self);

    while(1)
    {
        GLED.Toggle(GLED.Self);

        uint32_t SystimeNow = os.time.getTimems();

        os.task.UiUpdate(&UIData,sizeof(UIData));

        uint32_t dt = os.time.getTimems() - SystimeNow;

        Screen.WriteXLine(Screen.Self,17,"Time = %lu",dt);
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{
    uint32_t times = 0;

    data_t *data = &Data[data_pointer];

    while(1)
    {
        times++;

        if(!(times% 50))
            BLED.Toggle(BLED.Self);

        os.task.KeyScan(NULL,0);

        for(int i = 0 ; i < 5;i++)
            Console.WriteLine("AD[%d] = %u",i,data->LADC_Value[i]);

        Console.WriteLine("Angle = %d",data->Angle);

        os.time.delayms(20);
    }
}

/*
 * @Brief:CPU2 Main Func 暂时不用 可能会供电不足
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    BEEP.ON(BEEP.Self);

    BEEP.OFF(BEEP.Self);

    while(1);
//    {
//        GLED.Toggle(GLED.Self);
//        os.task.UiUpdate(&UIData,sizeof(UIData));
//    }
}

