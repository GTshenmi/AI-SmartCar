/*
 * element.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *          This file is for the special element.
 */
#include <element.h>
#include "include.h"

void LossLine_Handler(data_t *data);
void Cycle_Handler(data_t *data);
void Cross_Handler(data_t *data);
void Normal_Tracking(data_t *data);

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementCorrection(void *data)
{
    //data_t * usr_data = (data_t *) data;

    if(0)
    {
        Cycle_Handler(data);
    }
    else if(0)
    {
        LossLine_Handler(data);
    }
    else if(0)
    {
        Cross_Handler(data);
    }
    else
    {
        Normal_Tracking(data);
    }
}

void Normal_Tracking(data_t *data)
{
    
}

void Cross_Handler(data_t *data)
{

}

void Cycle_Handler(data_t *data)
{

}

void LossLine_Handler(data_t *data)
{

}
