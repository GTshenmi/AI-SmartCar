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
中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复

中断归哪个内核管理  范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
中断服务函数放哪个核执行，CPU就选用对应的那个！
比如CCU61CH0中断服务函数默认放在放在CPU0中执行，
如果想放在CPU1中，就需要把
初始化函数CCU6_InitConfig(CCU61, CCU6_Channel0, 300000);
拷贝到cpu1_main.c中，同时把#define  CCU61_VECTABNUM 0 改为 #define  CCU61_VECTABNUM 2
如果想放在CPU2中，就需要把
初始化函数CCU6_InitConfig(CCU61, CCU6_Channel0, 300000);
拷贝到cpu2_main.c中，同时把 #define  CCU61_VECTABNUM 0 改为 #define  CCU61_VECTABNUM 3
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INTERRUPT_H_
#define _LQ_INTERRUPT_H_

//////////////////////////////////////////////////////////////////////////////////

// GPIO外部中断 组0中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT0_PRIORITY     100

// GPIO外部中断 组0中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT0_VECTABNUM    0

// GPIO外部中断 组1中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT1_PRIORITY     101

// GPIO外部中断 组1中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT1_VECTABNUM    0

// GPIO外部中断 组2中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT2_PRIORITY     102   //摄像头像素信号

// GPIO外部中断 组2中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT2_VECTABNUM    1   // DMA 管理

// GPIO外部中断 组3中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define PIN_INT3_PRIORITY     253   //摄像头场信号

// GPIO外部中断 组3中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define PIN_INT3_VECTABNUM    0

//////////////////////////////////////////////////////////////////////////////////

// CCU6定时器中断 CCU60  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CCU60_CH0_PRIORITY    80

// CCU6定时器中断 CCU60  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CCU60_CH1_PRIORITY    81

// CCU6定时器中断 CCU60   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  CCU60_VECTABNUM       2

// CCU6定时器中断 CCU61  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CCU61_CH0_PRIORITY    82

// CCU6定时器中断 CCU61  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CCU61_CH1_PRIORITY    83

// CCU6定时器中断 CCU60   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  CCU61_VECTABNUM       3

//////////////////////////////////////////////////////////////////////////////////

// STM定时器中断 STM0  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  STM0_CH0_PRIORITY    110

// STM定时器中断 STM0  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  STM0_CH1_PRIORITY    111

// STM定时器中断 STM0  中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  STM0_VECTABNUM       0

// STM定时器中断 STM1  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  STM1_CH0_PRIORITY    112

// STM定时器中断 STM1  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  STM1_CH1_PRIORITY    113

// STM定时器中断 STM1   中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  STM1_VECTABNUM       0

//////////////////////////////////////////////////////////////////////////////////

// UART0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART0_RX_PRIORITY    120

// UART0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART0_TX_PRIORITY    121

// UART0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART0_ER_PRIORITY    122

// UART0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  UART0_VECTABNUM      0

// UART1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART1_RX_PRIORITY    130

// UART1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART1_TX_PRIORITY    131

// UART1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART1_ER_PRIORITY    132

// UART1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  UART1_VECTABNUM      0

// UART2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART2_RX_PRIORITY    133

// UART2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART2_TX_PRIORITY    134

// UART2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART2_ER_PRIORITY    135

// UART2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  UART2_VECTABNUM      0

// UART3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART3_RX_PRIORITY    136

// UART3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART3_TX_PRIORITY    137

// UART3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  UART3_ER_PRIORITY    138

// UART3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  UART3_VECTABNUM      0

//////////////////////////////////////////////////////////////////////////////////

// QSPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI0_RX_PRIORITY    150

// QSPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI0_TX_PRIORITY    151

// QSPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI0_ER_PRIORITY    152

// QSPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  QSPI0_VECTABNUM      0

// QSPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI1_RX_PRIORITY    153

// QSPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI1_TX_PRIORITY    154

// QSPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI1_ER_PRIORITY    155

// QSPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  QSPI1_VECTABNUM      0

// QSPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI2_RX_PRIORITY    156

// QSPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI2_TX_PRIORITY    157

// QSPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI2_ER_PRIORITY    158

// QSPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  QSPI2_VECTABNUM      0

// QSPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI3_RX_PRIORITY    159

// QSPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI3_TX_PRIORITY    160

// QSPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  QSPI3_ER_PRIORITY    161

// QSPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  QSPI3_VECTABNUM      0
//////////////////////////////////////////////////////////////////////////////////

// SPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI0_RX_PRIORITY    180

// SPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI0_TX_PRIORITY    181

// SPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI0_ER_PRIORITY    182

// SPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  SPI0_VECTABNUM      0

// SPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI1_RX_PRIORITY    183

// SPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI1_TX_PRIORITY    184

// SPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI1_ER_PRIORITY    185

// SPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  SPI1_VECTABNUM      0

// SPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI2_RX_PRIORITY    186

// SPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI2_TX_PRIORITY    187

// SPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI2_ER_PRIORITY    188

// SPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  SPI2_VECTABNUM      0

// SPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI3_RX_PRIORITY    190

// SPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI3_TX_PRIORITY    191

// SPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  SPI3_ER_PRIORITY    192

//////////////////////////////////////////////////////////////////////////////////

// SPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  SPI3_VECTABNUM      0

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU0_SOFT_IRQ0_PRIORITY    220

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU0_SOFT_IRQ1_PRIORITY    221

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU0_SOFT_IRQ2_PRIORITY    222

// CPU0软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU0_SOFT_IRQ3_PRIORITY    223

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU1_SOFT_IRQ0_PRIORITY    224

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU1_SOFT_IRQ1_PRIORITY    225

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU1_SOFT_IRQ2_PRIORITY    226

// CPU1软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU1_SOFT_IRQ3_PRIORITY    227

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU2_SOFT_IRQ0_PRIORITY    228

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU2_SOFT_IRQ1_PRIORITY    229

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU2_SOFT_IRQ2_PRIORITY    230

// CPU2软件中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  CPU2_SOFT_IRQ3_PRIORITY    231

//////////////////////////////////////////////////////////////////////////////////

// DMA中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复
#define  DMA_PRIORITY    255

// DMA中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0 1：DMA 2：CPU1  3：CPU2...
#define  DMA_VECTABNUM    0

#endif

