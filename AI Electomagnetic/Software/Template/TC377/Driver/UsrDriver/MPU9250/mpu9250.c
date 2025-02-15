//
// Created by Chi Zhang on 2021/7/1.
//

#include "mpu9250.h"

#ifdef MPU9250_DEBUG
#define MPU9250_DEBUG_PRINT(fmt,__VA_ARGS__) printf(fmt,__VA_ARGS__)
#else
#define MPU9250_DEBUG_PRINT(fmt,__VA_ARGS__)
#endif

u8 MPU_Write_Byte(u8 devaddr,u8 reg,u8 data);
u8 MPU_Read_Byte(u8 devaddr,u8 reg);
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//MPU9250驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/7/19
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//V1.1 20160902
//修改MPU6500_ID为: MPU6500_ID1,MPU6500_ID2,因为9205可能有2种ID!!!
//////////////////////////////////////////////////////////////////////////////////
static iicx_t iic;
//初始化MPU9250
//返回值:0,成功
//    其他,错误代码
u8 MPU9250_Init(void *config) {
	iic.SCLPin = &GPIO_Resources[25].GPION;
	iic.SDAPin = &GPIO_Resources[26].GPION;
	iic.IIC_Type = SoftWare_IIC;
	iic.IIC_DeviceType = IIC_Master;
	iic.BaudRate = 10;
	u8 res;
	//IIC_Init();     //初始化IIC总线
	IICx.Init(&iic);
	for (int i = 0; i < 16; ++i) {
		res = MPU_Read_Byte(MPU9250_ADDR, 0x75);
		if (res == 0x71)
			break;
		MPU9250_DEBUG_PRINT("MPU9250:WHO_AM_I=%d\n", res);
	}
	MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X80);//复位MPU9250
	Systime.Delayms(100);  //延时100ms
	MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X00);//唤醒MPU9250
	MPU_Set_Gyro_Fsr(3);                              //陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);                             //加速度传感器,±2g
	MPU_Set_Rate(500);                                 //设置采样率500Hz
	MPU_Write_Byte(MPU9250_ADDR, MPU_INT_EN_REG, 0X00);   //关闭所有中断
	MPU_Write_Byte(MPU9250_ADDR, MPU_USER_CTRL_REG, 0X00);//I2C主模式关闭
	MPU_Write_Byte(MPU9250_ADDR, MPU_FIFO_EN_REG, 0X00);    //关闭FIFO
	MPU_Write_Byte(MPU9250_ADDR, MPU_INTBP_CFG_REG, 0X82);//INT引脚低电平有效，开启bypass模式，可以直接读取磁力计
	res = MPU_Read_Byte(MPU9250_ADDR, MPU_DEVICE_ID_REG);  //读取MPU6500的ID
	if (res == MPU6500_ID1 || res == MPU6500_ID2)                //器件ID正确
	{
		MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X01);    //设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT2_REG, 0X00);    //加速度与陀螺仪都工作
		MPU_Set_Rate(50);                                       //设置采样率为50Hz
	} else return 1;

	res = MPU_Read_Byte(AK8963_ADDR, MAG_WIA);               //读取AK8963 ID
	if (res == AK8963_ID) {
		MPU_Write_Byte(AK8963_ADDR, MAG_CNTL2, 0X01);     //复位AK8963
		Systime.Delayms(50);
		MPU_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11);     //设置AK8963为单次测量
	} else return 1;

	return 0;
}

//设置MPU9250陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Gyro_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU9250_ADDR, MPU_GYRO_CFG_REG, (fsr << 3) | 3);//设置陀螺仪满量程范围
}
//设置MPU9250加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Accel_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU9250_ADDR, MPU_ACCEL_CFG_REG, fsr << 3);//设置加速度传感器满量程范围
}

//设置MPU9250的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_LPF(u16 lpf) {
	u8 data;
	if (lpf >= 188)data = 1;
	else if (lpf >= 98)data = 2;
	else if (lpf >= 42)data = 3;
	else if (lpf >= 20)data = 4;
	else if (lpf >= 10)data = 5;
	else data = 6;
	return MPU_Write_Byte(MPU9250_ADDR, MPU_CFG_REG, data);//设置数字低通滤波器
}

//设置MPU9250的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Rate(u16 rate) {
	u8 data;
	if (rate > 1000)rate = 1000;
	if (rate < 4)rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU9250_ADDR, MPU_SAMPLE_RATE_REG, data);   //设置数字低通滤波器
	return MPU_Set_LPF(rate / 2);   //自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void) {
	u8 buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU9250_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16) buf[0] << 8) | buf[1];
	temp = 21 + ((double) raw) / 333.87;
	return temp * 100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(float *gx, float *gy, float *gz) {
	u8 buf[6], res;

	short gxr,gyr,gzr;

	res = MPU_Read_Len(MPU9250_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0) {
		gxr = ((u16) buf[0] << 8) | buf[1];
		gyr = ((u16) buf[2] << 8) | buf[3];
		gzr = ((u16) buf[4] << 8) | buf[5];
	}

	*gx = gxr * 1.0;
	*gy = gyr * 1.0;
	*gz = gzr * 1.0;

	return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(float *ax, float *ay, float *az) {
	u8 buf[6], res;

	short axr,ayr,azr;

	res = MPU_Read_Len(MPU9250_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0) {
		axr = ((u16) buf[0] << 8) | buf[1];
		ayr = ((u16) buf[2] << 8) | buf[3];
		azr = ((u16) buf[4] << 8) | buf[5];
	}

	*ax = axr * 1.0;
	*ay = ayr * 1.0;
	*az = azr * 1.0;

	return res;;
}

//得到磁力计值(原始值)
//mx,my,mz:磁力计x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Magnetometer(float *mx, float *my, float *mz) {

	u8 buf[6], res;

	short mxr,myr,mzr;

	res = MPU_Read_Len(AK8963_ADDR, MAG_XOUT_L, 6, buf);
	if (res == 0) {
		mxr = ((u16) buf[1] << 8) | buf[0];
		myr = ((u16) buf[3] << 8) | buf[2];
		mzr = ((u16) buf[5] << 8) | buf[4];
	}

	*mx = mxr * 1.0;
	*my = myr * 1.0;
	*mz = mzr * 1.0;

	MPU_Write_Byte(AK8963_ADDR, MAG_CNTL1, 0X11); //AK8963每次读完以后都需要重新设置为单次测量模式

	return res;
}

//IIC连续写
//addr:器件地址
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf) {
	IICx.WriteBytesToSlave(&iic, addr << 1, reg, len, buf);
	return 0;
	//return HAL_I2C_Mem_Write(&hi2c2, addr << 1, reg,
	//                  I2C_MEMADD_SIZE_8BIT, (uint8_t *)buf, len, 10);
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf) {
	IICx.ReadBytesFromSlave(&iic, addr << 1, reg, len, buf);
	return 0;
//  return HAL_I2C_Mem_Read(&hi2c2, addr << 1, reg,
//                   I2C_MEMADD_SIZE_8BIT, buf, len, 10);
}

//IIC写一个字节
//devaddr:器件IIC地址
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 addr, u8 reg, u8 data) {
	IICx.WriteBytesToSlave(&iic, addr << 1, reg, 1, &data);
//  return HAL_I2C_Mem_Write(&hi2c2, addr << 1, reg,
//                    I2C_MEMADD_SIZE_8BIT, (uint8_t *)&data, 1, 10);
	return 0;
}

//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
u8 MPU_Read_Byte(u8 addr, u8 reg) {
	uint8_t res;
//  HAL_I2C_Mem_Read(&hi2c2, addr << 1, reg,
//                   I2C_MEMADD_SIZE_8BIT, &res, 1, 10);
	IICx.ReadBytesFromSlave(&iic, addr << 1, reg, 1, &res);
	return res;
}
