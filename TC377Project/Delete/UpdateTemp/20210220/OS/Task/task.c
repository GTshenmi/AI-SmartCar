/*
 * task.c
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */
#include "task.h"
#include "softtimer.h"

void Task_KeyScan(void *argv,uint16_t argc)
{
    for(int i = 0 ; i < 4 ;i++)
    {
        KEY[i].Scan(KEY[i].Self);
    }
}
