/*
 * motor.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_MOTOR_MOTOR_H_
#define OS_DEVICE_MOTOR_MOTOR_H_

#include "platform.h"

#define Drv8701 0
#define IR7843  1
#define BTN7971 2
#define OtherDriverChip   3

#define DriverChip Drv8701

struct motor_ctrl;

typedef sint16_t (*motor_ctrlcallback)(struct motor_ctrl *self,float target_speed,float actual_speed,void *argv,uint16_t argc);

typedef enum
{
    Motor_Running,Motor_Sleeping,Motor_Stopped,Motor_Stalling,Motor_Normal_Stopping,Motor_Force_Stopping,
}motor_state_t;

typedef struct motor_ctrl
{
    private

        void *Pwmn[2];
        void *Encn;

        sint16_t PwmValue;/*占空比*/

        motor_state_t State;

        float TargetSpeed;

        float SpeedCache;

        float Speed[10];

        uint32_t StallingTime;

        void *Argv;
        uint16_t Argc;
        motor_ctrlcallback CtrlStrategy;

        float MaxSpeed;
        float MinSpeed;

        void (*SetState)(struct motor_ctrl *self,motor_state_t state);
        void (*SetPwmValue)(struct motor_ctrl *self,sint16_t value);  /*不使用闭环，直接给占空比驱动*/
        void (*Driver)(struct motor_ctrl *self,sint16_t value);       /*电机驱动器*/

    public

        uint8_t (*Init)(struct motor_ctrl *self);

        sint16_t (*Update)(struct motor_ctrl *self);

        void (*Connect)(struct motor_ctrl *self,motor_ctrlcallback ctrlstrategy,void *argv,uint16_t argc);

        void (*Protect)(struct motor_ctrl *self,float speed,void *argv,uint16_t argc);/*Motor Protect,Can Re-Definition*/

        void (*Sleep)(struct motor_ctrl *self); /*Disable CtrlStrategy*/
        void (*WakeUp)(struct motor_ctrl *self);/*Enable CtrlStrategy*/
        void (*Stop)(struct motor_ctrl *self); /*Normal Stop Motor:speed = 0*/
        void (*Start)(struct motor_ctrl *self);/*Start Motor*/
        void (*Break)(struct motor_ctrl *self);/*Force Stop Motor:PwmValue = 0*/

        float (*SetSpeed)(struct motor_ctrl *self,float speed);

        void (*SetSpeedLimit)(struct motor_ctrl *self,float MaxSpeed,float MinSpeed);

        float (*GetSpeed)(struct motor_ctrl *self);              /*Read Speed From ENC,it will ReWrite SpeedCache*/
        float (*SpeedFilter)(struct motor_ctrl *self);
        float (*GetSpeedFromCache)(struct motor_ctrl *self);     /*Read Speed From SpeedCache,it will not ReWrite SpeedCache*/
        float (*GetMaxSpeed)(struct motor_ctrl *self);
        float (*GetMinSpeed)(struct motor_ctrl *self);

        uint16_t (*GetPwmValue)(struct motor_ctrl *self);
        motor_state_t (*GetState)(struct motor_ctrl *self);

        struct motor_ctrl *Self;

        void (*Test)(struct motor_ctrl *self);

}motor_ctrl_t;

uint8_t Motor_Init(struct motor_ctrl *self);

#endif /* OS_DEVICE_MOTOR_MOTOR_H_ */
