/*
 * ui.h
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */

#ifndef OS_UI_UI_H_
#define OS_UI_UI_H_

#include "device.h"
#include "template_page.h"

#define EnableUiDataLink 1

typedef struct
{
        /*��Ŵ���������*/
        uint16_t *LADC;         /*��ǰհ���ֵ(ԭ����)*/
        float    *NLADC;        /*��ǰհ���ֵ(��һ��)*/
        uint16_t *SADC;         /*��ǰհ���ֵ(ԭ����)*/
        float    *NSADC;        /*��ǰհ���ֵ(��һ��)*/

        /*�������*/
        float    *Speed;         /*Ҫ���õĵ��ת��(ԭ����)*/
        float    *NSpeed;        /*Ҫ���õĵ��ת��(��һ��)*/
        float    *Actual_Speed;  /*�����ʵ���ٶ�(ԭ����)*/
        float    *NActual_Speed; /*�����ʵ���ٶ�(��һ��)*/

        float    *MPID_Kp;       /*���PID����*/
        float    *MPID_Ki;       /*���PID����*/
        float    *MPID_Kd;       /*���PID����*/
        float    *MPID_Result;   /*���PID���*/

        uint16_t *M_PwmDuty;     /*���ռ�ձ�*/

        /*�������*/

        //uint *ElementType;

        float    *Angle;         /*Ҫ���õĶ���Ƕ�(ԭ����)*/
        float    *NAngle;        /*Ҫ���õĶ���Ƕ�(��һ��)*/

        float    *Ke;

        float    *Bias;          /*����ƫ��*/

        float    *DynamicKp;

        float    *SPID_Kp;       /*���PID����*/
        float    *SPID_Ki;       /*���PID����*/
        float    *SPID_Kd;       /*���PID����*/
        float    *SPID_Result;   /*���PID���*/

        /*����״̬����*/

        uint16_t *S_PwmDuty;     /*���ռ�ձ�*/

        uint *CarState;          /*С������״̬ 0 : ֹͣ 1������*/

        uint *AI_State;          /* AI ����״̬ AI_Free = 0 , AI_Start = 1 , AI_Busy = 2 , AI_Fin = 3*/

        uint *MotorSysState[3];  /*���ϵͳ״̬ Stopped = 0 ,Running = 1,Sleeping = 2  [0]=MSU [1]=MDU [2] = MEU */

        uint *ServoSysState[3];  /*���ϵͳ״̬ Stopped = 0 ,Running = 1,Sleeping = 2  [0]=SSU [1]=SDU [2] = SEU */

        uint *ElementType;       /*����Ԫ������*/

        uint *CarMode;           /*С������ģʽ AI_Mode = 0,   AutoBoot_Mode = 1,  ManualBoot_Mode = 2,   DebugMode = 3,  */

        float *h_bias;
        float *h_difference;
        float *h_sum;
        float *v_bias;
        float *v_difference;
        float *v_sum;
        float *o_bias;
        float *o_difference;
        float *o_sum;

}ui_data_pkg_t;

extern ui_data_pkg_t UIData;

void UI_Update(void *argv,uint16_t argc);

void UI_Init(void);

extern void (*LoadParameterFromSD)(void);
extern void (*SaveParameterToSD)(void);

#endif /* OS_UI_UI_H_ */
