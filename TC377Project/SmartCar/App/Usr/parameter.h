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

typedef enum
{
    RASC_Wait,
    RASC_Tracking,
    RASC_Out,
}rightangle_speedctrl;

typedef enum
{
    NoError = 0,
    FileSysInitError,
    BusError,
}err_t;
/* ���� Begin*/
typedef enum
{
    LL_Undefined,
    LL_ExceptionHandler,
    LL_Wait,
    LL_Config,
    LL_Confirm,
    LL_Lose,      //����
    LL_BackSearchLine,
    LL_SearchLine,//����
    LL_Searched   //���ҵ�
}loseline_state_t;

typedef struct
{
    float bias;
}loseline_config;

/* ���� End*/

/* ֱ�� Begin*/
typedef enum
{
    RA_Undefined,
    RA_ExceptionHandler,
    RA_Wait,
    RA_Config,
    RA_Confirm,
    RA_Tracking,
    RA_Out,
}rightangle_state_t;

typedef struct
{
    float bias;
}rightangle_config;

typedef struct
{
    sint32_t Tracking;
}rightangle_cnt;

/* ֱ�� End*/


/* ���� Begin*/

typedef struct
{
    sint32_t Wait;
    sint32_t Confirm;
    sint32_t WaitIn;
    sint32_t In;
    sint32_t Tracking;
    sint32_t Out;
    sint32_t Flags;

}cycle_cnt;

typedef struct
{
    float bias;

    float isCyclePos;

    float waitInDistance;

    float inDistance;

    float dYaw;

    float inCyclePointYaw;

    float sum_l;
    float sum_r;
}cycle_config;

typedef struct
{
    bool isOESensorMaxValue;
    bool isLeftOSensorFall;
    bool isRightOSensorFall;
    bool isMidHSensorFall;

    bool isLeftHSensorFall;
    bool isRightHSensorFall;
}cycle_flag_t;

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
    CC_Config,
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
    CS_ExceptionHandler,
    CS_Wait,
    CS_Config,
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

typedef struct
{
    sint32_t interval;
}cross_config;

typedef struct
{
    sint32_t cnt;
}cross_cnt;
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
    Lost,
    Straight,
    Corner,
    Other
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
    uint Type;
    uint Point;
    bool Lock;
}elementwlock_t;

typedef enum
{
    AI_Mode = 0,           /*����������Ѳ��*/
    LAutoBoot_Mode = 1,     /*��ǰհ�Զ�Ѳ��ģʽ*/
    ManualBoot_Mode = 2,   /*�ֶ�Ѳ��ģʽ*/
    DebugMode = 3,         /*����*/
    SAutoBoot_Mode = 4,    /*��ǰհѲ��*/
    DataCollection = 5,
}car_mode_t;

typedef enum
{
    AI_Free,AI_Start,AI_Busy,AI_Fin,
}ai_state_t;

typedef struct
{
      float    Value;
      float    K;
}esensor_data_t;

typedef struct
{
      /*For Motor*/

      PID_TypeDef M_PID;         /*Motor Fuzzy PID Controler*/
      Fuzzy_TypeDef M_FuzzyKp;
      Fuzzy_TypeDef M_FuzzyKi;

      Fuzzy_TypeDef M_FuzzySpeed;/*Fuzzy Speed Controler*/

      float ActualSpeed;
      float Speed;
      bool  Is_AdjustSpeed;

      /*For Servo*/

      float DynamicKp;
      PID_TypeDef S_PID;        /*Servo PID Controler(Not Use)*/

      Fuzzy_TypeDef S_Fuzzy;    /*Servo Fuzzy Controler*/

//      float _Bias;
      float Bias;               /*Actual Centerline Deviation*/

      uint16_t SPwmValue;

      float NNOutput;

      bool Is_AdjustAngle;

      float Angle;            /*�Ƕ�*/
      float A[10];            /*��ʷʮ�νǶ�*/
      float B[10];            /*��ʷʮ��ƫ��*/

      float CorAngle;
      float AIAngle;
      float Da;

      uint16_t LESensor_SampleValue[MAX_LESENSOR_NUM];  /*��ǰհ����ֵ*/
      uint16_t SESensor_SampleValue[MAX_SESENSOR_NUM];  /*��ǰհ����ֵ*/

      float LESensor_NormalizedValue[MAX_LESENSOR_NUM];
      float SESensor_NormalizedValue[MAX_SESENSOR_NUM];

      esensor_data_t HESensor[4];
      esensor_data_t VESensor[4];
      esensor_data_t OESensor[4];

      float Ke[MAX_SESENSOR_NUM];

      float h_difference;
      float v_difference;
      float o_difference;

      float h_sum;
      float h_suml2;
      float h_sumr2;
      float h_sum2;
      float h_suml3;
      float h_sumr3;
      float h_sum3;
      float h_sum4;
      float v_sum;
      float o_sum;

      float h_bias;
      float v_bias;
      float o_bias;

      bool IsAddNoise;

      /*For Element*/    

      elementwlock_t Element;  /*����Ԫ������*/

      tracking_state_t TrackingState;
      
      cycle_dir_t CycleDir;

      cycle_state_t CycleState;

      float CycleWaitInDistance;
      float CycleInDistance;

      float Err;

      /*State*/

      err_t Error;

      bool UIEnable;

      bool NeedToBack;

      uint AI_State;

      uint CarState;

      const uint CarMode;

      float x;

      float v;

      attitude_t attitude;
      axis_t     acc;
      axis_t     gyro;
      axis_t     mag;

      /*For Debug.*/

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
