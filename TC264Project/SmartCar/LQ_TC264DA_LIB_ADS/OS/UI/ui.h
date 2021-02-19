/*
 * ui.h
 *
 *  Created on: 2020��12��7��
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
        /*��Ŵ���������*/
        uint16_t *LADC;         /*��ǰհ���ֵ(ԭ����)*/
        uint16_t *SADC;         /*��ǰհ���ֵ(ԭ����)*/
        float    *NLADC;        /*��ǰհ���ֵ(��һ��)*/
        float    *NSADC;        /*��ǰհ���ֵ(��һ��)*/

        /*�������*/
        sint16_t *Speed;         /*Ҫ���õĵ��ת��(ԭ����)*/
        float    *NSpeed;        /*Ҫ���õĵ��ת��(��һ��)*/
        sint16_t *Actual_Speed;  /*�����ʵ���ٶ�(ԭ����)*/
        float    *NActual_Speed; /*�����ʵ���ٶ�(��һ��)*/

        float    *MPID_Kp;       /*���PID����*/
        float    *MPID_Ki;       /*���PID����*/
        float    *MPID_Kd;       /*���PID����*/
        float    *MPID_Result;   /*���PID���*/

        uint16_t *M_PwmDuty;     /*���ռ�ձ�*/

        /*�������*/
        sint16_t *Angle;         /*Ҫ���õĵ��ת��(ԭ����)*/
        float    *FAngle;        /*Ҫ���õĵ��ת��(��һ��)*/
//        sint16_t *Actual_Speed;  /*�����ʵ���ٶ�(ԭ����)*/
//        float    *NActual_Speed; /*�����ʵ���ٶ�(��һ��)*/
//
//        float    *MPID_Kp;       /*���PID����*/
//        float    *MPID_Ki;       /*���PID����*/
//        float    *MPID_Kd;       /*���PID����*/
//        float    *MPID_Result;   /*���PID���*/
//
//        uint16_t *M_PwmDuty;     /*���ռ�ձ�*/



}ui_data_pkg_t;

extern ui_t UI;
extern __attribute__((weak)) void Task_UiUpdate(void *argv,uint16_t argc);

#endif /* OS_UI_UI_H_ */
