/*
 * imu.c
 *
 *  Created on: 2021年4月25日
 *      Author: 936305695
 */
#include "imu.h"
#include "driver.h"


uint8_t IMU_ReadAcc(struct imu *self,axis_t *acc)
{
    if(self->__ReadAcc__ != NULL)
        return self->__ReadAcc__(&acc->x,&acc->y,&acc->z);
    else
    {
        acc->x = 0.0;
        acc->y = 0.0;
        acc->z = 0.0;

        return 1;
    }
}

uint8_t IMU_ReadGyro(struct imu *self,axis_t *gyro)
{
    if(self->__ReadGyro__ != NULL)
        return self->__ReadGyro__(&gyro->x,&gyro->y,&gyro->z);
    else
    {
        gyro->x = 0.0;
        gyro->y = 0.0;
        gyro->z = 0.0;
        
        return 1;
    }
}

uint8_t IMU_ReadMag(struct imu *self,axis_t *mag)
{
    if(self->__ReadMag__ != NULL)
        return self->__ReadMag__(&mag->x,&mag->y,&mag->z);
    else
    {
        mag->x = 0.0;
        mag->y = 0.0;
        mag->z = 0.0;
        
        return 1;
    }
}

uint8_t IMU_Read(struct imu *self,axis_t *acc,axis_t *gyro,axis_t *mag)
{
    uint8_t res = 0;

    res += self->ReadAcc(self,acc);
    res += self->ReadGyro(self,gyro);
    res += self->ReadMag(self,mag);

    return res;
}

attitude_t IMU_GetAttitude(struct imu *self)
{
    return self->Attitude;
}

uint8_t IMU_AttitudeUpdate(struct imu *self)
{
    //float matrix[9]={1.f,0.0f,0.0f,0.0f,1.f,0.0f,0.0f,0.0f,1.f};//初始化矩阵 定高定点

    self->Read(self,&self->Acc,&self->Gyro,&self->Mag);

    axis_t *acc = &self->Acc;
    axis_t *gyro = &self->Gyro;
    axis_t *mag = &self->Mag;

    mag->x = mag->x * 1.0;

    attitude_t *attitude = &self->Attitude;

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
        return 127;

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

    return 0;
    //此处也可添加修正微调值 roll+=(float)OFFSET_ROLL
}

void IMU_SetUpdateFlags(struct imu *self,bool state)
{
    self->Is_Update = state;
}

uint8_t IMU_GetUpdateFlags(struct imu *self)
{
    return self->Is_Update;
}

uint8_t IMU_Init(struct imu *self)
{
    uint8_t res = 127;

    self->Read = IMU_Read;

    self->ReadAcc = IMU_ReadAcc;

    self->ReadGyro = IMU_ReadGyro;

    self->ReadMag = IMU_ReadMag;

    self->AttitudeUpdate = IMU_AttitudeUpdate;

    self->GetAttitude = IMU_GetAttitude;

    self->SetUpdateFlags = IMU_SetUpdateFlags;

    self->GetUpdateFlags = IMU_GetUpdateFlags;

    if(self->__Init__ != NULL)
    {
        res = self->__Init__(self->__InitConfig__);

        self->Is_Init = true;
    }

    return res;
}



