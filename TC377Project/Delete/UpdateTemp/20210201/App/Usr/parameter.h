/*
 * parameter.h
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */

#ifndef APP_USR_PARAMETER_H_
#define APP_USR_PARAMETER_H_

#include "sys.h"

#define MAX_DATA_LEN 4

typedef enum
{
    AI_Mode = 0,           /*����������Ѳ��*/
    AutoBoot_Mode = 1,     /*��ǰհ�Զ�Ѳ��ģʽ*/
    ManualBoot_Mode = 2,   /*�ֶ�Ѳ��ģʽ*/
    DebugMode = 3,         /*����*/
}car_mode_t;

typedef struct
{
      uint32_t Cache;        /*ûɶ��*/

      PID_TypeDef M_PID;     /*����ٶȵ�PID������*/
      PID_TypeDef S_PID;     /*�������ƫ���PID������*/

      sint16_t Position;     /*ʵ������ƫ��*/

      sint16_t Actual_Speed; /*ʵ���ٶ�*/


      sint16_t Speed;        /*�ٶ�*/
      sint16_t Angle;        /*�Ƕ�*/

      uint16_t LADC_Value[5]; /*ADCֵ*/
      uint16_t SADC_Value[7]; /*ADCֵ*/

      const car_mode_t CarMode;
}data_t;

typedef struct
{
      const uint16_t MaxLADCDeviceNum;
      const uint16_t MaxSADCDeviceNum;
      const uint16_t MaxKEYDeviceNum;

}constdata_t;

extern constdata_t CData;

extern data_t Data[MAX_DATA_LEN];

extern uint16_t data_pointer;
extern uint16_t CarMode;

#endif /* APP_USR_PARAMETER_H_ */