/*
 * task.c
 *
 *  Created on: 2020Äê12ÔÂ7ÈÕ
 *      Author: 936305695
 */
#include "task.h"
#include "softtimer.h"
#include "ui.h"
#include "console.h"

void Task_SoftTimerUpdate(void *argv,uint16_t argc)
{
    for(uint16_t i = 0;i< softTimerNum;i++)
    {
        SoftTimerUpdate((uint16_t)i);
    }
}

void Task_DebugConsole(void *argv,uint16_t argc)
{
    Console.Update(argv,argc);
}

void Task_UiUpdate(void *argv,uint16_t argc)
{
    //UI_Update(argv,argc);
}

void Task_KeyScan(void *argv,uint16_t argc)
{
    for(int i = 0 ; i < 4 ;i++)
    {
        KEY[i].Scan(KEY[i].Self);
    }
}
