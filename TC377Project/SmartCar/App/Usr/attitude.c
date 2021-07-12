/*
 * attitude.c
 *
 *  Created on: 2021年4月18日
 *      Author: 936305695
 */
#include "attitude.h"
#include "math.h"
#include "sysmath.h"

#define sampleFreq  512.0f          // sample frequency in Hz
#define twoKpDef    (2.0f * 0.5f)   // 2 * proportional gain
#define twoKiDef    (2.0f * 0.0f)   // 2 * integral gain


// Variable definitions
volatile float twoKp = twoKpDef;                                            // 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;                                            // 2 * integral gain (Ki)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;                  // quaternion of sensor frame relative to auxiliary frame
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f; // integral error terms scaled by Ki

void MahonyAHRSupdate(axis_t *acc/*input*/,axis_t *gyro/*input*/,axis_t *mag/*not use*/,attitude_t *attitude/*output*/)
{
    float gx,gy,gz,ax,ay,az,mx,my,mz;

    ax = acc->x;
    ay = acc->y;
    az = acc->z;

    gx = gyro->x;
    gy = gyro->y;
    gz = gyro->z;

    mx = mag->x;
    my = mag->y;
    mz = mag->z;

    float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float hx, hy, hz, bx, bz;
    float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    // 只在加速度计数据有效时才进行运算
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        // 将加速度计得到的实际重力加速度向量v单位化
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        // 将磁力计得到的实际磁场向量m单位化
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        // 辅助变量，以避免重复运算
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        // 通过磁力计测量值与坐标转换矩阵得到大地坐标系下的理论地磁向量
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        hz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

        // Estimated direction of gravity and magnetic field
        // 将理论重力加速度向量与理论地磁向量变换至机体坐标系
        halfvx = q1q3 - q0q2;
        halfvy = q0q1 + q2q3;
        halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

        // Error is sum of cross product between estimated direction and measured direction of field vectors
        // 通过向量外积得到重力加速度向量和地磁向量的实际值与测量值之间误差
        halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
        halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
        halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

        // Compute and apply integral feedback if enabled
        // 在PI补偿器中积分项使能情况下计算并应用积分项
        if(twoKi > 0.0f) {
            // integral error scaled by Ki
            // 积分过程
            integralFBx += twoKi * halfex * (1.0f / sampleFreq);
            integralFBy += twoKi * halfey * (1.0f / sampleFreq);
            integralFBz += twoKi * halfez * (1.0f / sampleFreq);

            // apply integral feedback
            // 应用误差补偿中的积分项
            gx += integralFBx;
            gy += integralFBy;
            gz += integralFBz;
        }
        else {
            // prevent integral windup
            // 避免为负值的Ki时积分异常饱和
            integralFBx = 0.0f;
            integralFBy = 0.0f;
            integralFBz = 0.0f;
        }

        // Apply proportional feedback
        // 应用误差补偿中的比例项
        gx += twoKp * halfex;
        gy += twoKp * halfey;
        gz += twoKp * halfez;
    }

    // Integrate rate of change of quaternion
    // 微分方程迭代求解
    gx *= (0.5f * (1.0f / sampleFreq));     // pre-multiply common factors
    gy *= (0.5f * (1.0f / sampleFreq));
    gz *= (0.5f * (1.0f / sampleFreq));
    qa = q0;
    qb = q1;
    qc = q2;
    q0 += (-qb * gx - qc * gy - q3 * gz);
    q1 += (qa * gx + qc * gz - q3 * gy);
    q2 += (qa * gy - qb * gz + q3 * gx);
    q3 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    // 单位化四元数 保证四元数在迭代过程中保持单位性质
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    attitude->yaw = atan2(2.0*(q1q2+q0q3),q0q0+q1q1-q2q2-q3q3)*57.3f;//这种方法肯定有漂移
    attitude->roll = asin(2.0*(q0*q1+q2*q3))*57.3f; //横滚角
    attitude->pitch = asin(2.0*(q0*q2-q1*q3))*57.3f;//俯仰角57.2957795f
}

void AttitudeUpdate(axis_t *acc/*input*/,axis_t *gyro/*input*/,axis_t *mag/*not use*/,attitude_t *attitude/*output*/)
{
    //float matrix[9]={1.f,0.0f,0.0f,0.0f,1.f,0.0f,0.0f,0.0f,1.f};//初始化矩阵 定高定点

    static float q0=1.0,q1=0.0,q2=0.0,q3=0.0;
    static float exInt=0.0,eyInt=0.0,ezInt=0.0;

    //姿态解算PI控制器
    static float halfT = 0.001f;

    static float Kp = 2.0f;
    static float Ki = 0.0001f;


    float norm;
    float vx,vy,vz;
    float ex,ey,ez;

    float q0q0=q0*q0;
    float q0q1=q0*q1;
    float q0q2=q0*q2;
    float q0q3=q0*q3;
    float q1q1=q1*q1;
    float q1q2=q1*q2;
    float q1q3=q1*q3;
    float q2q2=q2*q2;
    float q2q3=q2*q3;
    float q3q3=q3*q3;

    if(acc->x*acc->y*acc->z==0)
        return;

    norm = sqrt( acc->x * acc->x+ acc->y * acc->y + acc->z * acc->z );

    acc->x=acc->x/norm;
    acc->y=acc->y/norm;
    acc->z=acc->z/norm;

    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    
    ex = (acc->y*vz - acc->z*vy);
    ey = (acc->z*vx - acc->x*vz);
    ez = (acc->x*vy - acc->y*vx);

    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki;
    ezInt = ezInt + ez*Ki;

    gyro->x = gyro->x+Kp*ex +exInt;
    gyro->y = gyro->y+Kp*ey +eyInt;
    gyro->z = gyro->z+Kp*ez +ezInt;

    //四元数微分方程
    q0 = q0 + (-q1*gyro->x -q2*gyro->y -q3*gyro->z)*halfT;
    q1 = q1 + (q0*gyro->x +q2*gyro->z -q3*gyro->y)*halfT;
    q2 = q2 + (q0*gyro->y -q1*gyro->z +q3*gyro->x)*halfT;
    q3 = q3 + (q0*gyro->z +q1*gyro->y -q2*gyro->x)*halfT;

    norm = sqrt(q0*q0+q1*q1+q2*q2+q3*q3);

    q0=q0/norm;
    q1=q1/norm;
    q2=q2/norm;
    q3=q3/norm;

    //四元数转换成欧拉角
    //attitude->yaw += gyro->z * RadtoDeg*0.01f;//对陀螺仪做积分得偏航角

    attitude->yaw = atan2(2.f*(q1q2+q0q3),q0q0+q1q1-q2q2-q3q3)*57.3f;//这种方法肯定有漂移
    attitude->roll = asin(2*(q0*q1+q2*q3))*57.3f; //横滚角
    attitude->pitch = asin(2*(q0*q2-q1*q3))*57.3f;//俯仰角57.2957795f

    //此处也可添加修正微调值 roll+=(float)OFFSET_ROLL
}

