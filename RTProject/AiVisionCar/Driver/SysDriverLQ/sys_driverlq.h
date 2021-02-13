/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】zyf/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

#define EnableInterrupts  __asm(" CPSIE i");//开总中断
#define DisableInterrupts __asm(" CPSID i");//关总中断
/////////////c标准库文件//////////////////////////////////////////////////
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include <stdarg.h>
#include "stdbool.h"

/////////////illD底层库文件//////////////////////////////////////////////////
/* fsl 底层驱动 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_adc.h"
#include "fsl_cache.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_enc.h"
#include "fsl_flexspi.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"
#include "fsl_iomuxc.h"
#include "fsl_lpi2c.h"
#include "fsl_lpspi.h"
#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_pit.h"
#include "fsl_pwm.h"
#include "fsl_qtmr.h"
#include "fsl_rtwdog.h"
#include "fsl_sccb.h"
#include "fsl_tempmon.h"
#include "fsl_trng.h"
#include "fsl_usdhc.h"
#include "fsl_video_common.h"
#include "fsl_wdog.h"
#include "fsl_xbara.h"
#include "fsl_xbarb.h"
#include "fsl_flexio.h"
#include "fsl_flexio_camera.h"
#include "fsl_flexio_camera_edma.h"
#include "fsl_flexcan.h"

///* 龙邱 底层驱动 */
#include "LQ_GPIO.h"
#include "LQ_GPIO_Cfg.h"
#include "LQ_PIT.h"
#include "LQ_UART.h"
#include "LQ_GPT.h"
#include "LQ_ADC.h"
#include "LQ_PWM.h"
#include "LQ_Encoder.h"
#include "LQ_QTMR.h"
#include "LQ_SOFTIIC.h"
#include "LQ_IIC.h"
#include "LQ_SPI.h"
#include "LQ_DMA.h"
#include "LQ_UART_DMA.h"

/* 中断优先级组 */
#define NVIC_Group0   0x07
#define NVIC_Group1   0x06
#define NVIC_Group2   0x05
#define NVIC_Group3   0x04
#define NVIC_Group4   0x03



#endif
////////////////////////////////////////////////////////////////////////////
