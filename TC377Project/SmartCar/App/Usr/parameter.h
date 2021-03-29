/*
 * parameter.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef APP_USR_PARAMETER_H_
#define APP_USR_PARAMETER_H_

#include "sys.h"

#define MAX_DATA_LEN 5

typedef enum
{
    AI_Mode = 0,           /*神经网络引导巡线*/
    AutoBoot_Mode = 1,     /*长前瞻自动巡线模式*/
    ManualBoot_Mode = 2,   /*手动巡线模式*/
    DebugMode = 3,         /*调试*/
}car_mode_t;

typedef enum
{
    AI_Free,AI_Start,AI_Busy,AI_Fin,
}ai_state_t;

typedef struct
{
      uint32_t Cache;        /*没啥用*/

      PID_TypeDef M_PID;     /*电机速度的PID控制器*/
      PID_TypeDef S_PID;     /*舵机中线偏差的PID控制器*/

      float _Bias;
      float Bias;            /*实际中线偏差*/

      sint16_t Actual_Speed; /*实际速度*/

      uint16_t SPwmValue;


      sint16_t Speed;        /*速度*/
      sint16_t Angle;        /*角度*/
      uint ElementType;  /*赛道元素类型*/

      uint16_t LADC_Value[5]; /*ADC值*/
      uint16_t SADC_Value[7]; /*ADC值*/

      float N_LADC[5];/*归一化后的长前瞻参数*/
      float N_SADC[7];/*归一化后的短前瞻参数*/

      uint AI_State;

      uint CarState;

      const uint CarMode;

      uint8_t ReportSensorData;

      uint8_t ReportMotorData;

      uint8_t ReportServoData;
}data_t;

typedef struct
{
      const uint16_t MaxLADCDeviceNum;
      const uint16_t MaxSADCDeviceNum;
      const uint16_t MaxKEYDeviceNum;

}constdata_t;

void ParameterInit(void *data);

char *GetCarState(uint index);
char *GetCarMode(uint index);
char *GetAIState(uint index);
char *GetElementType(uint index);

extern constdata_t CData;

extern data_t Data[MAX_DATA_LEN];

extern uint16_t data_pointer;
extern uint16_t CarMode;

#endif /* APP_USR_PARAMETER_H_ */
