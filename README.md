# AI-SmartCar
 2021 National College Student SmartCar Competition Software Project.  
 
 
#### `项目介绍`
-----------------------------------------------------------------------------------------------

        本项目是第十六届全国大学生智能汽车竞赛`AI电磁越野组`:`这个电磁不懂AI`的工程项目，以TC377微控制器为主控，通过电磁传感器
    检测赛道信息并进行赛道识别，采用干簧管检测起跑线完成停车任务，用神经网络拟合参数，结合模糊控制算法，实现了对车模运动的速度控制
    和方向控制。利用无线串口、上位机、SD卡模块、以及TFT屏幕等对程序进行调试，以获得更好的控制效果。 
    
#### `项目展示`
-----------------------------------------------------------------------------------------------

|   系统框图    |
|:---------------:| 
|![image10](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/0.PNG)|

|   AI电磁越野    |   AI电磁越野    |   AI电磁越野    |   
|:---------------:|:---------------:|:---------------:|
|![image4](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/2.jpeg)|![image3](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/1.JPG)|![image5](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/3.jpeg)|
|   AI电磁越野    |    AI电磁越野         |      AI电磁越野        |   
|![image7](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/4.jpeg)|![image6](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/5.jpeg)|![image8](https://github.com/GTshenmi/AI-SmartCar/blob/main/Display%20Image/6.jpeg)|
    
#### `项目文件说明`
-----------------------------------------------------------------------------------------------

* **Data Process**：数据采集程序及数据集文件
* **Hardware**：项目的硬件设计，包括原理图、PCB等文件
* **Simulation And Analysis**：仿真及数据分析文件
* **Software**：项目的源程序代码及程序模版
* **Technical Report**：项目的技术报告。

#### `使用说明`
-----------------------------------------------------------------------------------------------

    本项目采用32-bit TriCore™ AURIX™ – TC3xx 系列MCU，采用AURIX™ Development Studio IDE进行程序编写。
    1. 安装AURIX™ Development Studio 及 Infineon DAS 驱动
    2. 下载SoftWare/Source Code/SmartCar文件夹下的源程序代码。
    3. 将代码导入AURIX™ Development Studio，并编译。
    4. 下载运行

#### `移植注意事项`
-----------------------------------------------------------------------------------------------
1.删除或更改Driver/SysDriverLQ里的文件。<br>

    •对于Infineon平台，可能只要更改即可  
    •对于其他平台，可以删掉。
    
2.删除或更改Driver/SysDriver/common.c 以及 Driver/SysDriver/common.h里的内容。 

    •在common.h添加相应平台底层所需的所有头文件，如
        
 ```c
 #include "ifx_hal.h"。
 ```
       
    •在commom.c里定义和相应平台底层有关的变量，如IfxEvadc_Adc_Group g_AdcGroup[ADC_GROUPS_NUM]。  
    •未用到的可删除。 
    
3.更改Driver/SysDriver/platform.h里的芯片类型以及数据类型定义  

    •更改 Chip 定义，例如，将芯片移植到MK66平台，需要添加两条宏定义：  
    
```c
#define MK66 3//(该数字不能重复)  
#define Chip MK66  
```
    
    •更改数据类型定义，如uint32_t为32位无符号整形，在TC377平台为unsigned long型。  
    
4.重定义Driver/SysDriver/xxx.h里的xxx_t结构体定义，并重写Driver/SysDriver/xxx.c里的相应函数。  

    •重定义结构体  
      以adc为例，需要更改adc.h里的adcx_t，将adc初始化，读取或者其他操作所需的变量全部定义在该结构体中。  
    •重写函数  
      以adc为例，需要更改adc.c里的函数内容，以完成相应的功能。  
      如uint8_t ADCx_Init(adcx_t *adc)函数，该函数完成adc的初始化，移植时需要更改该函数完成初始化adc。  
      也可以只更改需要用到的函数，不用的函数可以留空。  
    
5.更改OS/resource_config.c or OS/resource_config.h里的资源配置，参考Driver/SysDriver/里的结构体定义。  

    •Example：  
      如果配置ADC，需要将OS/resource_config.c里的ADC_Resources进行重新初始化，以ADC_ADC_Resources[0]为例，其他类似:  
   
```c
adc_resource_t ADC_Resources[20] =  
{  
     [0] = {  
           .ADCN = {ADC27,10000},  
           .Description = "AD1"  
     },       
};
```
    
      其中，ADCN的初始化顺序与Driver/SysDriver/adc.h里定义的结构体adcx_t内的成员顺序相同。ADC27对应Channel，10000对应Freq。  
      Description为描述信息，选填。
      20为需要用到的adc个数
      
6.根据需要更改OS/devices_config.c or OS/devices_config.h里的设备配置，参考OS/Device/里的设备定义。

    •Example：  
      如果配置电机，需要更改OS/devices_config.c里的Motor:
```c    
motor_ctrl_t Motor =/*ok*/
{
    .Init = Motor_Init,
    .Self = &Motor,
    //.Pwmn = {&PWM_Resources[1].PWMN,&PWM_Resources[0].PWMN},
    .Pwmn = {&PWM_Resources[1].PWMN,&PWM_Resources[0].PWMN},
    .Encn = &ENC_Resources[0].ENCN,  
};
 ```
      其中，Pwmn为驱动电机所使用的2路pwm资源，从OS/resource_config.c 里的 PWM_Resources中选取。
      Encn为电机测速需要的enc资源，从OS/resource_config.c 里的 ENC_Resources中选取。
      Init为初始化函数，对于Motor，需要填Motor_Init。
      Self为结构体指针，指向该结构体本身，即Motor。
      
      其他设备的配置类似，可参考OS/devices_config.c中对应的定义。
      
7.移植interrupt.c里的中断服务函数。

```c
•void STM1_CH0_IRQHandler(void)    /*For Data Analysis And Calculate Bias：2ms*/
```
```c
•void STM1_CH1_IRQHandler(void)    /*For Direction Control：20ms*/
```
```c
•void CCU60_CH0_IRQHandler (void)  /*For Speed Control：2ms*/
```
```c
•void CCU60_CH1_IRQHandler (void)  /*For Attitude Update：20ms*/
```

    
8.根据需要移植Cpux_Main.c里的corex_main函数。

    •core0_main
        
        os.init(0);

        os.time.delayms(100);

        Core0_HardWareInit();
        Core0_SoftWareInit();

        Core0_Main();
        
    •core1_main
 
        Core1_HardWareInit();
        Core1_SoftWareInit();

        Core1_Main();
        
    •core2_main
    
        Core2_HardWareInit();
        Core2_SoftWareInit();

        Core2_Main();        
        
    
