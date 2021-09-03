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
TC eepromc 文件
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ__EEPROM_H
#define __LQ__EEPROM_H

#include <Cpu/Std/Platform_Types.h>
#include "stdint.h"
#include <IfxFlash.h>
#include <IfxFlash_cfg.h>
#include <IfxScuWdt.h>

void EEPROM_EraseSector(unsigned char sector);
void EEPROM_Write(unsigned char sector, unsigned short page, unsigned long * buff, unsigned short len);
void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len);


#endif
