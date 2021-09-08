/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include "stdint.h"
#include "LQ_GPIO_Cfg.h"

/*!< GPIO 配置 */
typedef enum
{
    /*转换速度字段 
    这是一个可以调整高低电平切换上升和下降时间速度的设置项，
    在一般的应用中使用芯片默认的设置就可以了。
    在需要微调SI或EMI性能时可以尝试修改配置 */
    SlewRateSlow         = 0x0,     /*!< 默认 */
    SlewRateFast         = 0x1,
    
    /* 驱动能力字段，作为输出时有效 */
    DriveStrengthDisable = 0x0,     /*!< 禁止输出强度配置 */
    DriveStrengthR0      = 0x8,     /*!< R0：260欧姆@3.3v；150欧姆@1.8v  */
    DriveStrengthR0_2    = 0x10,    /*!< R0/2 */
    DriveStrengthR0_3    = 0x18,    /*!< R0/3 */
    DriveStrengthR0_4    = 0x20,    /*!< R0/4 */
    DriveStrengthR0_5    = 0x28,    /*!< R0/5 */
    DriveStrengthR0_6    = 0x30,    /*!< 默认R0/6 */
    DriveStrengthR0_7    = 0x38,    /*!< R0/7 */
    
    /* 带宽速度 这个不是指IO翻转速度，可以通过IO口波形不失真的带宽 */
    Speed50Mhz           = 0x0,     /*!< 50MHz */
    Speed100Mhz          = 0x80,    /*!< 默认100MHz */
    Speed200Mhz          = 0xC0,    /*!< 200MHz */
    
    /* 漏极开路 作为输出时有效，依靠外部电压，提高输出能力 */
    OpenDrainDisable     = 0x0,     /*!< 默认关闭 */
    OpenDrainEnable      = 0x800,   /*!< 开启 */
    
    /* 拉/保持使能字段 */
    PullKeeperDisable    = 0x0,     /*!< 关闭 */
    PullKeeperEnable     = 0x1000,  /*!< 默认开启 */
    
    /* 选择拉或者保持 
    保持：使能输入状态保存器可以在IO供电NVCC_xxx关掉之后，使输入缓冲器的输出自动维持在关电之前的逻辑状态。其价值可以在低功耗的应用中得到体现。 
    拉：  使能输出状态保存器可以在内核供电关掉之后，使IO的输出自动维持在关电之前的逻辑状态(需要注意的是输出状态保存器不能与上下拉同时工作)。其价值同样可以体现在低功耗的应用中 */
    KeeperSelect         = 0x0,     /*!< 默认选择Keeper */
    PullSelect           = 0x2000,  /*!< 选择Pull */
        
    /* 上下拉 输入时有效 */
    Down100K             = 0x0,     /*!< 默认100K下拉电阻 */
    UP47K                = 0x4000,  /*!< 47K上拉电阻 */
    UP100K               = 0x8000,  /*!< 100K上拉电阻 */
    UP22K                = 0xC000,  /*!< 22K上拉电阻 */
        
    /* 磁滞使能字段 作为输入时有效 */
    HysteresisDisable    = 0x0,     /*!< 默认关闭 */
    HysteresisEnable     = 0x10000, /*!< 开启 */
    
    /* 输入 输出(为方便使用自己添加的，寄存器并没有这个) */
    OUTPUT               = 0x8000000,    /*!< 输出 */
    INPUT                = 0,            /*!< 输入 */
    
}PinConfig_t;


/* GPIO 常用模式 */
#define PIN_MODE_OUTPUT         OUTPUT|DriveStrengthR0_6                                           //推挽输出
#define PIN_MODE_INPUT          INPUT|DriveStrengthR0_6|OpenDrainEnable                            //浮空输入
#define PIN_MODE_INPUT_PULLUP   INPUT|DriveStrengthR0_6|PullKeeperEnable|PullSelect|UP100K         //上拉输入
#define PIN_MODE_INPUT_PULLDOWN INPUT|DriveStrengthR0_6|PullKeeperEnable|PullSelect|Down100K       //下拉输入
#define PIN_MODE_OUTPUT_OD      OUTPUT|DriveStrengthR0_6|OpenDrainEnable                           //开漏输出

/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             0x00 //上升沿（下拉）触发中断  
#define PIN_IRQ_MODE_FALLING            0x01 //下降沿（上拉）触发中断
#define PIN_IRQ_MODE_RISING_FALLING     0x02 //双边沿（开漏）触发中断
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03 //高电平（下拉）触发中断
#define PIN_IRQ_MODE_LOW_LEVEL          0x04 //低电平（上拉）触发中断



/*!
  * @brief    GPIO初始化
  *
  * @param    pin      ： 管脚标号  LQ_GPIO_Cfg.h中定义
  * @param    mode     ： GPIO 模式   LQ_GPIO.h中的宏定义中选择
  * @param    output   ： 输出模式时，输出电平 1：高电平  0：低电平
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_InitConfig(A7, PIN_MODE_OUTPUT, 1);//设置管脚A7为输出模式 高电平
  *
  * @date     2019/8/19 星期一
  */
void PIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output);



/*!
  * @brief    读取GPIO状态
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  *
  * @return   
  *
  * @note     GPIO要初始化为输入模式
  *
  * @example  PIN_Read(A7); //获取A7状态
  *
  * @date     2019/8/19 星期一
  */
uint8_t PIN_Read(GPIO_Name_t pin);



/*!
  * @brief    设置GPIO输出状态
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    output   ： 1:高电平  0：低电平
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_Write(A7, 1);//A7置为高电平
  *
  * @date     2019/8/19 星期一
  */
void PIN_Write(GPIO_Name_t pin, uint8_t output);


/*!
  * @brief    设置GPIO输入输出
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    mode     ： 1:输出  0：输入
  *
  * @return   无
  *
  * @note     使用前请先初始化
  *
  * @example  PIN_Dir(A7, 1);//A7设置为输出
  *
  * @date     2019/8/19 星期一
  */
void PIN_Dir(GPIO_Name_t pin, uint8_t mode);


/*!
  * @brief    GPIO外部中断初始化
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    mode     ： GPIO 中断模式   LQ_GPIO.h中的宏定义中选择
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_Exti(C13, PIN_IRQ_MODE_FALLING);//配置C13 下降沿触发中断
  * @example  优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
  * @example  NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31IO的中断  
  *
  * @date     2019/8/19 星期一
  */
void PIN_Exti(GPIO_Name_t pin, uint8_t mode);



/*!< RT1064的GPIO除了 GPIO1-GPIO5之外 GPIO1-GPIO4还可以映射到GPIO6-GPIO9上 
     GPIO6 - GPIO9挂载在AHB总线上 翻转速度比较快 */
#if (defined(CPU_MIMXRT1064CVL5A) || defined(CPU_MIMXRT1064DVL6A)) 
/*!
  * @brief    RT1064 快速IO初始化
  *
  * @param    pin      ： 管脚标号  LQ_GPIO_Cfg.h中定义
  * @param    mode     ： GPIO 模式   LQ_GPIO.h中的宏定义中选择
  * @param    output   ： 输出模式时，输出电平 1：高电平  0：低电平
  *
  * @return   
  *
  * @note     L6/L7/K7不能使用该函数
  *
  * @example  FASTPIN_InitConfig(A7, PIN_MODE_OUTPUT, 1);//设置管脚A7为输出模式 高电平
  *
  * @date     2019/8/19 星期一
  */
void FASTPIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output);


/*!
  * @brief    读取GPIO状态
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  *
  * @return   无
  *
  * @note     GPIO要初始化为输入模式
  *
  * @example  FASTPIN_Read(A7); //获取A7状态
  *
  * @date     2019/8/19 星期一
  */
uint8_t FASTPIN_Read(GPIO_Name_t pin);


/*!
  * @brief    设置GPIO输出状态
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    output   ： 1:高电平  0：低电平
  *
  * @return   无
  *
  * @note     
  *
  * @example  FASTPIN_Write(A7, 1);//A7置为高电平
  *
  * @date     2019/8/19 星期一
  */
void FASTPIN_Write(GPIO_Name_t pin, uint8_t output);


/*!
  * @brief    设置GPIO输入输出
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    mode     ： 1:输出  0：输入
  *
  * @return   无
  *
  * @note     使用前请先初始化
  *
  * @example  FASTPIN_Dir(A7, 1);//A7设置为输出
  *
  * @date     2019/8/19 星期一
  */
void FASTPIN_Dir(GPIO_Name_t pin, uint8_t mode);


/*!
  * @brief    GPIO外部中断初始化
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    mode     ： GPIO 中断模式   LQ_GPIO.h中的宏定义中选择
  *
  * @return   
  *
  * @note     在映射关系数组中，如果原来是GPIO1，使用此函数开启的中断 则用的是GPIO6
  * @note     GPIO1  ---> GPIO6
  * @note     GPIO2  ---> GPIO7
  * @note     GPIO3  ---> GPIO8
  * @note     GPIO4  ---> GPIO9
  *
  * @example  FASTPIN_Exti(C13, PIN_IRQ_MODE_FALLING);//配置C13 下降沿触发中断
  * @example  优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
  * @example  NVIC_SetPriority(GPIO,7_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO7_Combined_16_31_IRQn);			          //使能GPIO7_16~31IO的中断  
  *
  * @date     2019/8/19 星期一
  */
void FASTPIN_Exti(GPIO_Name_t pin, uint8_t mode);

#endif

#endif  
