# AI-SmartCar
 2021 National College Student SmartCar Competition Software Project.
 
 
 移植注意事项
 1.删除或更改Driver/SysDriverLQ里的文件。
   •对于Infineon平台，可能只要更改即可
   •对于其他平台，可以删掉。
 1.删除或更改Driver/SysDriver/common.c 以及 Driver/SysDriver/common.h里的内容。
   •在common.h添加相应平台底层所需的所有头文件，如#include "ifx_hal.h"。
   •在commom.c里定义和相应平台底层有关的变量，如IfxEvadc_Adc_Group g_AdcGroup[ADC_GROUPS_NUM]。
   •未用到的可删除。
 2.更改Driver/SysDriver/platform.h里的芯片类型以及数据类型定义
   •更改 Chip 定义，例如，将芯片移植到MK66平台，需要添加两条宏定义：
     #define MK66 3//(该数字不能重复)
     #define Chip MK66
   •更改数据类型定义，如uint32_t为32位无符号整形，在TC377平台为unsigned long型。
 3.重定义Driver/SysDriver/xxx.h里的xxx_t结构体定义，并重写Driver/SysDriver/xxx.c里的相应函数。
   •重定义结构体
    以adc为例，需要更改adc.h里的adcx_t，将adc初始化，读取或者其他操作所需的变量全都定义在该结构体中。
   •重写函数
    以adc为例，需要更改adc.c里的函数内容，以完成相应的功能。
    如uint8_t ADCx_Init(adcx_t *adc)函数，该函数完成adc的初始化，移植时需要更改该函数完成初始化adc。
    也可以只更改需要用到的函数，不用的函数可以改成空函数。
   •注意:函数定义不可更改。
 4.更改OS/resource_config.c 里的资源配置，参考Driver/SysDriver/里的结构体定义。
   Example：
   如果配置ADC，需要将OS/resource_config.c里的ADC_Resources进行重新初始化，以ADC_ADC_Resources[0]为例，其他类似:
   adc_resource_t ADC_Resources[20] =
   {
        [0] = {
               .ADCN = {ADC27,10000},
               .Description = "AD1"
        },
   }
   其中，ADCN的初始化顺序与Driver/SysDriver/adc.h里定义的结构体adcx_t内的成员顺序相同。ADC27对应Channel，10000对应Freq。
   Description为描述信息，选填。
 5.更改interrupt.c里的中断服务函数定义
 6.根据需要移植Cpux_Main.c里的corex_main函数。
