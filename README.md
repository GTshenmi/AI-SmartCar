# AI-SmartCar
 2021 National College Student SmartCar Competition Software Project.
 
 
 移植注意事项
 1.
 1.更改OS/resource_config.c 里的资源配置，参考Driver/SysDriver/里的结构体定义。
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
