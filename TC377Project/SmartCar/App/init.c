/*
 * init.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *          This file is for the initialization function.
 */

#include "init.h"
#include "include.h"

void UIParameterInit(void);

void KeyPressedCallBack (struct key *self, void *argv, uint16_t argc)
{
    for(int i = 0 ; i < 6 ; i++)
    {
        if(self == KEY[i].Self)
            Screen.WriteXLine(Screen.Self,0,"KEY[%d] Pressed.",i);
    }
}
void BeepOffTimerCallBack(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
}

/*
 * @Brief:  ����0Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
 * */
void Core0_HardWareInit()
{
    data_pointer = CarMode;
    Motor.Init(Motor.Self);
    Motor.BindUsrData(Motor.Self,&Data[data_pointer],sizeof(data_t));
    Motor.CtrlStrategy = MotorCtrlStrategy;

    Servo.Init(Servo.Self);
    Servo.BindUsrData(Servo.Self,&Data[data_pointer],sizeof(data_t));
    Servo.CtrlStrategy = ServoCtrlStrategy;
    Servo.SetPwmCentValue(Servo.Self,850);
    Servo.Start(Servo.Self);

    for(int i = 0;i<CData.MaxLADCDeviceNum;i++)
    {
        LESensor[i].Init(LESensor[i].Self);
        LESensor[i].EnableFilter(LESensor[i].Self,false);
//        LESensor[i].EnableGain(LESensor[i].Self,true);
//        LESensor[i].SetGain(LESensor[i].Self,1.5);
    }
    for(int i = 0;i<CData.MaxSADCDeviceNum;i++)
    {
        SESensor[i].Init(SESensor[i].Self);
        SESensor[i].EnableFilter(SESensor[i].Self,false);
    }

    GLED.Init(GLED.Self);
    BLED.Init(BLED.Self);

    BEEP.Init(BEEP.Self);

#if 0
    //CUART.Init(CUART.Self);
    //DebugCom.Init(DebugCom.Self);
#else
    Console.Init();
#endif

//    for(int i = 0 ; i < CData.MaxKEYDeviceNum;i++)
//    {
//        KEY[i].Init(KEY[i].Self);
//        KEY[i].PressedCallBack = KeyPressedCallBack;
//    }

    UIParameterInit();

    UI_Init();
    //Screen.Init(Screen.Self);
/* System Init Finished,BEEP ON */
    BEEP.ON(BEEP.Self);
/*Set BEEP OFF 1 sec later*/
    os.softtimer.start(1,SoftTimer_Mode_OneShot,1000000,0,BeepOffTimerCallBack,NULL,0);

    Console.WriteLine("HardWare System Init Finished.");
}

/*
 * @Brief:  ����0�����ʼ��(����/����ָ���ʼ��)
 * */
void Core0_SoftWareInit()
{
    ServoCtrlSysInit();
    MotorCtrlSysInit();

    /*������Ԫ����Ҫ���֣����Թ���ͬһ�������������Ҫ���ֿ��Է���DecisionUnit����*/

    MEU.Run = Motor_ExecutionUnitRun;
    SEU.Run = Servo_ExecutionUnitRun;
    MSU.Run = Motor_SensorUnitRun;
    SSU.Run = Servo_SensorUnitRun;

    Data[data_pointer].S_PID = PID_Init(PositionalPID);
    Data[data_pointer].M_PID = PID_Init(IncrementalPID);


    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));
    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,1.0));

    MDU.Run = Motor_DecisionUnitRun_AutoBootMode;
    SDU.Run = Servo_DecisionUnitRun_AutoBootMode;
    Data[data_pointer].AI_State = AI_Free;

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));

    Console.WriteLine("SoftWare System Init Finished.");
    Console.WriteLine("Wait For Core Sync...");
}

/*
 * @Brief:  ����1Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
 * */
void Core1_HardWareInit()
{

}
/*
 * @Brief:  ����1�����ʼ��(����/����ָ���ʼ��)
 * */
void Core1_SoftWareInit()
{

}





/*
 * @Brief:  ����2Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
 * */
void Core2_HardWareInit()
{

}
/*
 * @Brief:  ����2�����ʼ��(����/����ָ���ʼ��)
 * */
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

