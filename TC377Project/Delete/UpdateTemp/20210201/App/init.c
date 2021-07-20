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
 * @Brief:  核心0初始化
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
    //开启CPU总中断
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&SCore0.mutexCoreInitIsOk);
    while(!IfxCpu_acquireMutex(&SCore1.mutexCoreInitIsOk));
#endif
    /*Init Timer / Interrupt*/

}
/*
 * @Brief:  核心1初始化
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
    //开启CPU总中断
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&SCore1.mutexCoreInitIsOk);
    while(!IfxCpu_acquireMutex(&SCore0.mutexCoreInitIsOk));
#endif
    /*Init Timer / Interrupt*/
}

/*
 * @Brief:  核心1初始化
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
    //开启CPU总中断
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
 * @Brief:  核心0硬件基本环境初始化
 * */
void Core0_BasicInit()
{
#if defined(Chip) && Chip == TC264
    //关闭CPU总中断
    IfxCpu_disableInterrupts();

    //关闭看门狗，如果不设置看门狗喂狗需要关闭
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    //读取总线频率
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

#endif

#if defined(Chip) && Chip == RT1064
    BOARD_ConfigMPU();            /* 初始化内存保护单元 */

    BOARD_BootClockRUN();         /* 初始化开发板时钟   */

    POWER_ENABLE();               /* 开启母板外围电路电源 与母版设计有关 */

    BOARD_InitPins();             /* 串口管脚初始化     */
    BOARD_InitDebugConsole();     /* 初始化串口         */

    /*设置中断优先级组  0: 0个抢占优先级16位个子优先级
     *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级
     *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
     */
    /* 配置优先级组 2: 4个抢占优先级 4个子优先级 */
    NVIC_SetPriorityGrouping(NVIC_Group2);
#endif
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

    /*其他单元不需要划分，可以共用同一个函数，如果需要划分可以仿照DecisionUnit更改*/
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
 * @Brief:  核心1硬件基本环境初始化
 * */
void Core1_BasicInit()
{
#if defined(Chip) && Chip == TC264

        //关闭看门狗
        IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
#endif
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
 * @Brief:  核心2硬件基本环境初始化
 * */
void Core2_BasicInit()
{
#if defined(Chip) && Chip == TC264

        //关闭看门狗
        IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
#endif
}
/*
 * @Brief:  核心2硬件设备初始化
 * @Attention: 不要在此函数中初始化定时器或中断,放到SoftWareInit后面
 * */
void Core2_HardWareInit()
{
    BLED.Init(BLED.Self);
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


