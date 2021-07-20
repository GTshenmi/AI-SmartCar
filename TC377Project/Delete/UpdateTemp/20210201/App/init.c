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

void Core0_BasicInit();
void Core0_HardWareInit();
void Core0_SoftWareInit();

void Core1_BasicInit();
void Core1_HardWareInit();
void Core1_SoftWareInit();

void Core2_BasicInit();
void Core2_HardWareInit();
void Core2_SoftWareInit();

void BEEP_OFF_TMR_CALLBACK(void *argc,unsigned short argv);

/*
 * @Brief:  ����0��ʼ��
 * */
void Core0_Init()
{
    SCore0.BasicInit = Core0_BasicInit;
    SCore0.HardWareInit = Core0_HardWareInit;
    SCore0.SoftWareInit = Core0_SoftWareInit;
    SCore0.Run = Core0_Main;

    SCore0.BasicInit();

    os.init();
    SCore0.HardWareInit();
    SCore0.SoftWareInit();
#if defined(Chip) && Chip == TC264
    //����CPU���ж�
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&SCore0.mutexCoreInitIsOk);
    while(!IfxCpu_acquireMutex(&SCore1.mutexCoreInitIsOk));
#endif
    /*Init Timer / Interrupt*/

}
/*
 * @Brief:  ����1��ʼ��
 * */
void Core1_Init()
{
    SCore1.BasicInit = Core1_BasicInit;
    SCore1.HardWareInit = Core1_HardWareInit;
    SCore1.SoftWareInit = Core1_SoftWareInit;
    SCore1.Run = Core1_Main;

    SCore1.BasicInit();
    SCore1.HardWareInit();

    SCore1.SoftWareInit();
#if defined(Chip) && Chip == TC264
    //����CPU���ж�
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&SCore1.mutexCoreInitIsOk);
    while(!IfxCpu_acquireMutex(&SCore0.mutexCoreInitIsOk));
#endif
    /*Init Timer / Interrupt*/
}

/*
 * @Brief:  ����1��ʼ��
 * */
void Core2_Init()
{
    SCore2.BasicInit = Core2_BasicInit;
    SCore2.HardWareInit = Core2_HardWareInit;
    SCore2.SoftWareInit = Core2_SoftWareInit;
    SCore2.Run = Core2_Main;

    SCore2.BasicInit();
    SCore2.HardWareInit();

    SCore2.SoftWareInit();
#if defined(Chip) && Chip == TC264
    //����CPU���ж�
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&SCore1.mutexCoreInitIsOk);
    while(!IfxCpu_acquireMutex(&SCore0.mutexCoreInitIsOk));
#endif
    /*Init Timer / Interrupt*/
}

void BEEP_OFF_TMR_CALLBACK(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
}
/*
 * @Brief:  ����0Ӳ������������ʼ��
 * */
void Core0_BasicInit()
{
#if defined(Chip) && Chip == TC264
    //�ر�CPU���ж�
    IfxCpu_disableInterrupts();

    //�رտ��Ź�����������ÿ��Ź�ι����Ҫ�ر�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    //��ȡ����Ƶ��
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

#endif

#if defined(Chip) && Chip == RT1064
    BOARD_ConfigMPU();            /* ��ʼ���ڴ汣����Ԫ */

    BOARD_BootClockRUN();         /* ��ʼ��������ʱ��   */

    POWER_ENABLE();               /* ����ĸ����Χ��·��Դ ��ĸ������й� */

    BOARD_InitPins();             /* ���ڹܽų�ʼ��     */
    BOARD_InitDebugConsole();     /* ��ʼ������         */

    /*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ�
     *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ�
     *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
     */
    /* �������ȼ��� 2: 4����ռ���ȼ� 4�������ȼ� */
    NVIC_SetPriorityGrouping(NVIC_Group2);
#endif
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

    MotorSys.Init(MotorSys.Self);
    ServoSys.Init(ServoSys.Self);

    Motor.UsrData = &Data[data_pointer];
    Servo.UsrData = &Data[data_pointer];
    MotorSys.UsrData = &Data[data_pointer];
    ServoSys.UsrData = &Data[data_pointer];

    MSensorUnit.Start(MSensorUnit.Self);
    MDecisionUnit.Start(MDecisionUnit.Self);
    MExecutionUnit.Start(MExecutionUnit.Self);

    SSensorUnit.Start(SSensorUnit.Self);
    SDecisionUnit.Start(SDecisionUnit.Self);
    SExecutionUnit.Start(SExecutionUnit.Self);

    Data[data_pointer].S_PID = PID_Init(PositionalPID);
    Data[data_pointer].M_PID = PID_Init(IncrementalPID);

    PID_SetGain(&Data[data_pointer].S_PID,PIDGainValue(1.0,1.0));
    PID_SetGain(&Data[data_pointer].M_PID,PIDGainValue(1.0,1.0));

    /*������Ԫ����Ҫ���֣����Թ���ͬһ�������������Ҫ���ֿ��Է���DecisionUnit����*/
    Motor.CtrlStrategy = MotorCtrlStrategy_ALLMode;
    Servo.CtrlStrategy = ServoCtrlStrategy_ALLMode;
    MExecutionUnit.Run = Motor_ExecutionUnitRun_ALLMode;
    SExecutionUnit.Run = Servo_ExecutionUnitRun_ALLMode;
    MSensorUnit.Run = Motor_SensorUnitRun_ALLMode;
    SSensorUnit.Run = Servo_SensorUnitRun_ALLMode;

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
 * @Brief:  ����1Ӳ������������ʼ��
 * */
void Core1_BasicInit()
{
#if defined(Chip) && Chip == TC264

        //�رտ��Ź�
        IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
#endif
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
 * @Brief:  ����2Ӳ������������ʼ��
 * */
void Core2_BasicInit()
{
#if defined(Chip) && Chip == TC264

        //�رտ��Ź�
        IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
#endif
}
/*
 * @Brief:  ����2Ӳ���豸��ʼ��
 * @Attention: ��Ҫ�ڴ˺����г�ʼ����ʱ�����ж�,�ŵ�SoftWareInit����
 * */
void Core2_HardWareInit()
{
    BLED.Init(BLED.Self);
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
    MDecisionUnit.Run = Motor_DecisionUnitRun_ManualBootMode;
    SDecisionUnit.Run = Servo_DecisionUnitRun_ManualBootMode;

    PID_SetValue(&Data[data_pointer].S_PID,PIDValue(0.1,0.0,0.0));
    PID_SetValue(&Data[data_pointer].M_PID,PIDValue(0.1,0.0,0.0));
}


