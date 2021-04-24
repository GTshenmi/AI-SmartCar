/*
 *  icm20602.c
 *
 *  Created on: 2021年4月22日
 *      Author: 936305695
 */
#include "icm20602.h"


#define ICM20602CS(x)    GPIOx.Write(&GPIO_Resources[24].GPION,x)
#define ICM20602CSInit() GPIOx.Init(&GPIO_Resources[24].GPION)


#define     ICM20602_SPI_W              0x00
#define     ICM20602_SPI_R              0x80

#define     ICM20602_XG_OFFS_TC_H       0x04
#define     ICM20602_XG_OFFS_TC_L       0x05
#define     ICM20602_YG_OFFS_TC_H       0x07
#define     ICM20602_YG_OFFS_TC_L       0x08
#define     ICM20602_ZG_OFFS_TC_H       0x0A
#define     ICM20602_ZG_OFFS_TC_L       0x0B
#define     ICM20602_SELF_TEST_X_ACCEL  0x0D
#define     ICM20602_SELF_TEST_Y_ACCEL  0x0E
#define     ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define     ICM20602_XG_OFFS_USRH       0x13
#define     ICM20602_XG_OFFS_USRL       0x14
#define     ICM20602_YG_OFFS_USRH       0x15
#define     ICM20602_YG_OFFS_USRL       0x16
#define     ICM20602_ZG_OFFS_USRH       0x17
#define     ICM20602_ZG_OFFS_USRL       0x18
#define     ICM20602_SMPLRT_DIV         0x19
#define     ICM20602_CONFIG             0x1A
#define     ICM20602_GYRO_CONFIG        0x1B
#define     ICM20602_ACCEL_CONFIG       0x1C
#define     ICM20602_ACCEL_CONFIG_2     0x1D
#define     ICM20602_LP_MODE_CFG        0x1E
#define     ICM20602_ACCEL_WOM_X_THR    0x20
#define     ICM20602_ACCEL_WOM_Y_THR    0x21
#define     ICM20602_ACCEL_WOM_Z_THR    0x22
#define     ICM20602_FIFO_EN            0x23
#define     ICM20602_FSYNC_INT          0x36
#define     ICM20602_INT_PIN_CFG        0x37
#define     ICM20602_INT_ENABLE         0x38
#define     ICM20602_FIFO_WM_INT_STATUS 0x39
#define     ICM20602_INT_STATUS         0x3A
#define     ICM20602_ACCEL_XOUT_H       0x3B
#define     ICM20602_ACCEL_XOUT_L       0x3C
#define     ICM20602_ACCEL_YOUT_H       0x3D
#define     ICM20602_ACCEL_YOUT_L       0x3E
#define     ICM20602_ACCEL_ZOUT_H       0x3F
#define     ICM20602_ACCEL_ZOUT_L       0x40
#define     ICM20602_TEMP_OUT_H         0x41
#define     ICM20602_TEMP_OUT_L         0x42
#define     ICM20602_GYRO_XOUT_H        0x43
#define     ICM20602_GYRO_XOUT_L        0x44
#define     ICM20602_GYRO_YOUT_H        0x45
#define     ICM20602_GYRO_YOUT_L        0x46
#define     ICM20602_GYRO_ZOUT_H        0x47
#define     ICM20602_GYRO_ZOUT_L        0x48
#define     ICM20602_SELF_TEST_X_GYRO   0x50
#define     ICM20602_SELF_TEST_Y_GYRO   0x51
#define     ICM20602_SELF_TEST_Z_GYRO   0x52
#define     ICM20602_FIFO_WM_TH1        0x60
#define     ICM20602_FIFO_WM_TH2        0x61
#define     ICM20602_SIGNAL_PATH_RESET  0x68
#define     ICM20602_ACCEL_INTEL_CTRL   0x69
#define     ICM20602_USER_CTRL          0x6A
#define     ICM20602_PWR_MGMT_1         0x6B
#define     ICM20602_PWR_MGMT_2         0x6C
#define     ICM20602_I2C_IF             0x70
#define     ICM20602_FIFO_COUNTH        0x72
#define     ICM20602_FIFO_COUNTL        0x73
#define     ICM20602_FIFO_R_W           0x74
#define     ICM20602_WHO_AM_I           0x75
#define     ICM20602_XA_OFFSET_H        0x77
#define     ICM20602_XA_OFFSET_L        0x78
#define     ICM20602_YA_OFFSET_H        0x7A
#define     ICM20602_YA_OFFSET_L        0x7B
#define     ICM20602_ZA_OFFSET_H        0x7D
#define     ICM20602_ZA_OFFSET_L        0x7E


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI写寄存器
//  @param      cmd     寄存器地址
//  @param      val     需要写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_w_reg_byte(uint8 cmd, uint8 val)
{
    uint8 dat[2];
    ICM20602CS(0);

    dat[0] = cmd | ICM20602_SPI_W;
    dat[1] = val;

    SPIx.ReadWriteBytes(&SPI_Resources[2].SPIN,dat,NULL,2,SPIx.Time_Infinite);
    ICM20602CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_r_reg_byte(uint8 cmd, uint8 *val)
{
    uint8 dat[2];
    ICM20602CS(0);

    dat[0] = cmd | ICM20602_SPI_R;
    dat[1] = *val;

    SPIx.ReadWriteBytes(&SPI_Resources[2].SPIN,dat,dat,2,SPIx.Time_Infinite);

    *val = dat[1];
    ICM20602CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI多字节读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @param      num     读取数量
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_r_reg_bytes(uint8 * val, uint8 num)
{
    ICM20602CS(0);
    SPIx.ReadWriteBytes(&SPI_Resources[2].SPIN,val,val,num,SPIx.Time_Infinite);
    ICM20602CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8_t icm20602_self3_check(void)
{
    uint8_t res = 0;
    uint8 dat=0;
    uint8_t times = 0xff;
    
    while(0x12 != dat && times--)
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);

    if(dat != 0x12)
        res = 1;
    else
        res = 0;


    return res;

}

/*
 *
 * @能够读到3个方向的加速度和角度的原始数据
 *
 * */

/*初始化*/
uint8_t ICM20602_Init()
{
    uint8_t res = 0;
    uint8_t times = 0xff;

    ICM20602CSInit();

    SPIx.Init(&SPI_Resources[2].SPIN);

    uint8_t val = 0x0;

    res = icm20602_self3_check();//检测

    if(res)
        return 1;

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//复位设备

    do
    {//等待复位成功
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
        Systime.Delayms(10);
    }while(0x41 != val && times--);

    if(val != 0x41)
       return 2;



    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //时钟设置
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //开启陀螺仪和加速度计
    icm_spi_w_reg_byte(ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x07);            //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //±2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);            //±8g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples

    return res;
}

/*读取三轴加速度*/
void ICM20602_ReadAcc(float *x,float *y,float *z)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

    buf.reg =  ICM20602_SPI_R | ICM20602_ACCEL_XOUT_H;

    icm_spi_r_reg_bytes(&buf.reg, 7);

    *x = (int16_t)(((uint16)buf.dat[0]<<8 | buf.dat[1])) * 1.0f;
    *y = (int16_t)(((uint16)buf.dat[2]<<8 | buf.dat[3])) * 1.0f;
    *z = (int16_t)(((uint16)buf.dat[4]<<8 | buf.dat[5])) * 1.0f;

//    *x = (int16_t)(((uint16)buf.dat[0]<<8 | buf.dat[1])) * 1.0f / 4096.0f;
//    *y = (int16_t)(((uint16)buf.dat[2]<<8 | buf.dat[3])) * 1.0f / 4096.0f;
//    *z = (int16_t)(((uint16)buf.dat[4]<<8 | buf.dat[5])) * 1.0f / 4096.0f;
}

/*读取三轴角度*/
void ICM20602_ReadGyro(float *x,float *y,float *z)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

    buf.reg =  ICM20602_SPI_R  | ICM20602_GYRO_XOUT_H;

    icm_spi_r_reg_bytes(&buf.reg, 7);


    *x = (int16_t)(((uint16)buf.dat[0]<<8 | buf.dat[1])) * 1.0f;
    *y = (int16_t)(((uint16)buf.dat[2]<<8 | buf.dat[3])) * 1.0f;
    *z = (int16_t)(((uint16)buf.dat[4]<<8 | buf.dat[5])) * 1.0f;

//    *x = (int16_t)(((uint16)buf.dat[0]<<8 | buf.dat[1])) * 1.0f / 16.4f;
//    *y = (int16_t)(((uint16)buf.dat[2]<<8 | buf.dat[3])) * 1.0f / 16.4f;
//    *z = (int16_t)(((uint16)buf.dat[4]<<8 | buf.dat[5])) * 1.0f / 16.4f;
}

