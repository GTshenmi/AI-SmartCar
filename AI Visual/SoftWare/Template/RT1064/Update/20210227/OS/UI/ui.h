/*
 * ui.h
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */

#ifndef OS_UI_UI_H_
#define OS_UI_UI_H_

#include "device.h"

#define EnableUiDataLink 1

typedef struct
{
        /*电磁传感器部分*/
        uint16_t *LADC;         /*长前瞻电感值(原数据)*/
        float    *NLADC;        /*长前瞻电感值(归一化)*/
        uint16_t *SADC;         /*短前瞻电感值(原数据)*/
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
        sint16_t *Angle;         /*要设置的舵机角度(原数据)*/
        float    *NAngle;        /*要设置的舵机角度(归一化)*/

        float    *Bias;          /*中线偏差*/

        float    *SPID_Kp;       /*舵机PID参数*/
        float    *SPID_Ki;       /*舵机PID参数*/
        float    *SPID_Kd;       /*舵机PID参数*/
        float    *SPID_Result;   /*舵机PID输出*/

        /*其他状态参数*/

        uint16_t *S_PwmDuty;     /*舵机占空比*/

        uint *CarState;          /*小车运行状态 0 : 停止 1：运行*/

        uint *AI_State;          /* AI 解算状态 AI_Free = 0 , AI_Start = 1 , AI_Busy = 2 , AI_Fin = 3*/

        uint *MotorSysState[3];  /*电机系统状态 Stopped = 0 ,Running = 1,Sleeping = 2  [0]=MSU [1]=MDU [2] = MEU */

        uint *ServoSysState[3];  /*舵机系统状态 Stopped = 0 ,Running = 1,Sleeping = 2  [0]=SSU [1]=SDU [2] = SEU */

        uint *ElementType;       /*赛道元素类型*/

        uint *CarMode;           /*小车运行模式 AI_Mode = 0,   AutoBoot_Mode = 1,  ManualBoot_Mode = 2,   DebugMode = 3,  */

}ui_data_pkg_t;

extern ui_data_pkg_t UIData;

void UI_Update(void *argv,uint16_t argc);

void UI_Init(void);

#endif /* OS_UI_UI_H_ */
