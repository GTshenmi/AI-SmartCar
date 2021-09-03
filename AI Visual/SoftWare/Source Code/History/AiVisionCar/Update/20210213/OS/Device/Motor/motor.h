/*
 * motor.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_MOTOR_MOTOR_H_
#define OS_DEVICE_MOTOR_MOTOR_H_

#include "driver.h"

typedef enum
{
    Motor_Running,Motor_Sleeping,Motor_Stopped,Motor_Stalling,Motor_Normal_Stopping,Motor_Force_Stopping,
}motor_state_t;

typedef struct motor_ctrl
{
/*
 * How To Use
 * 1. Let UsrData Point to You Own Data Struct used for Close Loop Ctrl.if not have Data Struct,Let Motor->UsrData = NULL;
 *    and do not use it in CallBack Function.
 *    For Example,
 *      musr_t MUsr(global);
 *      Motor.BindUsrData(Motor.Self,(void *)MUsr);
 * 2. Write CallBack Function:(not necessary,it has default callback function) CtrlStrategy,Protect;
 *    For Example:
 *     uint16_t MyCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *usr)
 *     {
 *          uint16_t PwmValue = 0;
 *          musr_t musr = (musr_t *)usr;
 *          ...
 *          ...
 *          return PwmValue;
 *     }
 * 3. Motor.Init(Motor.Self);
 * 4. Config Parameter.
 *    Motor.SetSpeedLimit(Motor.Self,10000,-10000);
 * 5. Call Motor.GetSpeed(Motor.Self) in n-ms Interrupt;
 * 6. Motor.Start(Motor.Self);
 * 7. Finish.
 *
 * */
    private

        pwmx_t *PwmDevice[2];
        encx_t *EncDevice;

        uint16_t PwmValue;

        motor_state_t State;

        uint16_t SpeedCache;

        uint32_t StallingTime;

        void *Argv;
        uint16_t Argc;

        sint16_t MaxSpeed;
        sint16_t MinSpeed;

        void (*SetState)(struct motor_ctrl *self,motor_state_t state);
        void (*SetPwmValue)(struct motor_ctrl *self,sint16_t value);  /*不使用闭环，直接给占空比驱动*/
        void (*Driver)(struct motor_ctrl *self,sint16_t value);       /*电机驱动器*/

    public

        uint8_t (*Init)(struct motor_ctrl *self);

        sint16_t (*CtrlStrategy)(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc);/*Motor Close Loop Control Function,Can Re-Definition*/

        void (*BindUsrData)(struct motor_ctrl *self,void *argv,uint16_t argc);
        void (*Protect)(struct motor_ctrl *self,sint16_t speed,void *argv,uint16_t argc);/*Motor Protect,Can Re-Definition*/

        void (*Sleep)(struct motor_ctrl *self); /*Disable CtrlStrategy*/
        void (*WakeUp)(struct motor_ctrl *self);/*Enable CtrlStrategy*/
        void (*Stop)(struct motor_ctrl *self); /*Normal Stop Motor:speed = 0*/
        void (*Start)(struct motor_ctrl *self);/*Start Motor*/
        void (*Break)(struct motor_ctrl *self);/*Force Stop Motor:PwmValue = 0*/

        uint16_t (*SetSpeed)(struct motor_ctrl *self,sint16_t speed);
        void (*SetSpeedLimit)(struct motor_ctrl *self,sint16_t MaxSpeed,sint16_t MinSpeed);

        sint16_t (*GetSpeed)(struct motor_ctrl *self);              /*Read Speed From ENC,it will ReWrite SpeedCache*/
        sint16_t (*GetSpeedFromCache)(struct motor_ctrl *self);     /*Read Speed From SpeedCache,it will not ReWrite SpeedCache*/
        sint16_t (*GetMaxSpeed)(struct motor_ctrl *self);
        sint16_t (*GetMinSpeed)(struct motor_ctrl *self);
        uint16_t (*GetPwmValue)(struct motor_ctrl *self);
        motor_state_t (*GetState)(struct motor_ctrl *self);

        struct motor_ctrl *Self;

        void (*Test)(struct motor_ctrl *self);

}motor_ctrl_t;

uint8_t MotorInit(struct motor_ctrl *self);

#endif /* OS_DEVICE_MOTOR_MOTOR_H_ */
