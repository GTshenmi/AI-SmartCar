/*
 * kalman_filter.h
 *
 *  Created on: 2021年1月17日
 *      Author: 936305695
 */

#ifndef APP_SYS_KALMAN_FILTER_H_
#define APP_SYS_KALMAN_FILTER_H_

typedef struct
{
       float K;   /*卡尔曼增益*/
       float X;   /*后验估计值*/
       float X_;  /*先验估计值*/

       float Px_;   /*先验估计协方差*/
       float Px; /*后验估计协方差*/

       float F;  /*状态转移矩阵*/
       float B;  /*控制矩阵*/
       float H;  /*测量系统参数矩阵*/


       float R;      /*超参数R:测量噪声的方差*/
       float Q;      /*超参数Q:过程噪声的方差*/

}KalManFilter_TypeDef;

KalManFilter_TypeDef KalManFilter_Init();
KalManFilter_TypeDef *KalManFilter_Update(KalManFilter_TypeDef *KalManStr,float V /*系统控制输入*/,float Measured_Value);

#endif /* APP_SYS_KALMAN_FILTER_H_ */
