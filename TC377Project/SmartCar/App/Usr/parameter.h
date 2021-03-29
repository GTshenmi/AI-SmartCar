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

      PID_TypeDef M_PID;     /*����ٶȵ�PID������*/
      PID_TypeDef S_PID;     /*�������ƫ���PID������*/

      float _Bias;
      float Bias;            /*ʵ������ƫ��*/

      sint16_t Actual_Speed; /*ʵ���ٶ�*/

      uint16_t SPwmValue;


      sint16_t Speed;        /*�ٶ�*/
      sint16_t Angle;        /*�Ƕ�*/
      uint ElementType;  /*����Ԫ������*/

      uint16_t LADC_Value[5]; /*ADCֵ*/
      uint16_t SADC_Value[7]; /*ADCֵ*/

      float N_LADC[5];/*��һ����ĳ�ǰհ����*/
      float N_SADC[7];/*��һ����Ķ�ǰհ����*/

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
