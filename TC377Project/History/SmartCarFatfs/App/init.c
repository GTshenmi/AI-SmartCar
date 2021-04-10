/*
 * init.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 *  @Brief:
 *          This file is for the initialization function.
 */

#include "init.h"
#include "include.h"

void UIParameterInit(void);

void BeepOffTimerCallBack(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
}

void Core0_HardWareInit()
{
    data_pointer = CarMode;

    /*Init Motor.*/
    Motor.Init(Motor.Self);
    Motor.Connect(Motor.Self,MotorCtrlStrategy,&Data[data_pointer],sizeof(data_t));
    Motor.SetSpeedLimit(Motor.Self,10000,-10000);
    Motor.Start(Motor.Self);

    /*Init Servo.*/
    Servo.Init(Servo.Self);
    Servo.Connect(Servo.Self,ServoCtrlStrategy,&Data[data_pointer],sizeof(data_t));
    Servo.SetAngleLimit(Servo.Self,200.0,-200.0);
    Servo.SetPwmCentValue(Servo.Self,300);
    Servo.Start(Servo.Self);
    Servo.SetAngle(Servo.Self,0);
    Servo.Update(Servo.Self);

    /*Init Sensor.*/
    for(int i = 0;i<CData.MaxLADCDeviceNum;i++)
    {
        LESensor[i].Init(LESensor[i].Self);
        LESensor[i].EnableFilter(LESensor[i].Self,true);
    }
    for(int i = 0;i<CData.MaxSADCDeviceNum;i++)
    {
        SESensor[i].Init(SESensor[i].Self);
        SESensor[i].EnableFilter(SESensor[i].Self,true);
    }

    /*Init LED And BEEP.*/
    GLED.Init(GLED.Self);
    BLED.Init(BLED.Self);

    BEEP.Init(BEEP.Self);

    /*Init Debug Console.*/

    Console.Init();

    /*Init User Interface.*/

    UIParameterInit();

    UI_Init();

    /* System Init Finished,BEEP ON */
    BEEP.ON(BEEP.Self);
    /*Set BEEP OFF 1 sec later*/
    os.softtimer.start(1,SoftTimer_Mode_OneShot,1000000,0,BeepOffTimerCallBack,NULL,0);

#if defined(Debug)
    Console.WriteLine("HardWare System Init Finished.");
#endif

}

void Core0_SoftWareInit()
{
    /*Init Control System.*/
    ServoCtrlSysInit();
    MotorCtrlSysInit();
    /*Init PID Controller.*/
    Data[data_pointer].S_PID = PID_Init(PositionalPID);
    Data[data_pointer].M_PID = PID_Init(IncrementalPID);
    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));
    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,1.0));

    /*Init Data Save System.*/
    DataSaveSysInit("1.xls","1.txt");

    /*Init Debug System.*/
    ANO.Init();

    /*Init NerualNetWork.*/
    NeuralNetworkInit(NULL);

    /*Init Parameter.*/
    ParameterInit(&Data[data_pointer]);

#if defined(Debug)
    Console.WriteLine("SoftWare System Init Finished.");
    Console.WriteLine("Wait For Core Sync...");
#endif
}

void Core1_HardWareInit()
{

}
void Core1_SoftWareInit()
{

}

void Core2_HardWareInit()
{

}
void Core2_SoftWareInit()
{

}

void UIParameterInit(void)
{
#if EnableUiDataLink

    UIData.Actual_Speed = &Data[data_pointer].Actual_Speed;
    UIData.Angle =        &Data[data_pointer].Angle;
    UIData.Bias =         &Data[data_pointer].Bias;
    UIData.MPID_Kd =      &Data[data_pointer].M_PID.Kd;
    UIData.MPID_Ki =      &Data[data_pointer].M_PID.Ki;
    UIData.MPID_Kp =      &Data[data_pointer].M_PID.Kp;
    UIData.MPID_Result =  &Data[data_pointer].M_PID.Result;
    UIData.M_PwmDuty =    (uint16_t *)&Motor.PwmValue;
    UIData.SPID_Kd =      &Data[data_pointer].S_PID.Kd;
    UIData.SPID_Ki =      &Data[data_pointer].S_PID.Ki;
    UIData.SPID_Kp =      &Data[data_pointer].S_PID.Kp;
    UIData.SPID_Result =  &Data[data_pointer].S_PID.Result;
    UIData.S_PwmDuty =    &Servo.PwmValue;
    //UIData.S_PwmDuty =    &Data[data_pointer].SPwmValue;
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

    UIData.Actual_Speed = &Data[4].Actual_Speed;
    UIData.Angle =        &Data[4].Angle;
    UIData.Bias =         &Data[4].Bias;
    UIData.MPID_Kd =      &Data[4].M_PID.Kd;
    UIData.MPID_Ki =      &Data[4].M_PID.Ki;
    UIData.MPID_Kp =      &Data[4].M_PID.Kp;
    UIData.MPID_Result =  &Data[4].M_PID.Result;
    UIData.M_PwmDuty =    &Data[4].Speed;
    UIData.SPID_Kd =      &Data[4].S_PID.Kd;
    UIData.SPID_Ki =      &Data[4].S_PID.Ki;
    UIData.SPID_Kp =      &Data[4].S_PID.Kp;
    UIData.SPID_Result =  &Data[4].S_PID.Result;
    UIData.S_PwmDuty =    &Data[4].Angle;
    UIData.Speed =        &Data[4].Speed;

    UIData.SADC =          Data[4].SADC_Value;
    UIData.LADC =          Data[4].LADC_Value;
    UIData.NLADC  =        Data[4].N_LADC;
    UIData.NSADC =         Data[4].N_SADC;

    UIData.CarState =    (uint *)&Data[4].CarState;
    UIData.CarMode =     (uint *)&Data[4].CarMode;
    UIData.AI_State =    (uint *)&Data[4].AI_State;
    UIData.ElementType = (uint *)&Data[4].ElementType;

    UIData.MotorSysState[0] = (uint *)&Data[4].CarState;
    UIData.MotorSysState[1] = (uint *)&Data[4].CarState;
    UIData.MotorSysState[2] = (uint *)&Data[4].CarState;
    UIData.ServoSysState[0] = (uint *)&Data[4].CarState;
    UIData.ServoSysState[1] = (uint *)&Data[4].CarState;
    UIData.ServoSysState[2] = (uint *)&Data[4].CarState;


    //UIData.NActual_Speed = &Data[data_pointer].Actual_Speed;
    //UIData.NAngle = NULL;
    //UIData.NSpeed = &Data[data_pointer].Speed;

#endif
}

