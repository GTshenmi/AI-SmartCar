/*
 * ui.h
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */

#ifndef OS_UI_UI_H_
#define OS_UI_UI_H_

#include "device.h"

#define EnableUiDataLink 1

typedef struct
{
        /*��Ŵ���������*/
        uint16_t *LADC;         /*��ǰհ���ֵ(ԭ����)*/
        float    *NLADC;        /*��ǰհ���ֵ(��һ��)*/
        uint16_t *SADC;         /*��ǰհ���ֵ(ԭ����)*/
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
        sint16_t *Angle;         /*Ҫ���õĶ��ת��(ԭ����)*/
        float    *NAngle;        /*Ҫ���õĶ��ת��(��һ��)*/

        float    *Bias;

        float    *SPID_Kp;       /*���PID����*/
        float    *SPID_Ki;       /*���PID����*/
        float    *SPID_Kd;       /*���PID����*/
        float    *SPID_Result;   /*���PID���*/

        uint16_t *S_PwmDuty;     /*���ռ�ձ�*/

}ui_data_pkg_t;

extern ui_data_pkg_t UIData;

void UI_Update(void *argv,uint16_t argc);

#endif /* OS_UI_UI_H_ */
