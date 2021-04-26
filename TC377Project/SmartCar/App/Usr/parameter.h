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

      /*For Motor PID*/

      PID_TypeDef M_PID;     /*电机速度的PID控制器*/

      float Actual_Speed; /*实际速度*/
      float Speed;        /*速度*/

      /*For Servo PID*/
      PID_TypeDef S_PID;     /*舵机中线偏差的PID控制器*/

      float _Bias;
      float Bias;            /*实际中线偏差*/

      uint16_t SPwmValue;
      float Angle;        /*角度*/

      uint16_t LADC_Value[MAX_LESENSOR_NUM]; /*ADC值*/
      uint16_t SADC_Value[MAX_SESENSOR_NUM]; /*ADC值*/

      float N_LADC[MAX_LESENSOR_NUM];/*归一化后的长前瞻参数*/
      float N_SADC[MAX_SESENSOR_NUM];/*归一化后的短前瞻参数*/

      /*For Element*/    
      uint ElementType;  /*赛道元素类型*/
      
      esensor_queue_t Queue;

      /*State*/

      uint AI_State;

      uint CarState;

      const uint CarMode;

      /*For Debug.*/

      uint8_t ReportSensorData;

      uint8_t ReportMotorData;

      uint8_t ReportServoData;

      float TSpeed;
      float ASpeed;
      uint16_t MPwmValue;

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
