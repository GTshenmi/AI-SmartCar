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
        },
        [1] = {
                .Cache = 0,
                .CarMode = AutoBoot_Mode,
        },
        [2] = {
                .Cache = 0,
                .CarMode = ManualBoot_Mode,
        },
        [3] = {
                .Cache = 0,
                .CarMode = DebugMode,
        },
};

constdata_t CData =
{
        .MaxLADCDeviceNum = 5,
        .MaxSADCDeviceNum = 7,
        .MaxKEYDeviceNum = 4,
};
