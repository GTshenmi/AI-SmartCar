/*
 * parameter.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *
 *  @Brief:
 *          This file is for the definition of parameter.
 */
#include <parameter.h>
#include "include.h"

uint16_t CarMode = AutoBoot_Mode;/*运行模式*/
uint16_t data_pointer = 0;

void ParameterInit(void *data)
{
    data_t *pdata = (data_t *)data;

    pdata->DynamicKp = 0.000860;


    /*Set Servo And Motor PID Value.*/
    PID_SetValue(&pdata->M_PID,PIDValue(3.3,0.2,0.0));

    pdata->M_PID.MaxIntegralValue = 100.0;

    PID_SetValue(&pdata->S_PID,PIDValue(0.8,0.0,0.0));

    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));

    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,100.0));

    PID_SetOutPutLimit(&Data[data_pointer].S_PID,PIDLimit(Servo.MinAngle,Servo.MaxAngle));

    PID_SetOutPutLimit(&Data[data_pointer].M_PID,PIDLimit(-100.0,100.0));
}

data_t Data[MAX_DATA_LEN] =
{
        [0] = {
                .Cache = 0,
                .CarMode = AI_Mode,
                .AI_State = AI_Free,
        },
        [1] = {
                .Cache = 0,
                .CarMode = AutoBoot_Mode,
                .AI_State = AI_Free,
        },
        [2] = {
                .Cache = 0,
                .CarMode = ManualBoot_Mode,
                .AI_State = AI_Free,
        },
        [3] = {
                .Cache = 0,
                .CarMode = DebugMode,
                .AI_State = AI_Free,
        },
};

constdata_t CData =
{
        .MaxLADCDeviceNum = MAX_LESENSOR_NUM,
        .MaxSADCDeviceNum = MAX_SESENSOR_NUM,
        .MaxKEYDeviceNum = MAX_KEY_NUM,
};

reserved_data_t ReservedData;
