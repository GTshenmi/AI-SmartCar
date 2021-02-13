/*
 * servo.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SERVO_SERVO_H_
#define OS_DEVICE_SERVO_SERVO_H_

#define AngleToPwm(self,angle) (((self)->PwmCentValue) - (angle))

#include "driver.h"

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
/*
 *
 * How To Use
 * 1. Let UsrData Point to You Own Data Struct used for Close Loop Ctrl.if not have Data Struct,Let Servo->UsrData = NULL;
 *    and do not use it in CallBack Function.
 *    For Example,susr_t SUsr(global); Servo.UsrData = (void *)SUsr;
 * 2. Write CallBack Function:(not necessary,it has default callback function) ReadData CtrlStrategy,DataProcess,Protect,GetAngle;
 *    Do Not Write and Use Those Function if MPU6050 or other Angle Sensor Not Exist.
 *    For Example:
 *     uint16_t MyCtrlStrategy(struct servo_ctrl *self,sint16_t target_angle,sint16_t actual_angle,void *usr)
 *     {
 *          uint32_t PwmValue = 0;
 *          susr_t susr = (susr_t *)usr;
 *          ...
 *          ...
 *          return PwmValue;
 *     }
 * 3. Servo.Init(Servo.Self);
 * 4. Call Servo.GetAngle(Servo.Self) in n-ms Interrupt if step 2 if Done.
 * 5. Servo.Start(Servo.Self);
 * 6. Finish.
 *
 * */
    private

        pwmx_t *PwmDevice;
        mpu_t *MPUDevice;

        sint16_t MaxAngle;
        sint16_t MinAngle;

        uint16_t PwmValue;
        uint16_t PwmCentValue;

        uint32_t StallingTime;

        servo_state_t State;

        float AngleCache;

        void *Argv;
        uint16_t Argc;

        void (*SetState)(struct servo_ctrl *self,servo_state_t state);

        void (*SetPwmValue)(struct servo_ctrl *self,uint16_t value);/*不使用闭环，直接给占空比驱动*/

        void (*Driver)(struct servo_ctrl *self,uint16_t value);      /*舵机驱动器*/

    public

        uint8_t (*Init)(struct servo_ctrl *self);

        uint16_t (*CtrlStrategy)(struct servo_ctrl *self,sint16_t target_angle,float actual_angle,void *argv,uint16_t argc);/*Servo Close Loop Control Function,It Can Re-Definition,Keep It NULL if  MPU6050 or other Angle Sensor not exist.*/

        void (*Protect)(struct servo_ctrl *self,sint16_t angle,void *argv,uint16_t argc);/*Servo Protect,It Can Re-Definition*/

        void (*Start)(struct servo_ctrl *self);/*Start Servo*/
        void (*Sleep)(struct servo_ctrl *self);/*Disable CtrlStrategy,But DataRead and Process Still Enable*/
        void (*WakeUp)(struct servo_ctrl *self);/*Enable CtrlStrategy*/
        void (*Stop)(struct servo_ctrl *self); /*Normal Stop Servo:angle = 0*/
        void (*Break)(struct servo_ctrl *self);/*Force Stop Servo:PwmValue = PwmCentValue*/

        uint16_t (*SetAngle)(struct servo_ctrl *self,sint16_t angle);/*闭环驱动*/
        void (*SetAngleLimit)(struct servo_ctrl *self,sint16_t MaxAngle,sint16_t MinAngle);
        void (*SetPwmCentValue)(struct servo_ctrl *self,uint16_t value);

        float (*GetAngle)(struct servo_ctrl *self);
        float (*GetAngleFromCache)(struct servo_ctrl *self);
        servo_state_t (*GetState)(struct servo_ctrl *self);
        sint16_t (*GetMaxAngle)(struct servo_ctrl *self);
        sint16_t (*GetMinAngle)(struct servo_ctrl *self);
        uint16_t (*GetPwmValue)(struct servo_ctrl *self);
        uint16_t (*GetPwmCentValue)(struct servo_ctrl *self);

        void (*BindUsrData)(struct servo_ctrl *self,void *argv,uint16_t argc);

        struct servo_ctrl *Self;

        void (*Test)(struct servo_ctrl *self);

}servo_ctrl_t;

uint8_t ServoInit(struct servo_ctrl *self);


#endif /* OS_DEVICE_SERVO_SERVO_H_ */
