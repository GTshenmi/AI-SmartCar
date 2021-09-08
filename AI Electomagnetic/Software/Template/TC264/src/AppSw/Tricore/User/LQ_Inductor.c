/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
使用说明：
本教学演示程序适用于电磁四轮或者三轮车：
整车资源为：
模块：龙邱TC264DA核心板、配套母板、双路全桥电机驱动、双编码器、TFT1.8屏幕、单舵机、四路电感模块；
车模：三轮或者四轮均可；
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
//#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_MotorServo.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_GPT12_ENC.h>

sint16 ECPULSE1 = 0;          //速度全局变量
sint16 ECPULSE2 = 0;          //速度全局变量

//初始化函数
void InductorInit (void)
{
    ADC_InitConfig(ADC0, 1000000); //初始化
    ADC_InitConfig(ADC1, 1000000); //初始化
    ADC_InitConfig(ADC2, 1000000); //初始化
    ADC_InitConfig(ADC3, 1000000); //初始化
}

/*************************************************************************
 *  函数名称：void CCU61_CH0_IRQHandler (void)
 *  功能说明：定时读取编码器数值
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月10日
 *  备    注：
 *************************************************************************/
void CCU61_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
    /* 获取编码器值 */
    ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); //左电机 母板上编码器1，小车前进为负值
    ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); //右电机 母板上编码器2，小车前进为正值
}

/*************************************************************************
 *  函数名称：void ElectroMagneticCar(void)
 *  功能说明：电磁车双电机差速控制，简单的分段比例控制算法
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：驱动2个电机
 *************************************************************************/
void ElectroMagneticCar (void)
{
    while (1)
    {
        //用户代码
    }
}

