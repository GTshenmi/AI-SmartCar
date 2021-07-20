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

void SpecialElementCorrection_AIMode(void *data);
void SpecialElementCorrection_OtherMode(void *data);
void SpecialElementCorrection_DebugMode(void *data);

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementCorrection(void *data)
{
    data_t * usr_data = (data_t *) data;

    switch(usr_data->CarMode)
    {
        case AI_Mode:
            SpecialElementCorrection_AIMode(data);
            break;
        case AutoBoot_Mode:
            SpecialElementCorrection_OtherMode(data);
            break;
        case ManualBoot_Mode:
            SpecialElementCorrection_OtherMode(data);
            break;
        case DebugMode:
            SpecialElementCorrection_DebugMode(data);
            break;
        default:
            break;
    }
}

void SpecialElementCorrection_AIMode(void *data)
{

}

void SpecialElementCorrection_OtherMode(void *data)
{

}

void SpecialElementCorrection_DebugMode(void *data)
{
    SpecialElementCorrection_OtherMode(data);
}

