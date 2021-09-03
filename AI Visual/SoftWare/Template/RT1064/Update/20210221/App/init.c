/*
 * init.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *          This file is for the initialization function.
 */

#include "init.h"
#include "include.h"

void Core0_SoftWareInit_AIMode(void);
void Core0_SoftWareInit_AutoBootMode(void);
void Core0_SoftWareInit_ManualBootMode(void);
void Core0_SoftWareInit_DebugMode(void);
void UIParameterInit(void);

void BEEP_OFF_TMR_CALLBACK(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
}

/*
 * @Brief:  核心0硬件设备初始化
 * @Attention: 不要在此函数中初始化定时器或中断,放到SoftWareInit后面
 * */
void Core0_HardWareInit()
{
    Motor.Init(Motor.Self);
    Servo.Init(Servo.Self);

    for(int i = 0;i<CData.MaxLADCDeviceNum;i++)
    {
        LESensor[i].Init(LESensor[i].Self);
    }
    for(int i = 0;i<CData.MaxSADCDeviceNum;i++)
    {
        SESensor[i].Init(SESensor[i].Self);
    }

    GLED.Init(GLED.Self);

    BEEP.Init(BEEP.Self);

    for(int i = 0;i<CData.MaxKEYDeviceNum;i++)
        KEY[i].Init(KEY[i].Self);

    CUART.Init(CUART.Self);

//    DIPSwitch.Init(DIPSwitch.Self);

//    Screen.Init(Screen.Self);

/*System Init Finished,BEEP ON */
    BEEP.ON(BEEP.Self);
/*Set BEEP OFF 1 sec later*/
    os.softtimer.start(1,SoftTimer_Mode_OneShot,1000000,0,BEEP_OFF_TMR_CALLBACK,NULL,0);

}
/*
 * @Brief:  核心0软件初始化(参数/函数指针初始化)
 * */
void Core0_SoftWareInit()
{
    data_pointer = CarMode;

    ServoCtrlSysInit();
    MotorCtrlSysInit();

    Motor.BindUsrData(Motor.Self,&Data[data_pointer],sizeof(data_t));
    Servo.BindUsrData(Servo.Self,&Data[data_pointer],sizeof(data_t));
    /*其他单元不需要划分，可以共用同一个函数，如果需要划分可以仿照DecisionUnit更改*/
    Motor.CtrlStrategy = MotorCtrlStrategy;
    Servo.CtrlStrategy = ServoCtrlStrategy;

    MEU.Run = Motor_ExecutionUnitRun;
    SEU.Run = Servo_ExecutionUnitRun;
    MSU.Run = Motor_SensorUnitRun;
    SSU.Run = Servo_SensorUnitRun;

    Data[data_pointer].S_PID = PID_Init(PositionalPID);
    Data[data_pointer].M_PID = PID_Init(IncrementalPID);

    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));
    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,1.0));

    switch(Data[data_pointer].CarMode)
    {
        case AI_Mode:
            Core0_SoftWareInit_AIMode();
            break;
        case AutoBoot_Mode:
            Core0_SoftWareInit_AutoBootMode();
            break;
        case ManualBoot_Mode:
            Core0_SoftWareInit_ManualBootMode();
            break;
        case DebugMode:
            Core0_SoftWareInit_DebugMode();
            break;
        default:
            break;
    }

    UIParameterInit();
}

/*
 * @Brief:  核心1硬件设备初始化
 * @Attention: 不要在此函数中初始化定时器或中断,放到SoftWareInit后面
 * */
void Core1_HardWareInit()
{
    BLED.Init(BLED.Self);
}
/*
 * @Brief:  核心1软件初始化(参数/函数指针初始化)
 * */
void Core1_SoftWareInit()
{

}





/*
 * @Brief:  核心2硬件设备初始化
 * @Attention: 不要在此函数中初始化定时器或中断,放到SoftWareInit后面
 * */
void Core2_HardWareInit()
{

}
/*
 * @Brief:  核心2软件初始化(参数/函数指针初始化)
 * */
void Core2_SoftWareInit()
{

}





void Core0_SoftWareInit_AIMode(void)
{
    MDecisionUnit.Run = Motor_DecisionUnitRun_AIMode;
    SDecisionUnit.Run = Servo_DecisionUnitRun_AIMode;
    Data[data_pointer].AI_State = AI_Free;
    NeuralNetworkInit();
    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));
}
void Core0_SoftWareInit_AutoBootMode(void)
{
    MDecisionUnit.Run = Motor_DecisionUnitRun_AutoBootMode;

    SDecisionUnit.Run = Servo_DecisionUnitRun_AutoBootMode;

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));
}
void Core0_SoftWareInit_ManualBootMode(void)
{
    MDecisionUnit.Run = Motor_DecisionUnitRun_ManualBootMode;

    SDecisionUnit.Run = Servo_DecisionUnitRun_ManualBootMode;

    MExecutionUnit.Sleep(MExecutionUnit.Self,0);
    SExecutionUnit.Sleep(SExecutionUnit.Self,0);

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));
}
void Core0_SoftWareInit_DebugMode(void)
{
    MDecisionUnit.Run = Motor_DecisionUnitRun_DebugMode;
    SDecisionUnit.Run = Servo_DecisionUnitRun_DebugMode;

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));
}

void UIParameterInit(void)
{
#if EnableUiDataLink

    UIData.Actual_Speed = &Data[data_pointer].Actual_Speed;;
    UIData.Angle =        &Data[data_pointer].Angle;
    UIData.Bias =         &Data[data_pointer].Bias;
    UIData.MPID_Kd =      &Data[data_pointer].M_PID.Kd;
    UIData.MPID_Ki =      &Data[data_pointer].M_PID.Ki;
    UIData.MPID_Kp =      &Data[data_pointer].M_PID.Kp;
    UIData.MPID_Result =  &Data[data_pointer].M_PID.Result;
    UIData.M_PwmDuty =    &Motor.PwmValue;
    UIData.SPID_Kd =      &Data[data_pointer].S_PID.Kd;
    UIData.SPID_Ki =      &Data[data_pointer].S_PID.Ki;
    UIData.SPID_Kp =      &Data[data_pointer].S_PID.Kp;
    UIData.SPID_Result =  &Data[data_pointer].S_PID.Result;
    UIData.S_PwmDuty =    &Servo.PwmValue;
    UIData.Speed =        &Data[data_pointer].Speed;

    UIData.SADC =          Data[data_pointer].SADC_Value;
    UIData.LADC =          Data[data_pointer].LADC_Value;
    UIData.NLADC  =        Data[data_pointer].N_LADC;
    UIData.NSADC =         Data[data_pointer].N_SADC;

    UIData.CarState =    (uint *)&Data[data_pointer].CarState;
    UIData.CarMode =     (uint *)&Data[data_pointer].CarMode;
    UIData.AI_State =    (uint *)&Data[data_pointer].AI_State;
    UIData.ElementType = (uint *)&Data[data_pointer].ElementType;

    UIData.MotorSysState[0] = (uint *)&MSU.State;
    UIData.MotorSysState[1] = (uint *)&MDU.State;
    UIData.MotorSysState[2] = (uint *)&MEU.State;
    UIData.ServoSysState[0] = (uint *)&SSU.State;
    UIData.ServoSysState[1] = (uint *)&SDU.State;
    UIData.ServoSysState[2] = (uint *)&SEU.State;


    //UIData.NActual_Speed = &Data[data_pointer].Actual_Speed;
    //UIData.NAngle = NULL;
    //UIData.NSpeed = &Data[data_pointer].Speed;

#else

    UIData.Actual_Speed = NULL;
    UIData.Angle = NULL;
    UIData.Bias = NULL;
    UIData.LADC = NULL;
    UIData.MPID_Kd = NULL;
    UIData.MPID_Ki = NULL;
    UIData.MPID_Kp = NULL;
    UIData.MPID_Result = NULL;
    UIData.M_PwmDuty = NULL;
    UIData.NActual_Speed = NULL;
    UIData.NAngle = NULL;
    UIData.NLADC  = NULL;
    UIData.NSADC = NULL;
    UIData.NSpeed = NULL;
    UIData.SADC = NULL;
    UIData.SPID_Kd = NULL;
    UIData.SPID_Ki = NULL;
    UIData.SPID_Kp = NULL;
    UIData.SPID_Result = NULL;
    UIData.S_PwmDuty = NULL;
    UIData.Speed = NULL;

    UIData.CarState =    NULL;
    UIData.CarMode =     NULL;
    UIData.AI_State = NULL;
    UIData.MotorSysState[0] = NULL;
    UIData.MotorSysState[1] = NULL;
    UIData.MotorSysState[2] = NULL;
    UIData.ServoSysState[0] = NULL;
    UIData.ServoSysState[1] = NULL;
    UIData.ServoSysState[2] = NULL;

#endif
}

