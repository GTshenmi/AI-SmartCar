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

float TSpeedArray[MPIDRecordLen];
float ASpeedArrayPID[MPIDRecordLen];
float ASpeedArrayFuzzyPID[MPIDRecordLen];

uint16_t data_pointer = 0;

void *app_data_pointer = NULL;

void FuzzyControlInit(data_t *data);

void ParameterInit(void *argv)
{
    data_t *data = (data_t *)argv;

    uint32_t bits = DIPSwitch.Read(DIPSwitch.Self);

    app_data_pointer = (void *)data;

    data->Error = NoError;

    data->DynamicKp = 0.000860;

    data->CarState = false;

    data->UIEnable = true;

    data->Speed = 2000;

    data->TrackingState = Normal_Tracking;

    data->CycleInDistance = 20.0;
    data->CycleWaitInDistance = 80.0;
    data->Err = 0.0;

    data->Is_AdjustAngle = false;
    data->Is_AdjustSpeed = false;

    /*Set Servo And Motor PID Value.*/
    PID_SetValue(&data->M_PID,PIDValue(3.6,0.5,0.0));

    data->M_PID.MaxIntegralValue = 1.2;

    data->Speed = 3200.0;

    PID_SetValue(&data->S_PID,PIDValue(2.227,0.0,0.0));

    PID_SetGain(&data->S_PID,PIDGainValue(1.0,1.0));

    PID_SetGain(&data->M_PID,PIDGainValue(1.0,100.0));

    PID_SetOutPutLimit(&data->S_PID,PIDLimit(Servo.MinAngle,Servo.MaxAngle));

    PID_SetOutPutLimit(&data->M_PID,PIDLimit(-100.0,100.0));

    FuzzyPIDInit(&data->M_FuzzyKp,&data->M_FuzzyKi);

    FuzzyControlInit(data);

    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
    {
        data->LESensorGain[i] = LESensor[i].Gain;
    }

    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
    {
        data->SESensorGain[i] = SESensor[i].Gain;
    }

    switch((bits >> 4) & 0x00000003)
    {
        case 0:

            break;

        case 1:

            LoadDataFromEeprom();

            for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
            {
                LESensor[i].EnableGain(LESensor[i].Self,true);
                LESensor[i].SetGain(LESensor[i].Self,data->LESensorGain[i]);
            }

            for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
            {
                SESensor[i].EnableGain(SESensor[i].Self,true);
                SESensor[i].SetGain(SESensor[i].Self,data->SESensorGain[i]);
            }

            Console.WriteArray("float",data->SESensorGain,8);

            break;

        case 2:

            LoadDataFromEeprom();

            for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
            {
                LESensor[i].EnableGain(LESensor[i].Self,true);
                LESensor[i].SetGain(LESensor[i].Self,data->LESensorGain[i]);
            }

            for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
            {
                SESensor[i].EnableGain(SESensor[i].Self,true);
                SESensor[i].SetGain(SESensor[i].Self,data->SESensorGain[i]);
            }

            break;

        case 3:

            LoadDataFromEeprom();

            SelfCalibration(data,CalibrationLHOESensor);

            os.time.delay(0.5,s);

            SelfCalibration(data,CalibrationSHOESensor);

            os.time.delay(0.5,s);

            SelfCalibration(data,CalibrationLVESensor);

            os.time.delay(0.5,s);

            SelfCalibration(data,CalibrationSVESensor);

            os.time.delay(0.5,s);

            break;


    }

    SelfCalibration(data,CalibrationIMU);
}

void FuzzyControlInit(data_t *data)
{
    if(data->CarMode == LAutoBoot_Mode)
    {
        for(int i = 0 ; i < 7 ; i++)
        {
            for(int j = 0; j <  7 ; j++)
            {
                SFuzzyRule[i][j] = LABMSFuzzyRule[i][j];
            }
        }
    }
    else
    {
        for(int i = 0 ; i < 7 ; i++)
        {
            for(int j = 0; j <  7 ; j++)
            {
                SFuzzyRule[i][j] = SABMSFuzzyRule[i][j];
            }
        }
    }
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
