/*
 * element.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *          This file is for the special element.
 */
#include <element.h>
#include "include.h"

static void SpecialElementCorrection_AIMode(void *data);
static void SpecialElementCorrection_OtherMode(void *data);
static void SpecialElementCorrection_DebugMode(void *data);
static void CycleGet(void *data);
/*
 * @Brief:����Ԫ�ش���ӿں���
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
    CycleGet(data);
}
void CycleGet(void *data)
{

}
void SpecialElementCorrection_DebugMode(void *data)
{
    SpecialElementCorrection_OtherMode(data);
}

