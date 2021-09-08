/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

理论有8个转换器，可同步转换；每个转换8个通道，精度为8/10/12位。默认为最高精度12位。
ADC端口AN可以作为输入口，不可作为输出口，与ARM单片机不同！
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_ADC_H_
#define _LQ_ADC_H_

#include "stdint.h"
#include "IfxEvadc_reg.h"
#include "IfxEvadc_regdef.h"
#include "Ifx_TypesReg.h"
#include "Evadc/Std/IfxEvadc.h"
#include "Evadc/Adc/IfxEvadc_Adc.h"
#include "Cpu\Std\Platform_Types.h"

/*
 * adc 通道 枚举   ADC0对应管脚 AN0 file IfxEvadc_PinMap.c
 */
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
#define ADC_GROUPS_NUM       12
#define ADC_CHANNELS_NUM     48
/* 获取ADC对应组 */
#define ADC_GetGroup(ADC_Channel)   ((ADC_Channel & 0xF000)>>12)
/* 获取ADC对应通道 */
#define ADC_GetChannel(ADC_Channel) ((ADC_Channel & 0x0F00)>>8)
/* 获取ADC对应管脚序列号 宏定义 */
#define ADC_GetIndex(ADC_Channel)   (unsigned char)(ADC_Channel & 0x00FF)

void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz);
unsigned short ADC_Read(ADC_Channel_t chn);
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count);


#endif /* 0_APPSW_TRICORE_APP_LQ_ADC_H_ */
