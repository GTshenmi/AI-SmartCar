/*
 * imu.h
 *
 *  Created on: 2021��4��25��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_IMU_IMU_H_
#define OS_DEVICE_IMU_IMU_H_

#include "platform.h"

typedef struct
{
   float x;
   float y;
   float z;
}axis_t;

typedef struct
{
   float roll;  //�����
   float yaw;   //ƫ����
   float pitch; //������
}attitude_t;

typedef struct imu
{
    private

    uint8_t (*__Init__)(void *config);
    void *__InitConfig__;

    uint8_t (*__ReadAcc__)(float *ax,float *ay,float *az);

    uint8_t (*__ReadGyro__)(float *gx,float *gy,float *gz);
    uint8_t (*__ReadMag__)(float *mx,float *my,float *mz);

    axis_t     Acc;
    axis_t     Gyro;
    axis_t     Mag;
    attitude_t Attitude;

    public 

    uint8_t (*Init)(struct imu *self);

    uint8_t (*ReadAcc)(struct imu *self,axis_t *acc);

    uint8_t (*ReadGyro)(struct imu *self,axis_t *gyro);
    uint8_t (*ReadMag)(struct imu *self,axis_t *mag);   

    uint8_t (*Read)(struct imu *self,axis_t *acc,axis_t *gyro,axis_t *mag);

    attitude_t (*GetAttitude)(struct imu *self);

    uint8_t (*AttitudeUpdate)(struct imu *self);

    struct imu *Self;
}imu_t;

uint8_t IMU_Init(struct imu *self);


#endif /* OS_DEVICE_IMU_IMU_H_ */
