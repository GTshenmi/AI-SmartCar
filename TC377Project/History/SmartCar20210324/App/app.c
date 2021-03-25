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
    TIMx.Init(&TIM_Resources[2].TIMN);





    DIPSwitch.Init(DIPSwitch.Self);
    DIPSwitch.SetSwitchNum(DIPSwitch.Self,8);
    //TIMx.Init(&TIM_Resources[1].TIMN);

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.8,0.0,0.0));

    while(1)
    {
        GLED.Toggle(GLED.Self);
        uint32_t bits = DIPSwitch.Read(DIPSwitch.Self);

        Console.WriteLine("Bits = 0x%x",bits);
        os.task.UiUpdate(&UIData,sizeof(UIData));
        os.time.delayms(500);
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{
    TIMx.Init(&TIM_Resources[3].TIMN);

    while(1)
    {
        GLED.Toggle(GLED.Self);
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
    BEEP.ON(BEEP.Self);

    //os.time.delayms(500);

    BEEP.OFF(BEEP.Self);

    while(1);
//    {
//        GLED.Toggle(GLED.Self);
//        os.task.UiUpdate(&UIData,sizeof(UIData));
//    }
}

