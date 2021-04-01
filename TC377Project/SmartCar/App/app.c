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

    GLED.ON(GLED.Self);

    //NNTest();

    while(1)
    {
        GLED.Toggle(GLED.Self);
        for(int i = 0 ; i < 65536 ; i++)
        {
            for(int j = 0 ; j < 200 ; j++)
            {

            }
        }
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{

    while(1)
    {
        os.task.UiUpdate(&UIData,sizeof(UIData));
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

    uint32_t times = 0;

    while(1)
    {
        times++;

        if(!(times% 50))
            BLED.Toggle(BLED.Self);

        os.task.KeyScan(NULL,0);
        os.task.SoftTimerUpdate(NULL,0);
        os.task.DebugConsole(NULL,0);

        os.time.delayms(20);
    }
}

