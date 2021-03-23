/*
 * app.c
 *
 *  Created on: 2020��12��6��
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

    GLED.ON(GLED.Self);

    //os.time.delayms(2000);

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.8,0.0,0.0));

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
    KEY[0].SetShield(KEY[0].Self,true);

    uint32_t times = 0;

    //os.time.delayms(300);

    while(1)
    {
        times++;

        if(!(times% 50))
            BLED.Toggle(BLED.Self);

        os.task.KeyScan(NULL,0);

        os.time.delayms(20);
    }
}

/*
 * @Brief:CPU2 Main Func ��ʱ���� ���ܻṩ�粻��
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    BEEP.ON(BEEP.Self);

    //os.time.delayms(500);

    BEEP.OFF(BEEP.Self);

    while(1)
    {
       //  os.task.SoftTimerUpdate(NULL,0);

       //  os.task.KeyScan(NULL,0);

       //  os.task.DebugConsole(NULL,0);

       //  os.time.delayms(20);
    }
}

