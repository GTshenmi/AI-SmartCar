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

void Core0_SoftWareInit_AIMode(void);
void Core0_SoftWareInit_AutoBootMode(void);
void Core0_SoftWareInit_ManualBootMode(void);
void Core0_SoftWareInit_DebugMode(void);

void BEEP_OFF_TMR_CALLBACK(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
}

/*
 * @Brief:  ����0Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
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
 * @Brief:  ����0�����ʼ��(����/����ָ���ʼ��)
 * */
void Core0_SoftWareInit()
{
    data_pointer = CarMode;

    ServoCtrlSysInit();
    MotorCtrlSysInit();

    Motor.BindUsrData(Motor.Self,&Data[data_pointer]);
    Servo.BindUsrData(Servo.Self,&Data[data_pointer]);
    /*������Ԫ����Ҫ���֣����Թ���ͬһ�������������Ҫ���ֿ��Է���DecisionUnit����*/
    Motor.CtrlStrategy = MotorCtrlStrategy;
    Servo.CtrlStrategy = ServoCtrlStrategy;

    MExecutionUnit.Run = Motor_ExecutionUnitRun;
    SExecutionUnit.Run = Servo_ExecutionUnitRun;
    MSensorUnit.Run = Motor_SensorUnitRun;
    SSensorUnit.Run = Servo_SensorUnitRun;

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
}

/*
 * @Brief:  ����1Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
 * */
void Core1_HardWareInit()
{
    BLED.Init(BLED.Self);
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





void Core0_SoftWareInit_AIMode(void)
{
    MDecisionUnit.Run = Motor_DecisionUnitRun_AIMode;
    SDecisionUnit.Run = Servo_DecisionUnitRun_AIMode;

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


