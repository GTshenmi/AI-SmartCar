/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC3xxxA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC3xxxA
��Crystal�� 20.000Mhz
��SYS PLL�� 300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c��׼���ļ�//////////////////////////////////////////////////
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include <stdarg.h>
#include "stdbool.h"
#include "stdlib.h"

/////////////illD�ײ���ļ�//////////////////////////////////////////////////
#include <Asclin\Asc\IfxAsclin_Asc.h>
#include "Asclin\Spi\IfxAsclin_Spi.h"
#include "Ccu6\TPwm\IfxCcu6_TPwm.h"
#include "Ccu6\Timer\IfxCcu6_Timer.h"
#include "Ccu6\Icu\IfxCcu6_Icu.h"
#include "Ccu6\PwmBc\IfxCcu6_PwmBc.h"
#include "Compilers.h"
#include "ConfigurationIsr.h"
#include "Configuration.h"
#include "Cpu\Std\Ifx_Types.h"
#include "Cpu\Irq\IfxCpu_Irq.h"
#include <Cpu\Std\IfxCpu.h>
#include "Cpu\Std\Platform_Types.h"
#include "Cpu\Std\IfxCpu_Intrinsics.h"
#include <Dma\Dma\IfxDma_Dma.h>
#include "Flash\Std\IfxFlash.h"

#include "Gtm\Std\IfxGtm.h"
#include "Gtm\Std\IfxGtm_Tom.h"
#include "Gtm\Std\IfxGtm_Tim.h"
#include "Gtm\Atom\Pwm\IfxGtm_Atom_Pwm.h"
#include "Gtm\Tim\In\IfxGtm_Tim_In.h"
#include "Gtm\Tom\Pwm\IfxGtm_Tom_Pwm.h"
#include "Gtm\Tom\Timer\IfxGtm_Tom_Timer.h"
#include "Gtm\Tom\PwmHl\IfxGtm_Tom_PwmHl.h"
#include "Gtm\Trig\IfxGtm_Trig.h"
#include <Gpt12\IncrEnc\IfxGpt12_IncrEnc.h>
#include "Gpt12\Std\IfxGpt12.h"
//#include "Ifx_Assert.h"
#include <I2c\I2c\IfxI2c_I2c.h>
#include "IfxCpu_reg.h"
#include <Ifx_Cfg.h>
#include "interrupt.h"

#include "LQ_EEPROM.h"
//#include "LQ_GPSR.h"

#include "IfxScu_reg.h"
#include "IfxPort.h"
#include "IfxPort_reg.h"
#include "Port\Std\IfxPort.h"
#include "Port\Io\IfxPort_Io.h"
#include "Qspi\SpiMaster\IfxQspi_SpiMaster.h"
#include "Qspi\SpiSlave\IfxQspi_SpiSlave.h"
#include "SysSe\Bsp\Bsp.h"
#include "Stm\Std\IfxStm.h"
#include "Scu\Std\IfxScuWdt.h"
#include "Src\Std\IfxSrc.h"
#include "Scu\Std\IfxScuEru.h"
#include "Tricore\Compilers\Compilers.h"
#include "Evadc\Adc\IfxEvadc_Adc.h"
#include "_PinMap\IfxGtm_PinMap.h"
#include "_Reg\IfxEvadc_reg.h"
#include "_Reg\IfxStm_reg.h"
#include "_Utilities\Ifx_Assert.h"
#include "I2c/I2c/IfxI2c_I2c.h"

#include <Cpu/Std/Platform_Types.h>
#include "stdint.h"
#include <IfxFlash.h>
#include <IfxFlash_cfg.h>
#include <IfxScuWdt.h>

#include "Bsp.h"

////////////////////////////////////////////////////////////////////////////
typedef struct
{
    float32 sysFreq;  //Actual SPB frequency
    float32 cpuFreq;  //Actual CPU frequency
    float32 pllFreq;  //Actual PLL frequency
    float32 stmFreq;  //Actual STM frequency
} AppInfo;

// Application information
typedef struct
{
    AppInfo info;     //Info object
} App_Cpu0;

#endif
////////////////////////////////////////////////////////////////////////////
