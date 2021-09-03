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
ASC异步串行通信，可以作为UART和汽车LIN总线使用，
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_ASC_H_
#define _LQ_ASC_H_

#include <Platform_Types.h>
#include <stdint.h>
#include <Ifx_Fifo.h>
#include <Ifx_Types.h>
#include <IfxAsclin.h>
#include <IfxAsclin_Asc.h>
#include <IfxAsclin_PinMap.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxPort.h>
#include <IfxPort_regdef.h>
#include "interrupt.h"


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


void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate);
void UART_PutChar(UART_t  uratn, char ch);
void UART_PutStr(UART_t  uratn, char *str);
void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len);
uint8 UART_GetCount(UART_t  uratn);
char UART_GetChar(UART_t  uratn);
char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len);

extern IfxAsclin_Asc g_UartConfig[4];

#endif /* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
