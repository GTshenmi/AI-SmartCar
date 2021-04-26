/*
 * parameter.h
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */

#ifndef APP_USR_PARAMETER_H_
#define APP_USR_PARAMETER_H_

#include "sys.h"

#define MAX_DATA_LEN 5

typedef enum
{
    AI_Mode = 0,           /*����������Ѳ��*/
    AutoBoot_Mode = 1,     /*��ǰհ�Զ�Ѳ��ģʽ*/
    ManualBoot_Mode = 2,   /*�ֶ�Ѳ��ģʽ*/
    DebugMode = 3,         /*����*/
}car_mode_t;

typedef enum
{
    AI_Free,AI_Start,AI_Busy,AI_Fin,
}ai_state_t;

typedef struct
{
      uint32_t Cache;        /*ûɶ��*/

      /*For Motor PID*/

      PID_TypeDef M_PID;     /*����ٶȵ�PID������*/

      float Actual_Speed; /*ʵ���ٶ�*/
      float Speed;        /*�ٶ�*/

      /*For Servo PID*/
      PID_TypeDef S_PID;     /*�������ƫ���PID������*/

      float _Bias;
      float Bias;            /*ʵ������ƫ��*/

      uint16_t SPwmValue;
      float Angle;        /*�Ƕ�*/

      uint16_t LADC_Value[MAX_LESENSOR_NUM]; /*ADCֵ*/
      uint16_t SADC_Value[MAX_SESENSOR_NUM]; /*ADCֵ*/

      float N_LADC[MAX_LESENSOR_NUM];/*��һ����ĳ�ǰհ����*/
      float N_SADC[MAX_SESENSOR_NUM];/*��һ����Ķ�ǰհ����*/

      /*For Element*/    
      uint ElementType;  /*����Ԫ������*/
      
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
