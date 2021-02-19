/*
 * ui.h
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */

#ifndef OS_UI_UI_H_
#define OS_UI_UI_H_

#include "device.h"

typedef struct
{
        void (*Update)(void *argv,uint16_t argc);
}ui_t;

typedef struct
{
        /*电磁传感器部分*/
        uint16_t *LADC;         /*长前瞻电感值(原数据)*/
        uint16_t *SADC;         /*短前瞻电感值(原数据)*/
        float    *NLADC;        /*长前瞻电感值(归一化)*/
        float    *NSADC;        /*短前瞻电感值(归一化)*/

        /*电机部分*/
        sint16_t *Speed;         /*要设置的电机转速(原数据)*/
        float    *NSpeed;        /*要设置的电机转速(归一化)*/
        sint16_t *Actual_Speed;  /*电机的实际速度(原数据)*/
        float    *NActual_Speed; /*电机的实际速度(归一化)*/

        float    *MPID_Kp;       /*电机PID参数*/
        float    *MPID_Ki;       /*电机PID参数*/
        float    *MPID_Kd;       /*电机PID参数*/
        float    *MPID_Result;   /*电机PID输出*/

        uint16_t *M_PwmDuty;     /*电机占空比*/

        /*舵机部分*/
        sint16_t *Angle;         /*要设置的电机转速(原数据)*/
        float    *FAngle;        /*要设置的电机转速(归一化)*/
//        sint16_t *Actual_Speed;  /*电机的实际速度(原数据)*/
//        float    *NActual_Speed; /*电机的实际速度(归一化)*/
//
//        float    *MPID_Kp;       /*电机PID参数*/
//        float    *MPID_Ki;       /*电机PID参数*/
//        float    *MPID_Kd;       /*电机PID参数*/
//        float    *MPID_Result;   /*电机PID输出*/
//
//        uint16_t *M_PwmDuty;     /*电机占空比*/



}ui_data_pkg_t;

extern ui_t UI;
extern __attribute__((weak)) void Task_UiUpdate(void *argv,uint16_t argc);

#endif /* OS_UI_UI_H_ */
