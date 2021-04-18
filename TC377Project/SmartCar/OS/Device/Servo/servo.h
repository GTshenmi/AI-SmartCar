/*
 * servo.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SERVO_SERVO_H_
#define OS_DEVICE_SERVO_SERVO_H_

#define AngleToPwm(self,angle) (((self)->PwmCentValue) - (angle))

#include "platform.h"

struct servo_ctrl;

typedef uint16_t (*servo_ctrlcallback)(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc);

typedef struct
{
    sint32_t handle;
}mpu_t;

typedef enum
{
    Servo_Running,Servo_Sleeping,Servo_Stopped,Servo_Stalling,Servo_Normal_Stopping,Servo_Force_Stopping,
}servo_state_t;

typedef struct servo_ctrl
{
    private

        void *Pwmn;
        void *MPUn;

        float MaxAngle;
        float MinAngle;

        uint16_t PwmValue;     /*占空比*/
        uint16_t PwmCentValue; /*舵机中心值*/

        uint32_t StallingTime;

        servo_state_t State;

        float TargetAngle;
        float AngleCache;

        void *Argv;
        uint16_t Argc;
        servo_ctrlcallback CtrlStrategy;

        void (*SetState)(struct servo_ctrl *self,servo_state_t state);

        void (*SetPwmValue)(struct servo_ctrl *self,uint16_t value);/*不使用闭环，直接给占空比驱动*/

        void (*Driver)(struct servo_ctrl *self,uint16_t value);      /*舵机驱动器*/

    public

        uint8_t (*Init)(struct servo_ctrl *self);

        uint16_t (*Update)(struct servo_ctrl *self);

        void (*Protect)(struct servo_ctrl *self,float angle,void *argv,uint16_t argc);/*Servo Protect,It Can Re-Definition*/

        void (*Start)(struct servo_ctrl *self);/*Start Servo*/
        void (*Sleep)(struct servo_ctrl *self);/*Disable CtrlStrategy,But DataRead and Process Still Enable*/
        void (*WakeUp)(struct servo_ctrl *self);/*Enable CtrlStrategy*/
        void (*Stop)(struct servo_ctrl *self); /*Normal Stop Servo:angle = 0*/
        void (*Break)(struct servo_ctrl *self);/*Force Stop Servo:PwmValue = PwmCentValue*/

        float (*SetAngle)(struct servo_ctrl *self,float angle);/*闭环驱动*/
        void (*SetAngleLimit)(struct servo_ctrl *self,float MaxAngle,float MinAngle);
        void (*SetPwmCentValue)(struct servo_ctrl *self,uint16_t value);

        float (*GetAngle)(struct servo_ctrl *self);
        float (*GetAngleFromCache)(struct servo_ctrl *self);
        servo_state_t (*GetState)(struct servo_ctrl *self);
        float (*GetMaxAngle)(struct servo_ctrl *self);
        float (*GetMinAngle)(struct servo_ctrl *self);
        uint16_t (*GetPwmValue)(struct servo_ctrl *self);
        uint16_t (*GetPwmCentValue)(struct servo_ctrl *self);

        void (*Connect)(struct servo_ctrl *self,servo_ctrlcallback ctrlstrategy,void *argv,uint16_t argc);

        struct servo_ctrl *Self;

        void (*Test)(struct servo_ctrl *self);

}servo_ctrl_t;

uint8_t Servo_Init(struct servo_ctrl *self);


#endif /* OS_DEVICE_SERVO_SERVO_H_ */
