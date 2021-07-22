//
// Created by Chi Zhang on 2021/7/1.
//

#include "mpu9250.h"

u8 MPU_Write_Byte(u8 devaddr,u8 reg,u8 data);
u8 MPU_Read_Byte(u8 devaddr,u8 reg);
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//MPU9250��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/7/19
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//V1.1 20160902
//�޸�MPU6500_IDΪ: MPU6500_ID1,MPU6500_ID2,��Ϊ9205������2��ID!!!
//////////////////////////////////////////////////////////////////////////////////
static iicx_t iic;
//��ʼ��MPU9250
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU9250_Init(void *config) {
	iic.SCLPin = &GPIO_Resources[25].GPION;
	iic.SDAPin = &GPIO_Resources[26].GPION;
	iic.IIC_Type = SoftWare_IIC;
	iic.IIC_DeviceType = IIC_Master;
	iic.BaudRate = 1000;
	u8 res;
	//IIC_Init();     //��ʼ��IIC����
	IICx.Init(&iic);
	for (int i = 0; i < 16; ++i) {
		res = MPU_Read_Byte(MPU9250_ADDR, 0x75);
		printf("WHO AM I:%d\n", res);
		if (res == 0x75)
			break;
	}
	MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X80);//��λMPU9250
	Systime.Delayms(100);  //��ʱ100ms
	MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X00);//����MPU9250
	MPU_Set_Gyro_Fsr(3);                              //�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);                             //���ٶȴ�����,��2g
	MPU_Set_Rate(50);                                 //���ò�����50Hz
	MPU_Write_Byte(MPU9250_ADDR, MPU_INT_EN_REG, 0X00);   //�ر������ж�
	MPU_Write_Byte(MPU9250_ADDR, MPU_USER_CTRL_REG, 0X00);//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU9250_ADDR, MPU_FIFO_EN_REG, 0X00);    //�ر�FIFO
	MPU_Write_Byte(MPU9250_ADDR, MPU_INTBP_CFG_REG, 0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ������ֱ�Ӷ�ȡ������
	res = MPU_Read_Byte(MPU9250_ADDR, MPU_DEVICE_ID_REG);  //��ȡMPU6500��ID
	if (res == MPU6500_ID1 || res == MPU6500_ID2)                //����ID��ȷ
	{
		MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X01);    //����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT2_REG, 0X00);    //���ٶ��������Ƕ�����
		MPU_Set_Rate(50);                                       //���ò�����Ϊ50Hz
	} else return 1;

	res = MPU_Read_Byte(AK8963_ADDR, MAG_WIA);               //��ȡAK8963 ID
	if (res == AK8963_ID) {
		MPU_Write_Byte(AK8963_ADDR, MAG_CNTL2, 0X01);     //��λAK8963
		Systime.Delayms(50);
		MPU_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11);     //����AK8963Ϊ���β���
	} else return 1;

	return 0;
}

//����MPU9250�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Gyro_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU9250_ADDR, MPU_GYRO_CFG_REG, (fsr << 3) | 3);//���������������̷�Χ
}
//����MPU9250���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Accel_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU9250_ADDR, MPU_ACCEL_CFG_REG, fsr << 3);//���ü��ٶȴ����������̷�Χ
}

//����MPU9250�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_LPF(u16 lpf) {
	u8 data;
	if (lpf >= 188)data = 1;
	else if (lpf >= 98)data = 2;
	else if (lpf >= 42)data = 3;
	else if (lpf >= 20)data = 4;
	else if (lpf >= 10)data = 5;
	else data = 6;
	return MPU_Write_Byte(MPU9250_ADDR, MPU_CFG_REG, data);//�������ֵ�ͨ�˲���
}

//����MPU9250�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Rate(u16 rate) {
	u8 data;
	if (rate > 1000)rate = 1000;
	if (rate < 4)rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU9250_ADDR, MPU_SAMPLE_RATE_REG, data);   //�������ֵ�ͨ�˲���
	return MPU_Set_LPF(rate / 2);   //�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void) {
	u8 buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU9250_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16) buf[0] << 8) | buf[1];
	temp = 21 + ((double) raw) / 333.87;
	return temp * 100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(float *gx, float *gy, float *gz) {
	u8 buf[6], res = 0;
	res = MPU_Read_Len(MPU9250_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0) {
		*gx = (((u16) buf[0] << 8) | buf[1]) * 1.0;
		*gy = (((u16) buf[2] << 8) | buf[3]) * 1.0;
		*gz = (((u16) buf[4] << 8) | buf[5]) * 1.0;
	}
	return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(float *ax, float *ay, float *az) {
	u8 buf[6], res = 0;
	res = MPU_Read_Len(MPU9250_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0) {
		*ax = (((u16) buf[0] << 8) | buf[1]) * 1.0;
		*ay = (((u16) buf[2] << 8) | buf[3]) * 1.0;
		*az = (((u16) buf[4] << 8) | buf[5]) * 1.0;
	}
	return res;
}

//�õ�������ֵ(ԭʼֵ)
//mx,my,mz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Magnetometer(float *mx, float *my, float *mz) {
	u8 buf[6], res = 0;
	res = MPU_Read_Len(AK8963_ADDR, MAG_XOUT_L, 6, buf);
	if (res == 0) {
		*mx = (((u16) buf[1] << 8) | buf[0]) * 1.0;
		*my = (((u16) buf[3] << 8) | buf[2]) * 1.0;
		*mz = (((u16) buf[5] << 8) | buf[4]) * 1.0;
	}
	MPU_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11); //AK8963ÿ�ζ����Ժ���Ҫ��������Ϊ���β���ģʽ

	return res;
}

//IIC����д
//addr:������ַ
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf) {
	IICx.WriteBytesToSlave(&iic, addr << 1, reg, len, buf);
	return 0;
	//return HAL_I2C_Mem_Write(&hi2c2, addr << 1, reg,
	//                  I2C_MEMADD_SIZE_8BIT, (uint8_t *)buf, len, 10);
}

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf) {
	return IICx.ReadBytesFromSlave(&iic, addr << 1, reg, len, buf);
	//return 0;
//  return HAL_I2C_Mem_Read(&hi2c2, addr << 1, reg,
//                   I2C_MEMADD_SIZE_8BIT, buf, len, 10);
}

//IICдһ���ֽ�
//devaddr:����IIC��ַ
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 addr, u8 reg, u8 data) {
	return IICx.WriteBytesToSlave(&iic, addr << 1, reg, 1, &data);
//  return HAL_I2C_Mem_Write(&hi2c2, addr << 1, reg,
//                    I2C_MEMADD_SIZE_8BIT, (uint8_t *)&data, 1, 10);
	//return 0;
}

//IIC��һ���ֽ�
//reg:�Ĵ�����ַ
//����ֵ:����������
u8 MPU_Read_Byte(u8 addr, u8 reg) {
	uint8_t res;
//  HAL_I2C_Mem_Read(&hi2c2, addr << 1, reg,
//                   I2C_MEMADD_SIZE_8BIT, &res, 1, 10);
	IICx.ReadBytesFromSlave(&iic, addr << 1, reg, 1, &res);

	return res;
}
