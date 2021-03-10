/*
 * control.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *      This file is for motor and servo close-loop control.
 */
#include <control.h>
#include "include.h"


sint16_t LeftMotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc)
{
  /*
      self: motor �ṹ��
      target_speed ��Ŀ���ٶ� Ҳ����SetSpeed�е� sint16_t speed
      actual_speed :ʵ���ٶ�  ��������������ֵ�����������GetSpeed �ǲ�����µ�
      argv           �Զ����������� 
      argc           �Զ�����������
      argv �� argc �����Ҫʹ�� ��Ҫ�ڳ�ʼ���������ټ���//���У���һ�����������������͵ı�����ַ�������ṹ���ַ��
                    Motor.BindUsrData(&data1,&data2);
                    ����Ĭ�ϴ���NULL �� 0
   */
//    sint16_t PwmValue = 0;
//    
//    PID_TypeDef M_PID = PID_Init(IncrementalPID);
//    
//    
//    
//    PID_Ctrl(&M_PID,target_speed * 1.0,actual_speed * 1.0);
//    
//    PwmValue = (sint16_t)M_PID.Result;
//    
//    return PwmValue;
    return target_speed;
}

sint16_t RightMotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc)
{
    return target_speed;
}

uint16_t Servo1CtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *argv,uint16_t argc)
{
  
    return - target_angle + self->PwmCentValue;

}

uint16_t Servo2CtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *argv,uint16_t argc)
{
  
    return - target_angle + self->PwmCentValue;

}
