/*
 * kalman_filter.c
 *
 *  Created on: 2021年1月17日
 *      Author: 936305695
 */
#include "kalman_filter.h"

KalManFilter_TypeDef KalManFilter_Init()
{
    KalManFilter_TypeDef KamManStr =
    {
            .K = 0.0,

            .Px = 1.0,
            .Px_ = 1.0,

            .Q = 0.0,
            .R = 0.0,

            .X = 0.0,
            .X_ = 0.0,

            .F = 1.0,
            .B = 0.0,
            .H = 1.0,
    };

    return KamManStr;
}
KalManFilter_TypeDef *KalManFilter_Update(KalManFilter_TypeDef *KalManStr,float V /*系统控制输入*/,float Measured_Value)
{
    /*状态估计*/
    KalManStr->X_ = KalManStr->F * KalManStr->X + KalManStr->B * V;   /*先验估计*/

    KalManStr->Px_ = KalManStr->F * KalManStr->Px * KalManStr->F/*T*/ + KalManStr->Q; /*计算先验估计协方差*/

    /*状态更新*/
    KalManStr->K = (KalManStr->Px * KalManStr->H/*T*/) / (KalManStr->H * KalManStr->Px_ * KalManStr->H/*T*/ + KalManStr->R); /*计算卡尔曼增益*/

    KalManStr->X = KalManStr->X_ + KalManStr->K * (Measured_Value - KalManStr->H * KalManStr->X_); /*后验估计*/

    KalManStr->Px = (1.0 - KalManStr->K * KalManStr->H) * KalManStr->Px_;/*计算后验估计协方差*/

    return KalManStr;
}


