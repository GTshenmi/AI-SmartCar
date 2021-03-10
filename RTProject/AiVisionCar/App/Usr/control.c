/*
 * control.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is for motor and servo close-loop control.
 */
#include <control.h>
#include "include.h"


sint16_t LeftMotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc)
{
  /*
      self: motor 结构体
      target_speed ：目标速度 也就是SetSpeed中的 sint16_t speed
      actual_speed :实际速度  编码器读过来的值，如果不调用GetSpeed 是不会更新的
      argv           自定义其他传参 
      argc           自定义其他传参
      argv 和 argc 如果需要使用 需要在初始化代码里再加上//其中，第一个参数传入任意类型的变量地址，包括结构体地址。
                    Motor.BindUsrData(&data1,&data2);
                    不用默认传入NULL 和 0
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
