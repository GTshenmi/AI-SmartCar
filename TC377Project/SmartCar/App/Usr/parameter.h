/*
 * parameter.h
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */

#ifndef APP_USR_PARAMETER_H_
#define APP_USR_PARAMETER_H_

#include "sys.h"
#include "fuzzycontrol.h"

#define MAX_DATA_LEN 5

#define Lock(x)  ((x).Lock = 1)
#define pLock(x) ((x)->Lock == 1)
#define Unlock(x) ((x).Lock = 0)
#define pUnlock(x) ((x)->Lock = 0) 

#define IsLocked(x) ((x).Lock)

#define SetValueWLock(x,data,value) \
    do                              \
    {                               \
        if(!IsLocked(x))             \
        {                           \
            x.data = value;         \
        }                           \
    }while(0);

/* ���� Begin*/
typedef enum
{
    LL_Undefined,
    LL_Wait,
    LL_Lose,      //����
    LL_BackSearchLine,
    LL_SearchLine,//����
    LL_Searched   //���ҵ�
}loseline_state_t;
/* ���� End*/

/* ֱ�� Begin*/
typedef enum
{
    RA_Undefined,
    RA_Wait,
    RA_Confirm,
    RA_Tracking,
    RA_Out,
}rightangle_state_t;
/* ֱ�� End*/


/* ���� Begin*/
typedef enum
{
    CC_DirUndefined,
    CC_DirLeft,
    CC_DirRight,
}cycle_dir_t;

typedef enum
{
    CC_Undefined,
    CC_Exception_Handler,

    CC_Wait,
    CC_Confirm,
    CC_WaitIn,

    CC_In,
    CC_Tracking,
    CC_Out,
}cycle_state_t;
/* ���� Begin*/

/* ʮ�� Begin*/
typedef enum
{
    CS_Undefined,
    CS_Wait,
    CS_Confirm,


    CS_In,
    CS_Tracking,
    CS_Out,
}cross_state_t;

typedef enum
{
    CS_UndefinedIn,
    CS_MidIn,
    CS_LeftIn,
    CS_RightIn,
}incross_attitude_t;

typedef enum
{
    CS_UndefinedInfo,
    CS_Left,
    CS_Right,
}cross_info_t;
/* ʮ�� Begin*/


typedef enum
{
    LoseLine = 1,               //����
    Normal_Tracking = 2,        //����Ѱ��
}tracking_state_t;

typedef enum
{
    None = 0,
    RightAngle, //ֱ����
    Cross,      //ʮ��
    Cycle,
    Ramp,
}element_t;

typedef enum
{
    NoException,

    CC_MisJudge,

    CC_Err,

    CC_SlantIn,

    RA_To_CC,

}element_exception_e;

typedef struct
{
    uint EXT;

    uint CC;

    uint RA;

    float Info[10][10];

}element_exception_t;

typedef struct
{
    uint Type;
    element_exception_t Exception;
    bool Lock;
}elementwlock_t;

typedef enum
{
    AI_Mode = 0,           /*����������Ѳ��*/
    LAutoBoot_Mode = 1,     /*��ǰհ�Զ�Ѳ��ģʽ*/
    ManualBoot_Mode = 2,   /*�ֶ�Ѳ��ģʽ*/
    DebugMode = 3,         /*����*/
    SAutoBoot_Mode = 4,    /*��ǰհѲ��*/
}car_mode_t;

typedef enum
{
    AI_Free,AI_Start,AI_Busy,AI_Fin,
}ai_state_t;

typedef struct
{
      /*For Motor PID*/

      PID_TypeDef M_PID;     /*����ٶȵ�PID������*/
      Fuzzy_TypeDef M_FuzzyKp;
      Fuzzy_TypeDef M_FuzzyKi;

      Fuzzy_TypeDef FuzzySpeed;

      float Actual_Speed; /*ʵ���ٶ�*/
      float Speed;        /*�ٶ�*/
      bool  Is_AdjustSpeed;

      /*For Servo PID*/

      float DynamicKp;
      PID_TypeDef S_PID;     /*�������ƫ���PID������*/

      fuzzy_ctrl_t S_Fuzzy;

      float _Bias;
      float Bias;            /*ʵ������ƫ��*/

      uint16_t SPwmValue;

      bool Is_AdjustAngle;
      float Angle;        /*�Ƕ�*/

      uint16_t LESensor_SampleValue[MAX_LESENSOR_NUM]; /*ADCֵ*/
      uint16_t SESensor_SampleValue[MAX_SESENSOR_NUM]; /*ADCֵ*/

      float LESensor_NormalizedValue[MAX_LESENSOR_NUM];/*��һ����ĳ�ǰհ����*/
      float SESensor_NormalizedValue[MAX_SESENSOR_NUM];/*��һ����Ķ�ǰհ����*/

      float H_ESensorValue[4];
      float V_ESensorValue[2];
      float O_ESensorValue[2];

      float Ke[MAX_SESENSOR_NUM];

      float h_difference;
      float v_difference;
      float o_difference;

      float h_sum;
      float v_sum;
      float o_sum;

      float h_bias;
      float v_bias;
      float o_bias;

      bool IsAddNoise;

      /*For Element*/    

      elementwlock_t Element;  /*����Ԫ������*/

      elementwlock_t LastElement;  /*����Ԫ������*/

      tracking_state_t TrackingState;
      
      queue_t ESensorQueue;//ESensor
      queue_t RawBiasQueue;//Raw Bias
      queue_t ElementBiasQueue;//Element Bias
      queue_t TrackingQueue;//LoseLine
      queue_t ElementTypeQueue;//Element

      queue_t HESensorQueue;
      queue_t VESensorQueue;
      queue_t OESensorQueue;

      queue_t HBiasQueue;
      queue_t VBiasQueue;
      queue_t OBiasQueue;

      /*State*/

      bool NeedToBack;

      uint AI_State;

      uint CarState;

      const uint CarMode;

      float x;

      float v;

      /*For Debug.*/

      float *eSensorData;

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
