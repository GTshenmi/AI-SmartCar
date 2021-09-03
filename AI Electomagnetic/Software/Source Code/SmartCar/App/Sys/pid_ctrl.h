#ifndef _PID_CTRL_H_
#define _PID_CTRL_H_

typedef enum
{
	PositionalPID,IncrementalPID,
}pid_type_t;

typedef struct 
{
	 float Input;
	 float Output;
}pid_gain_t;

typedef struct
{
	 float Kp;
	 float Ki;
	 float Kd;
}pid_param_t;

typedef struct
{
     float Max;
     float Min;
}pid_limit_t;

typedef struct
{
	 float Kp;
	 float Ki;
	 float Kd;
	
	 float PID_Error[3];
	 float IntegralValue;
	 float MaxIntegralValue;
	
	 float Result;

	 pid_limit_t OutputLimit;

	 float MaxResultValue;
	 float MinResultValue;
	
	 pid_gain_t Gain;
	
	 pid_type_t PID_Type;
	 
}PID_TypeDef;

PID_TypeDef PID_Init(pid_type_t PID_Type);

PID_TypeDef PID_SetValue(PID_TypeDef *PID_CtrlStr,pid_param_t PIDValue);

PID_TypeDef PID_SetGain(PID_TypeDef *PID_CtrlStr,pid_gain_t GainValue);

PID_TypeDef PID_SetOutPutLimit(PID_TypeDef *PID_CtrlStr,pid_limit_t Limit);

PID_TypeDef *PID_Ctrl(PID_TypeDef *PID_CtrlStr,float TargetValue,float ActualValue);

pid_limit_t PIDLimit(float Min,float Max);

pid_param_t PIDValue(float Kp,float Ki,float Kd);

pid_gain_t PIDGainValue(float inputgain,float outputgain);

#endif

