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

void POWER_ENABLE(void)
{ 
    PIN_InitConfig(C10, PIN_MODE_OUTPUT, 1);
}

/*
 * @Brief:  核心0初始化
 * */
void Core0_Init()
{
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
    
    os.init(0);/*这个要保留,是一些延时函数的初始化*/
    
    RLED.Init(RLED.Self);
    Screen.Init(Screen.Self); 
    //Screen.Test(Screen.Self);
    
    LMotor.Init(LMotor.Self);
    RMotor.Init(RMotor.Self);
    
    LMotor.SetSpeedLimit(LMotor.Self,13000,-13000);
    RMotor.SetSpeedLimit(RMotor.Self,13000,-13000);
           
    
    LMotor.Start(LMotor.Self);
    RMotor.Start(RMotor.Self);
    
    Servo1.Init(Servo1.Self);
    Servo2.Init(Servo2.Self);
       
    
    Servo1.Start(Servo1.Self);
    Servo2.Start(Servo2.Self);
    Console.Init();
    
    PIN_InitConfig(B11,PIN_MODE_INPUT_PULLUP,1);
    ADC_InitConfig(ADC2,ADC2_CH4_J14);
    
    Capture.Init(Capture.Self,50);
    Capture.Start(Capture.Self);
    Capture.Read(Capture.Self,1);
    
    
    
    for(int i = 0 ; i < 3 ; i++)      
      TIMx.Init(&TIM_Resources[i].TIMN);
    
}

