/*
 * kalman_filter.h
 *
 *  Created on: 2021��1��17��
 *      Author: 936305695
 */

#ifndef APP_SYS_KALMAN_FILTER_H_
#define APP_SYS_KALMAN_FILTER_H_

typedef struct
{
       float K;   /*����������*/
       float X;   /*�������ֵ*/
       float X_;  /*�������ֵ*/

       float Px_;   /*�������Э����*/
       float Px; /*�������Э����*/

       float F;  /*״̬ת�ƾ���*/
       float B;  /*���ƾ���*/
       float H;  /*����ϵͳ��������*/


       float R;      /*������R:���������ķ���*/
       float Q;      /*������Q:���������ķ���*/

}KalManFilter_TypeDef;

KalManFilter_TypeDef KalManFilter_Init();
KalManFilter_TypeDef *KalManFilter_Update(KalManFilter_TypeDef *KalManStr,float V /*ϵͳ��������*/,float Measured_Value);

#endif /* APP_SYS_KALMAN_FILTER_H_ */
