/*
 * common.h
 *
 *  Created on: 2021年4月1日
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_COMMON_H_
#define DRIVER_SYSDRIVER_COMMON_H_

#include "ifx_hal.h"
#include "interrupt.h"

/*
 *
 * ADC:
 *
 * */
#define ADC_GROUPS_NUM       12
#define ADC_CHANNELS_NUM     48

/* 获取ADC对应组 */
#define ADC_GetGroup(ADC_Channel)   ((ADC_Channel & 0xF000)>>12)
/* 获取ADC对应通道 */
#define ADC_GetChannel(ADC_Channel) ((ADC_Channel & 0x0F00)>>8)
/* 获取ADC对应管脚序列号 宏定义 */
#define ADC_GetIndex(ADC_Channel)   (unsigned char)(ADC_Channel & 0x00FF)

typedef enum
{
    ADC0 = 0x0000, ADC1 = 0x0101, ADC2 = 0x0202, ADC3 = 0x0303, ADC4 = 0x0404, ADC5 = 0x0505, ADC6 = 0x0606, ADC7 = 0x0707,

    ADC8 = 0x1008, ADC9 = 0x1109, ADC10= 0x120A, ADC11= 0x130B, ADC12= 0x140C, ADC13= 0x150D, ADC14= 0x160E, ADC15= 0x170F,

    ADC16= 0x2010, ADC17= 0x2111, ADC18= 0x2212, ADC19= 0x2313, ADC20= 0x2414, ADC21= 0x2515, ADC22= 0x2616, ADC23= 0x2717,

    ADC24= 0x3018, ADC25= 0x3119, ADC26= 0x321A, ADC27= 0x331B, ADC28= 0x341C, ADC29= 0x351D, ADC30= 0x361E, ADC31= 0x371F,

    ADC32= 0x8020, ADC33= 0x8121, ADC34= 0x8222, ADC35= 0x8323, ADC36= 0x8424, ADC37= 0x8525, ADC38= 0x8626, ADC39= 0x8727,

    ADC40= 0x8828, ADC41= 0x8929, ADC42= 0x8A2A,ADC43= 0x8B2B,ADC44= 0x8C2C, ADC45= 0x8D2D, ADC46= 0x8E2E, ADC47= 0x8F2F,
    //9--11用不到就不写了，需要的自己查看手册
}ADC_Channel_t;

/*
 *
 * ENC:
 *
 * */

/**
* @brief ENC 脉冲管脚
*/
typedef enum
{
  ENC2_InPut_P00_7 = 0x0200, ENC2_InPut_P33_7 = 0x0201,//00.7摄像头像素占用
  ENC3_InPut_P02_6 = 0x0300, ENC3_InPut_P10_4 = 0x0301,//02.6摄像头数据口占用
  ENC4_InPut_P02_8 = 0x0400, ENC4_InPut_P10_8 = 0x0401,//
  ENC5_InPut_P10_3 = 0x0500, ENC5_InPut_P21_7 = 0x0501,//
  ENC6_InPut_P20_3 = 0x0600, ENC6_InPut_P10_2 = 0x0601 //
}ENC_InputPin_t;

/**
* @brief ENC 方向管脚
*/
typedef enum
{
  ENC2_Dir_P00_8 = 0x0200, ENC2_Dir_P33_6 = 0x0201,   //00.8摄像头占用
  ENC3_Dir_P02_7 = 0x0300, ENC3_Dir_P10_7 = 0x0301,   //02.7摄像头数据口占用
  ENC4_Dir_P00_9 = 0x0400, ENC4_Dir_P33_5 = 0x0401,   //00.9摄像头占用
  ENC5_Dir_P10_1 = 0x0500, ENC5_Dir_P21_6 = 0x0501,
  ENC6_Dir_P20_0 = 0x0600, ENC6_Dir_P10_0 = 0x0601,
}ENC_DirPin_t;


#define ENC_GetTimer(pin)    (unsigned char)(pin >> 8)


/*
 *
 * GPIO:
 *
 * */

// GPIO 枚举端口编号，参考TC397datasheet更新，2020年11月16日 10:11:47
typedef enum //
{
    P00_0 = 0xA000,
    P00_1 = 0xA001,
    P00_2 = 0xA002,
    P00_3 = 0xA003,
    P00_4 = 0xA004,
    P00_5 = 0xA005,
    P00_6 = 0xA006,
    P00_7 = 0xA007,
    P00_8 = 0xA008,
    P00_9 = 0xA009,
    P00_10 = 0xA00A,
    P00_11 = 0xA00B,
    P00_12 = 0xA00C,

    P01_3 = 0xA103,
    P01_4 = 0xA104,
    P01_5 = 0xA105,
    P01_6 = 0xA106,
    P01_7 = 0xA107,

    P02_0 = 0xA200,
    P02_1 = 0xA201,
    P02_2 = 0xA202,
    P02_3 = 0xA203,
    P02_4 = 0xA204,
    P02_5 = 0xA205,
    P02_6 = 0xA206,
    P02_7 = 0xA207,
    P02_8 = 0xA208,
    P02_9 = 0xA209,
    P02_10 = 0xA20A,
    P02_11 = 0xA20B,

    P10_0 = 0xAA00,
    P10_1 = 0xAA01,
    P10_2 = 0xAA02,
    P10_3 = 0xAA03,
    P10_4 = 0xAA04,
    P10_5 = 0xAA05,
    P10_6 = 0xAA06,
    P10_7 = 0xAA07,
    P10_8 = 0xAA08,

    P11_0 = 0xAB00,
    P11_1 = 0xAB01,
    P11_2 = 0xAB02,
    P11_3 = 0xAB03,
    P11_4 = 0xAB04,
    P11_5 = 0xAB05,
    P11_6 = 0xAB06,
    P11_7 = 0xAB07,
    P11_8 = 0xAB08,
    P11_9 = 0xAB09,
    P11_10 = 0xAB0A,
    P11_11 = 0xAB0B,
    P11_12 = 0xAB0C,
    P11_13 = 0xAB0D,
    P11_14 = 0xAB0E,
    P11_15 = 0xAB0F,

    P12_0 = 0xAC00,
    P12_1 = 0xAC01,
    P13_0 = 0xAD00,
    P13_1 = 0xAD01,
    P13_2 = 0xAD02,
    P13_3 = 0xAD03,

    P14_0 = 0xAE00,
    P14_1 = 0xAE01,
    P14_2 = 0xAE02,
    P14_3 = 0xAE03,
    P14_4 = 0xAE04,
    P14_5 = 0xAE05,
    P14_6 = 0xAE06,
    P14_7 = 0xAE07,
    P14_8 = 0xAE08,
    P14_9 = 0xAE09,
    P14_10 = 0xAE0A,

    P15_0 = 0xAF00,
    P15_1 = 0xAF01,
    P15_2 = 0xAF02,
    P15_3 = 0xAF03,
    P15_4 = 0xAF04,
    P15_5 = 0xAF05,
    P15_6 = 0xAF06,
    P15_7 = 0xAF07,
    P15_8 = 0xAF08,

    P20_0 = 0xB400,
    P20_1 = 0xB401,
    P20_2 = 0xB402,
    P20_3 = 0xB403,
    P20_4 = 0xB404,
    P20_5 = 0xB405,
    P20_6 = 0xB406,
    P20_7 = 0xB407,
    P20_8 = 0xB408,
    P20_9 = 0xB409,
    P20_10 = 0xB40A,
    P20_11 = 0xB40B,
    P20_12 = 0xB40C,
    P20_13 = 0xB40D,
    P20_14 = 0xB40E,

    P21_0 = 0xB500,
    P21_1 = 0xB501,
    P21_2 = 0xB502,
    P21_3 = 0xB503,
    P21_4 = 0xB504,
    P21_5 = 0xB505,
    P21_6 = 0xB506,
    P21_7 = 0xB507,

    P22_0 = 0xB600,
    P22_1 = 0xB601,
    P22_2 = 0xB602,
    P22_3 = 0xB603,
    P22_4 = 0xB604,
    P22_5 = 0xB605,
    P22_6 = 0xB606,
    P22_7 = 0xB607,
    P22_8 = 0xB608,
    P22_9 = 0xB609,
    P22_10 = 0xB60A,
    P22_11 = 0xB60B,
    P22_12 = 0xB60C,

    P23_1 = 0xB701,
    P23_2 = 0xB702,
    P23_3 = 0xB703,
    P23_4 = 0xB704,
    P23_5 = 0xB705,
    P23_6 = 0xB706,
    P23_7 = 0xB707,

    P32_0 = 0xC000,
    P32_1 = 0xC001,
    P32_2 = 0xC002,
    P32_3 = 0xC003,
    P32_4 = 0xC004,
    P32_5 = 0xC005,
    P32_6 = 0xC006,
    P32_7 = 0xC007,

    P33_0 = 0xC100,
    P33_1 = 0xC101,
    P33_2 = 0xC102,
    P33_3 = 0xC103,
    P33_4 = 0xC104,
    P33_5 = 0xC105,
    P33_6 = 0xC106,
    P33_7 = 0xC107,
    P33_8 = 0xC108,
    P33_9 = 0xC109,
    P33_10 = 0xC10A,
    P33_11 = 0xC10B,
    P33_12 = 0xC10C,
    P33_13 = 0xC10D,
    P33_14 = 0xC10E,
    P33_15 = 0xC10F,

    P34_0 = 0xC200,
    P34_1 = 0xC201,
    P34_2 = 0xC202,
    P34_3 = 0xC203,
    P34_4 = 0xC204,
    P34_5 = 0xC205,

    P40_0 = 0xC800,
    P40_1 = 0xC801,
    P40_2 = 0xC802,
    P40_3 = 0xC803,
    P40_4 = 0xC804,
    P40_5 = 0xC805,
    P40_6 = 0xC806,
    P40_7 = 0xC807,
    P40_8 = 0xC808,
    P40_9 = 0xC809,
    P40_10 = 0xC80A,
    P40_11 = 0xC80B,
    P40_12 = 0xC80C,
    P40_13 = 0xC80D,
    P40_14 = 0xC80E,

} GPIO_Name_t;

/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(GPIO_NAME) (Ifx_P*)(0xF0030000u | (GPIO_NAME & 0xFF00))

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(GPIO_NAME)   (uint8)(GPIO_NAME & 0x000F)

/* GPIO模式 */
#define PIN_MODE_OUTPUT          IfxPort_Mode_outputPushPullGeneral    /*!< 推挽输出  */
#define PIN_MODE_OUTPUT_OD       IfxPort_Mode_outputOpenDrainGeneral   /*!< 开漏输出  */
#define PIN_MODE_INPUT           IfxPort_Mode_inputNoPullDevice        /*!< 浮空输入  */
#define PIN_MODE_INPUT_PULLUP    IfxPort_Mode_inputPullUp              /*!< 上拉输入  */
#define PIN_MODE_INPUT_PULLDOWN  IfxPort_Mode_inputPullDown            /*!< 下拉输入  */

/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             IfxPort_InputMode_pullDown     /*!< 上升沿（下拉）触发中断 */
#define PIN_IRQ_MODE_FALLING            IfxPort_InputMode_pullUp       /*!< 下降沿（上拉）触发中断 */
#define PIN_IRQ_MODE_RISING_FALLING     IfxPort_InputMode_noPullDevice /*!< 双边沿（开漏）触发中断 */

/*
 *
 * PWM:
 *
 * */

// ATOM 时钟 100MHz
#define ATOM_PWM_CLK  100000000

// ATOM 最大占空比 可自行修改
#define ATOM_PWM_MAX  10000.0f

// TOM 时钟 6.25MHz/2
#define TOM_PWM_CLK  6250000

// TOM 最大占空比 可自行修改
#define TOM_PWM_MAX  10000.0f


/*
 *
 * SPI:
 *
 * */

/**
 * QSPI CLK管脚枚举
 */
typedef enum
{
    QSPI0_CLK_P20_11= 0x0000, QSPI0_CLK_P20_13= 0x0001,
    QSPI1_CLK_P10_2 = 0x0100, QSPI1_CLK_P11_6 = 0x0101,
    QSPI2_CLK_P15_3 = 0x0202, QSPI2_CLK_P15_6 = 0x0203, QSPI2_CLK_P15_8 = 0x0204,
    QSPI3_CLK_P02_7 = 0x0300, QSPI3_CLK_P22_3 = 0x0304, QSPI3_CLK_P33_11= 0x0305,
    QSPI_CLK_NULL   = 0x0003
}QSPI_CLK_t;

/**
 * QSPI MISO/MRST 管脚枚举
 */
typedef enum
{
    QSPI0_MISO_P20_12= 0x0000,
    QSPI1_MISO_P10_1 = 0x0100, QSPI1_MISO_P11_3 = 0x0101,
    QSPI2_MISO_P15_4 = 0x0200, QSPI2_MISO_P15_7 = 0x0201,   QSPI2_MISO_P15_2  = 0x0204,
    QSPI3_MISO_P02_5 = 0x0300, QSPI3_MISO_P33_13 = 0x0303,  QSPI3_MISO_P22_1  = 0x0304,
    QSPI_MISO_NULL   = 0x0003
}QSPI_MISO_t;

/**
 * QSPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
    QSPI0_MOSI_P20_12= 0x0000, QSPI0_MOSI_P20_14= 0x0001,
    QSPI1_MOSI_P10_1 = 0x0100, QSPI1_MOSI_P10_3 = 0x0101, QSPI1_MOSI_P11_9 = 0x0103,
    QSPI2_MOSI_P15_5 = 0x0202, QSPI2_MOSI_P15_6 = 0x0203,
    QSPI3_MOSI_P02_6 = 0x0300, QSPI3_MOSI_PP10_6= 0x0301, QSPI3_MOSI_P22_0 = 0x0302,QSPI3_MOSI_P33_12= 0x0305,
    QSPI_MOSI_NULL   = 0x0003
}QSPI_MOSI_t;

/**
 * QSPI 片选管脚枚举
 */
typedef enum
{
    QSPI0_CS_P20_8 = 0x0000,  QSPI0_CS_P20_9 = 0x0001, QSPI0_CS_P20_13= 0x0002, QSPI0_CS_P11_10= 0x0003, QSPI0_CS_P11_11= 0x0004, QSPI0_CS_P11_2 = 0x0005, QSPI0_CS_P20_10= 0x0006, QSPI0_CS_P33_5 = 0x0007, QSPI0_CS_P20_6 = 0x0008, QSPI0_CS_P20_3 = 0x0009, QSPI0_CS_P15_0 = 0x000D,
    QSPI1_CS_P20_8 = 0x0100,  QSPI1_CS_P20_9 = 0x0101, QSPI1_CS_P20_13= 0x0102, QSPI1_CS_P11_10= 0x0103, QSPI1_CS_P11_11= 0x0104, QSPI1_CS_P11_2 = 0x0105, QSPI1_CS_P33_10= 0x0106, QSPI1_CS_P33_5 = 0x0107, QSPI1_CS_P10_4 = 0x0108, QSPI1_CS_P10_5 = 0x0109, QSPI1_CS_P10_0 = 0x010A,
    QSPI2_CS_P15_2 = 0x0200,  QSPI2_CS_P14_2 = 0x0201, QSPI2_CS_P14_6 = 0x0202, QSPI2_CS_P14_3 = 0x0203, QSPI2_CS_P15_1 = 0x0205, QSPI2_CS_P33_13= 0x0206, QSPI2_CS_P20_10= 0x0207, QSPI2_CS_P20_6 = 0x0208, QSPI2_CS_P20_3 = 0x0209,
    QSPI3_CS_P02_4 = 0x0300,  QSPI3_CS_P02_0 = 0x0301, QSPI3_CS_P02_1 = 0x0302, QSPI3_CS_P02_2 = 0x0303, QSPI3_CS_P02_3 = 0x0304, QSPI3_CS_P02_8 = 0x0305, QSPI3_CS_P00_8 = 0x0306, QSPI3_CS_P00_9 = 0x0307, QSPI3_CS_P10_5 = 0x0308, QSPI3_CS_P33_10= 0x030B, QSPI3_CS_P22_2= 0x030C, QSPI3_CS_P23_1= 0x030D,
}QSPI_CS_t;

/**
 * QSPI 枚举
 */
typedef enum
{
    QSPI0,
    QSPI1,
    QSPI2,
    QSPI3
}QSPI_t;

#define QSPI_GetModule(pin)    (uint8)(pin >> 8)
#define QSPI_GetIndex(pin)    (uint8)(pin & 0x0f)


/**
 * SPI CLK管脚枚举
 */
typedef enum
{
    SPI0_CLK_P14_0 = 0x0000, SPI0_CLK_P15_2 = 0x0001,
    SPI1_CLK_P15_0 = 0x0100, SPI1_CLK_P20_10= 0x0101, SPI1_CLK_P33_11= 0x0102, SPI1_CLK_P33_12= 0x0103,
    SPI2_CLK_P02_4 = 0x0200, SPI2_CLK_P10_6 = 0x0201, SPI2_CLK_P14_2 = 0x0202, SPI2_CLK_P33_7 = 0x0203, SPI2_CLK_P33_9 = 0x0204,
    SPI3_CLK_P00_0 = 0x0300, SPI3_CLK_P00_2 = 0x0301, SPI3_CLK_P15_6 = 0x0302, SPI3_CLK_P15_8 = 0x0303, SPI3_CLK_P20_0 = 0x0304, SPI3_CLK_P21_5 = 0x0305,
}SPI_CLK_t;

/**
 * SPI MISO/MRST 管脚枚举
 */
typedef enum
{
    SPI0_MISO_P14_1 = 0x0000, SPI0_MISO_P15_3 = 0x0001,
    SPI1_MISO_P15_1 = 0x0100, SPI1_MISO_P15_5 = 0x0101, SPI1_MISO_P20_9 = 0x0102,SPI1_MISO_P11_10 = 0x0104, SPI1_MISO_P33_13 = 0x0105, SPI1_MISO_P02_3 = 0x0106,
    SPI2_MISO_P14_3 = 0x0200, SPI2_MISO_P02_1 = 0x0201, SPI2_MISO_P10_6 = 0x0203, SPI2_MISO_P33_8  = 0x0204,SPI2_MISO_P02_0 = 0x0206,
    SPI3_MISO_P15_7 = 0x0300, SPI3_MISO_P20_3 = 0x0302,SPI3_MISO_P00_1  = 0x0304, SPI3_MISO_P21_6  = 0x0305,
    SPI3_MISO_NULL  = 0x0004
}SPI_MISO_t;

/**
 * SPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
    SPI0_MOSI_P14_0 = 0x0000, SPI0_MOSI_P14_1 = 0x0001, SPI0_MOSI_P15_2 = 0x0002, SPI0_MOSI_P15_3 = 0x0003,
    SPI1_MOSI_P02_2 = 0x0100, SPI1_MOSI_P11_12= 0x0101, SPI1_MOSI_P14_10= 0x0102, SPI1_MOSI_P15_0 = 0x0103, SPI1_MOSI_P15_1 = 0x0104, SPI1_MOSI_P15_4 = 0x0105, SPI1_MOSI_P15_5 = 0x0106, SPI1_MOSI_P20_10= 0x0107, SPI1_MOSI_P33_12 = 0x0108,
    SPI2_MOSI_P10_5 = 0x0200, SPI2_MOSI_P14_2 = 0x0201, SPI2_MOSI_P14_3 = 0x0202, SPI2_MOSI_P33_8 = 0x0203, SPI2_MOSI_P33_9 = 0x0204,
    SPI3_MOSI_P00_0 = 0x0300, SPI3_MOSI_P00_1 = 0x0301, SPI3_MOSI_P15_6 = 0x0302, SPI3_MOSI_P15_7 = 0x0303, SPI3_MOSI_P20_0 = 0x0304, SPI3_MOSI_P20_3 = 0x0305,
    SPI3_MOSI_NULL  = 0x0004
}SPI_MOSI_t;

/**
 * SPI 片选管脚枚举
 */
typedef enum
{
    SPI0_CS_Null  = 0x0000,
    SPI1_CS_P14_3 = 0x0100,  SPI1_CS_P20_8 = 0x0101, SPI1_CS_P33_10= 0x0102,
    SPI2_CS_P02_3 = 0x0200,  SPI2_CS_P10_5 = 0x0201, SPI2_CS_P33_6 = 0x0202,
    SPI3_CS_P00_3 = 0x0300,  SPI3_CS_P14_3 = 0x0301, SPI3_CS_P21_2 = 0x0302, SPI3_CS_P21_6 = 0x0303,
}SPI_CS_t;

/**
 * SPI 枚举
 */
typedef enum
{
    SPI0,
    SPI1,
    SPI2,
    SPI3
}SPI_t;


#define SPI_GetModule(pin)    (uint8)(pin >> 8)
#define SPI_GetIndex(pin)    (uint8)(pin & 0x0f)


/*
 *
 * TIM:
 *
 * */

typedef enum
{
    CCU60,
    CCU61
}CCU6_t;

/**
 *  CCU6通道枚举
 */
typedef enum
{
    CCU6_Channel0,
    CCU6_Channel1,
}CCU6_Channel_t;

/**
 *  STM模块枚举
 */
typedef enum
{
    STM0 = 0,
    STM1
}STM_t;

/**
 *  STM通道枚举
 */
typedef enum
{
    STM_Channel_0 = 0,
    STM_Channel_1
}STM_Channel_t;


/*
 *
 * UART:
 *
 * */

#define ASC_TX_BUFFER_SIZE 64        //发送缓冲区长度
#define ASC_RX_BUFFER_SIZE 64        //接收缓冲区长度


/**
  * @brief UART 枚举
  */
typedef enum
{
    UART0 = 0,
    UART1,
    UART2,
    UART3
}UART_t;

//L.Q UART RX 管脚枚举
typedef enum
{
    UART0_RX_P14_1 = 0xAE01, UART0_RX_P15_3 = 0xAF03,

    UART1_RX_P02_3 = 0xA203, UART1_RX_P11_10= 0xAB0A, UART1_RX_P15_1 = 0xAF01, UART1_RX_P15_5 = 0xAF05, UART1_RX_P20_9 = 0xB409, UART1_RX_P33_13= 0xC10D,

    UART2_RX_P02_0 = 0xA200, UART2_RX_P02_1 = 0xA201, UART2_RX_P10_6 = 0xAA06, UART2_RX_P14_3 = 0xAE03, UART2_RX_P33_8 = 0xC108,

    UART3_RX_P00_1 = 0xA001, UART3_RX_P15_7 = 0xAF07, UART3_RX_P20_3 = 0xB403, UART3_RX_P21_6 = 0xB506

}UART_RX_t;


//L.Q UART TX 管脚枚举
typedef enum
{
    UART0_TX_P14_0 = 0xAE00, UART0_TX_P14_1 = 0xAE01, UART0_TX_P15_2 = 0xAF02, UART0_TX_P15_3 = 0xAF03,

    UART1_TX_P02_2 = 0xA202, UART1_TX_P11_12= 0xAB0C, UART1_TX_P15_0 = 0xAF00, UART1_TX_P15_1 = 0xAF01, UART1_TX_P15_4 = 0xAF04, UART1_TX_P15_5 = 0xAF05, UART1_TX_P20_10= 0xB40A, UART1_TX_P33_12= 0xC10C, UART1_TX_P33_13 = 0xC10D,

    UART2_TX_P02_0 = 0xA200, UART2_TX_P10_5 = 0xAA05, UART2_TX_P14_2 = 0xAE02, UART2_TX_P14_3 = 0xAE03, UART2_TX_P33_8 = 0xC108, UART2_TX_P33_9 = 0xC109,

    UART3_TX_P00_0 = 0xA000, UART3_TX_P00_1 = 0xA001, UART3_TX_P15_6 = 0xAF06, UART3_TX_P15_7 = 0xAF07, UART3_TX_P20_0 = 0xB400, UART3_TX_P20_3 = 0xB403, UART3_TX_P21_7 = 0xB507

}UART_TX_t;


extern IfxEvadc_Adc vadc;
extern Ifx_EVADC_G_RES conversionResult;
extern IfxEvadc_Adc_Group g_AdcGroup[ADC_GROUPS_NUM];
extern char adc_init_flag;
extern char adc_group_init_flag[ADC_GROUPS_NUM];
extern IfxEvadc_Adc_Channel g_AdcChannel[ADC_CHANNELS_NUM];



/** 外部中断CPU标号 */
extern const unsigned char PinIrqVectabNum[4];

/** 外部中断优先级 */
extern const unsigned char PinIrqPriority[4];

/** 外部中断服务函数地址 */
extern const void *PinIrqFuncPointer[4];



extern IfxGtm_Atom_Pwm_Driver g_AtomDriverPWM[32];
extern IfxGtm_Atom_Pwm_Config g_atomConfig;
extern IfxGtm_Tom_Pwm_Config tomConfig;
extern IfxGtm_Tom_Pwm_Driver tomDriverPWM[32];


extern const unsigned char QSpiIrqVectabNum[4];
/** QSPI中断优先级 */
extern const unsigned char QSpiIrqPriority[12];

/** QSPI 配置结构体 */
extern IfxQspi_SpiMaster         g_QSPIConfig[4];
extern IfxQspi_SpiMaster_Channel g_QspiMasterChannel[4];
extern const void *QSPIIrqFuncPointer[12];


/** SPI中断CPU标号 */
extern const unsigned char SpiIrqVectabNum[4];

/** SPI中断优先级 */
extern const unsigned char SpiIrqPriority[12];
extern const void *SpiIrqFuncPointer[12];
extern IfxAsclin_Spi g_SpiConfig[4];


extern const uint8 StmIrqVectabNum[2];

/** 外部中断优先级 */
extern const uint8 StmIrqPriority[4];

/** 外部中断服务函数地址 */
extern const void *StmIrqFuncPointer[4];

extern IfxStm_CompareConfig g_StmCompareConfig[4];

extern const uint8 Ccu6IrqVectabNum[2];
extern const uint8 Ccu6IrqPriority[4] ;
extern const void *Ccu6IrqFuncPointer[4];

extern IfxAsclin_Asc g_UartConfig[4];
extern uint8 s_AscTxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
extern uint8 s_AscRxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/** UART中断CPU标号 */
extern const uint8_t UartIrqVectabNum[4];

/** UART中断优先级 */
extern const uint8_t UartIrqPriority[12];
/** UART中断服务函数地址 */
extern const void *UartIrqFuncPointer[12];


/*
 *
 * SoftWare Interrupt.
 *
 * */
/**
 * 软件中的序号枚举
 */
typedef enum
{
    SoftIRQ0,
    SoftIRQ1,
    SoftIRQ2,
    SoftIRQ3
}SOFT_IRQ;

extern const uint8 CpuSoftIrqPriority[12];
extern const void *CpuSoftIrqFuncPointer[12];

extern App_Cpu0 g_AppCpu0;

#endif /* DRIVER_SYSDRIVER_COMMON_H_ */
