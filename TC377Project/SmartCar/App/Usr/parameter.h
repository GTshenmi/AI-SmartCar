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

#define Lock(x)  ((x).Lock = 1)
#define pLock(x) ((x)->Lock == 1)
#define Unlock(x) ((x).Lock = 0)
#define pUnlock(x) ((x)->Lock = 0) 

#define IsLocked(x) ((x).Lock)

#define SetValueWLock(x,data,value) \
    do                              \
    {                               \
        if(!Is_Lock(x))             \
        {                           \
            x.data = value;         \
        }                           \
    }while(0);                      \



typedef enum
{
    LL_Undefined,
    LL_Wait,
    LL_Lose,      //����
    LL_SearchLine,//����
    LL_Searched   //���ҵ�
}loseline_state_t;

typedef enum
{
    CC_Undefined,
    CC_Wait,
    CC_Confirm,
    CC_In,
    CC_Tracking,
    CC_Out,
}cycle_state_t;

typedef enum
{
    LoseLine = 1,               //����
    Normal_Tracking = 2,        //����Ѱ��
}tracking_state_t;

typedef enum
{
    RightAngle, //ֱ����
    Cross,      //ʮ��
    Cycle,
    None,
}element_t;

typedef struct
{
    element_t Type;

    bool Lock;
}elementwlock_t;

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

      float DynamicKp;
      PID_TypeDef S_PID;     /*�������ƫ���PID������*/

      float _Bias;
      float Bias;            /*ʵ������ƫ��*/

      uint16_t SPwmValue;
      float Angle;        /*�Ƕ�*/

      uint16_t LESensor_SampleValue[MAX_LESENSOR_NUM]; /*ADCֵ*/
      uint16_t SESensor_SampleValue[MAX_SESENSOR_NUM]; /*ADCֵ*/

      float LESensor_NormalizedValue[MAX_LESENSOR_NUM];/*��һ����ĳ�ǰհ����*/
      float SESensor_NormalizedValue[MAX_SESENSOR_NUM];/*��һ����Ķ�ǰհ����*/

      float H_ESensorValue[3];
      float V_ESensorValue[2];
      float O_ESensorValue[2];

      float h_difference;
      float v_difference;
      float o_difference;

      float h_sum;
      float v_sum;
      float o_sum;

      float h_bias;
      float v_bias;
      float o_bias;

      /*For Element*/    
      elementwlock_t Element;  /*����Ԫ������*/

      tracking_state_t HTrackingState;
      tracking_state_t VTrackingState;
      tracking_state_t TrackingState;
      
      esensor_queue_t EQueue;

      /*State*/

      uint AI_State;

      uint CarState;

      const uint CarMode;

      /*For Debug.*/

      DebugDefine(float,TSpeed);
      DebugDefine(float,ASpeed);
      DebugDefine(uint16_t,MPwmValue);

      DebugDefine(float,VBias);
      DebugDefine(float,HBias);
      DebugDefine(float,Weight);
}data_t;

typedef struct
{
      const uint16_t MaxLADCDeviceNum;
      const uint16_t MaxSADCDeviceNum;
      const uint16_t MaxKEYDeviceNum;

}constdata_t;

typedef struct
{
     char D[100];
}reserved_data_t;

void ParameterInit(void *data);

extern reserved_data_t ReservedData;

extern constdata_t CData;

extern data_t Data[MAX_DATA_LEN];

extern uint16_t data_pointer;
extern uint16_t CarMode;

#endif /* APP_USR_PARAMETER_H_ */
