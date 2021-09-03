#include "pid_ctrl.h"
#include "math.h"
#include "sysmath.h"

void PID_CtrlDemo()
{
#if 1
    PID_TypeDef PID_CtrlStr = PID_Init(PositionalPID);

    PID_SetValue(&PID_CtrlStr,PIDValue(2.0,0.0,1.0));

    PID_SetGain(&PID_CtrlStr,PIDGainValue(10.0,10.0));
#else 
    PID_TypeDef PID_CtrlStr = PID_Init(PositionalPID);

    PID_CtrlStr.Kp = 2.0;
    PID_CtrlStr.Ki = 0.0;
    PID_CtrlStr.Kd = 1.0;

    PID_CtrlStr.Gain.Input = 10.0;
    PID_CtrlStr.Gain.OutPut = 10.0;
#endif
    float target_value = 100.0;
    float actual_value = 0.0;
    while(1)
    {

       PID_Ctrl(&PID_CtrlStr,target_value,actual_value);
               /*actual_value = get_actual_value();*/
    }
}

PID_TypeDef PID_Init(pid_type_t PID_Type)
{
    PID_TypeDef PID_CtrlStr =
    {
            .Kp = 0.0,
            .Ki = 0.0,
            .Kd = 0.0,

            .PID_Error = {0.0,0.0,0.0},
            .IntegralValue = 0.0,

            .Result = 0.0,

            .Gain.Input = 1.0,
            .Gain.Output = 1.0,
            .MaxIntegralValue = 100.0,

            .PID_Type = PositionalPID,
    };

    PID_CtrlStr.PID_Type = PID_Type;

    return PID_CtrlStr;
}

PID_TypeDef PID_SetValue(PID_TypeDef *PID_CtrlStr,pid_param_t PIDValue)
{
    PID_CtrlStr->Kp = PIDValue.Kp;
    PID_CtrlStr->Ki = PIDValue.Ki;
    PID_CtrlStr->Kd = PIDValue.Kd;

    return *PID_CtrlStr;
}

PID_TypeDef PID_SetGain(PID_TypeDef *PID_CtrlStr,pid_gain_t GainValue)
{
    PID_CtrlStr->Gain = GainValue;
    return *PID_CtrlStr;
}

PID_TypeDef PID_SetOutPutLimit(PID_TypeDef *PID_CtrlStr,pid_limit_t Limit)
{
    PID_CtrlStr->OutputLimit = Limit;
    return *PID_CtrlStr;
}

PID_TypeDef *PID_Ctrl(PID_TypeDef *PID_CtrlStr,float TargetValue,float ActualValue)
{
    PID_CtrlStr->PID_Error[0] = PID_CtrlStr->PID_Error[1];
    PID_CtrlStr->PID_Error[1] = PID_CtrlStr->PID_Error[2];
    PID_CtrlStr->PID_Error[2] = (TargetValue - ActualValue)* PID_CtrlStr->Gain.Input;

    if (PID_CtrlStr->PID_Type == PositionalPID)
    {
          float result = 0.0;

          PID_CtrlStr->IntegralValue += PID_CtrlStr->PID_Error[2];

          if(fabs(PID_CtrlStr->IntegralValue) > PID_CtrlStr->MaxIntegralValue)
              PID_CtrlStr->IntegralValue = PID_CtrlStr->MaxIntegralValue * fsign(PID_CtrlStr->IntegralValue);

          result = PID_CtrlStr->Kp * PID_CtrlStr->PID_Error[2]+\
                                PID_CtrlStr->Ki * PID_CtrlStr->IntegralValue+\
                                PID_CtrlStr->Kd * (PID_CtrlStr->PID_Error[2] - PID_CtrlStr->PID_Error[1]);

          result = ConstrainFloat(result,PID_CtrlStr->OutputLimit.Min,PID_CtrlStr->OutputLimit.Max);

          PID_CtrlStr->Result = result * PID_CtrlStr->Gain.Output;
    }
    else if(PID_CtrlStr->PID_Type == IncrementalPID)
    {
         float PID_Inc = 0.0;

         PID_Inc = PID_CtrlStr->Kp * (PID_CtrlStr->PID_Error[2] - PID_CtrlStr->PID_Error[1])+\
                             PID_CtrlStr->Ki * (PID_CtrlStr->PID_Error[2])+\
                             PID_CtrlStr->Kd * (PID_CtrlStr->PID_Error[2] - 2*PID_CtrlStr->PID_Error[1]+PID_CtrlStr->PID_Error[0]);

         PID_CtrlStr->Result += PID_Inc * PID_CtrlStr->Gain.Output;

         PID_CtrlStr->Result = ConstrainFloat(PID_CtrlStr->Result,PID_CtrlStr->OutputLimit.Min * PID_CtrlStr->Gain.Output,PID_CtrlStr->OutputLimit.Max * PID_CtrlStr->Gain.Output);
    }
    else
    {
         PID_CtrlStr->Result = 0.0;
    }


    return PID_CtrlStr;
}

pid_limit_t PIDLimit(float Min,float Max)
{
    pid_limit_t Limit;

    Limit.Max = Max;
    Limit.Min = Min;

    return Limit;
}

pid_param_t PIDValue(float Kp,float Ki,float Kd)
{
    pid_param_t PIDValue = {0,0,0};

    PIDValue.Kp = Kp;
    PIDValue.Ki = Ki;
    PIDValue.Kd = Kd;

    return PIDValue;
}

pid_gain_t PIDGainValue(float inputgain,float outputgain)
{
    pid_gain_t GainValue = {0,0};

    GainValue.Input = inputgain;
    GainValue.Output = outputgain;

    return GainValue;
}
