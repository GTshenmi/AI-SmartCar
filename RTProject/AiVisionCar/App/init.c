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

void POWER_ENABLE(void)
{ 
    PIN_InitConfig(C10, PIN_MODE_OUTPUT, 1);
}

/*
 * @Brief:  ����0��ʼ��
 * */
void Core0_Init()
{
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
    
    os.init(0);/*���Ҫ����,��һЩ��ʱ�����ĳ�ʼ��*/
    
    RLED.Init(RLED.Self);
    
    
    
    LMotor.Init(LMotor.Self);
    RMotor.Init(RMotor.Self);
    
    LMotor.SetSpeedLimit(LMotor.Self,10000,-10000);
    RMotor.SetSpeedLimit(RMotor.Self,10000,-10000);
           
    LMotor.CtrlStrategy = LeftMotorCtrlStrategy;
    RMotor.CtrlStrategy = RightMotorCtrlStrategy;
    
    LMotor.Start(LMotor.Self);
    RMotor.Start(RMotor.Self);
    
    
    
    Servo1.Init(Servo1.Self);
    Servo2.Init(Servo2.Self);
    
    Servo1.SetAngleLimit(Servo1.Self,90,-90);
    Servo2.SetAngleLimit(Servo2.Self,90,-90);
    
    Servo1.CtrlStrategy = Servo1CtrlStrategy;
    Servo2.CtrlStrategy = Servo2CtrlStrategy;    
    
    Servo1.SetPwmCentValue(Servo1.Self,1000);
    Servo2.SetPwmCentValue(Servo2.Self,1000);    
    
    Servo1.Start(Servo1.Self);
    Servo2.Start(Servo2.Self);  
    
}

