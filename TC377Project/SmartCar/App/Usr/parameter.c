/*
 * parameter.c
 *
 *  Created on: 2020-12-06
 *      Author: 936305695
 *
 *  @Brief:
 *          This file is for the definition of parameter.
 */
#include <parameter.h>
#include "include.h"

uint16_t CarMode =DebugMode;/*运行状态*/
//uint16_t CarMode = AI_Mode; //调AI 换成此状态

uint16_t data_pointer = 0;

float ESensorDataQueue[100][8];
float RBiasDataQueue[100];
float EBiasDataQueue[100];
float TrackingDataQueue[100];
float ElementDataQueue[100];

float HESensorDataQueue[100][4];
float VESensorDataQueue[100][2];
float OESensorDataQueue[100][2];

float HBiasQueue[100];
float VBiasQueue[100];
float OBiasQueue[100];

void ParameterInit(void *argv)
{
    data_t *data = (data_t *)argv;

    data->DynamicKp = 0.000860;

    data->CarState = false;

    data->TrackingState = Normal_Tracking;

    data->Is_AdjustAngle = false;
    data->Is_AdjustSpeed = false;

    /*Set Servo And Motor PID Value.*/
    PID_SetValue(&data->M_PID,PIDValue(3.3,0.2,0.0));

    data->M_PID.MaxIntegralValue = 100.0;

    data->Speed = 3200.0;

    PID_SetValue(&data->S_PID,PIDValue(2.227,0.0,0.0));

    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));

    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,100.0));

    PID_SetOutPutLimit(&Data[data_pointer].S_PID,PIDLimit(Servo.MinAngle,Servo.MaxAngle));

    PID_SetOutPutLimit(&Data[data_pointer].M_PID,PIDLimit(-100.0,100.0));

    FuzzyPIDInit(&data->M_FuzzyKp,&data->M_FuzzyKi);

    Queue.Init(&data->ESensorQueue,&ESensorDataQueue[0][0],8);

    Queue.Init(&data->RawBiasQueue,&RBiasDataQueue[0],1);
    Queue.Init(&data->ElementBiasQueue,&EBiasDataQueue[0],1);
    Queue.Init(&data->TrackingQueue,&TrackingDataQueue[0],1);
    Queue.Init(&data->ElementTypeQueue,&TrackingDataQueue[0],1);
    
    Queue.Init(&data->HESensorQueue,&HESensorDataQueue[0][0],4);
    Queue.Init(&data->VESensorQueue,&VESensorDataQueue[0][0],2);
    Queue.Init(&data->OESensorQueue,&OESensorDataQueue[0][0],2);

    Queue.Init(&data->HBiasQueue,&HBiasQueue[0],1);
    Queue.Init(&data->VBiasQueue,&VBiasQueue[0],1);
    Queue.Init(&data->OBiasQueue,&OBiasQueue[0],1);

}

data_t Data[MAX_DATA_LEN] =
{
        [0] = {
                .CarMode = AI_Mode,
                .AI_State = AI_Free,
        },
        [1] = {
                .CarMode = LAutoBoot_Mode,
                .AI_State = AI_Free,
        },
        [2] = {
                .CarMode = ManualBoot_Mode,
                .AI_State = AI_Free,
        },
        [3] = {
                .CarMode = DebugMode,
                .AI_State = AI_Free,
        },
        [4] = {
                .CarMode = SAutoBoot_Mode,
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
