/*
 * parameter.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *
 *  @Brief:
 *          This file is for the definition of parameter.
 */
#include <parameter.h>
#include "include.h"

uint16_t CarMode = AutoBoot_Mode;/*����ģʽ*/
uint16_t data_pointer = 0;

char *CarStateDictionary[] = {\
                          "Stop",\
                          "Running"};

char *CarModeDictionary[] = { \
                          "AI",\
                          "Auto Boot",\
                          "Manual Boot",\
                          "Debug"\
};

char *AIStateDictionary[] =\
{
    "Free",\
    "Start",\
    "Busy",\
    "Fin"\
};

char *ElementTypeDictionary[] =\
{
    "NULL",\
    "NULL",\
    "NULL",\
    "NULL"\
};

void ParameterInit(void *data)
{
    data_t *pdata = (data_t *)data;

    /*Set Servo And Motor PID Value.*/
    PID_SetValue(&pdata->M_PID,PIDValue(1.0,0.0,0.0));
    PID_SetValue(&pdata->S_PID,PIDValue(5.2,0.0,0.0));
}

char *GetCarState(uint index)
{
    return CarStateDictionary[index];
}

char *GetCarMode(uint index)
{
    return CarModeDictionary[index];
}

char *GetAIState(uint index)
{
    return AIStateDictionary[index];
}

char *GetElementType(uint index)
{
    return ElementTypeDictionary[index];
}

data_t Data[MAX_DATA_LEN] =
{
        [0] = {
                .Cache = 0,
                .CarMode = AI_Mode,
                .ReportMotorData = 0,
                .ReportSensorData = 0,
                .ReportServoData = 0,
                .AI_State = AI_Free,
        },
        [1] = {
                .Cache = 0,
                .CarMode = AutoBoot_Mode,
                .ReportMotorData = 0,
                .ReportSensorData = 0,
                .ReportServoData = 0,
                .AI_State = AI_Free,
        },
        [2] = {
                .Cache = 0,
                .CarMode = ManualBoot_Mode,
                .ReportMotorData = 0,
                .ReportSensorData = 0,
                .ReportServoData = 0,
                .AI_State = AI_Free,
        },
        [3] = {
                .Cache = 0,
                .CarMode = DebugMode,
                .ReportMotorData = 0,
                .ReportSensorData = 0,
                .ReportServoData = 0,
                .AI_State = AI_Free,
        },
};

constdata_t CData =
{
        .MaxLADCDeviceNum = MAX_LESENSOR_NUM,
        .MaxSADCDeviceNum = MAX_SESENSOR_NUM,
        .MaxKEYDeviceNum = MAX_KEY_NUM,
};

uint SaveParameterSD(uint16_t *LADC_Value,uint16_t *SADC_Value,sint16_t *Angle){
    char buffer[50];
    char* bufferPointer = buffer;

    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%u ",LADC_Value[i]);
    }
    bufferPointer += sprintf(bufferPointer,"\n");

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%u ",SADC_Value[i]);
    }
    bufferPointer += sprintf(bufferPointer,"\n");

    bufferPointer += sprintf(bufferPointer,"%d",*Angle);
    bufferPointer += sprintf(bufferPointer,"\n");

    return SD.fastWrite("Parameter.txt",buffer);

}