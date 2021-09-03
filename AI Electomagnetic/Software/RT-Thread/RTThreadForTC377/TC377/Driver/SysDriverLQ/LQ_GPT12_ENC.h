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
#ifndef _LQ_GPT12_H_
#define _LQ_GPT12_H_

#include "stdint.h"
#include <IfxGpt12.h>
#include <IfxGpt12_reg.h>
#include <Platform_Types.h>
/**
* @brief ENC ����ܽ�
*/
typedef enum
{
  ENC2_InPut_P00_7 = 0x0200, ENC2_InPut_P33_7 = 0x0201,//00.7����ͷ����ռ��
  ENC3_InPut_P02_6 = 0x0300, ENC3_InPut_P10_4 = 0x0301,//02.6����ͷ���ݿ�ռ��
  ENC4_InPut_P02_8 = 0x0400, ENC4_InPut_P10_8 = 0x0401,//
  ENC5_InPut_P10_3 = 0x0500, ENC5_InPut_P21_7 = 0x0501,//
  ENC6_InPut_P20_3 = 0x0600, ENC6_InPut_P10_2 = 0x0601 //
}ENC_InputPin_t;

/**
* @brief ENC ����ܽ�
*/
typedef enum
{
  ENC2_Dir_P00_8 = 0x0200, ENC2_Dir_P33_6 = 0x0201,   //00.8����ͷռ��
  ENC3_Dir_P02_7 = 0x0300, ENC3_Dir_P10_7 = 0x0301,   //02.7����ͷ���ݿ�ռ��
  ENC4_Dir_P00_9 = 0x0400, ENC4_Dir_P33_5 = 0x0401,   //00.9����ͷռ��
  ENC5_Dir_P10_1 = 0x0500, ENC5_Dir_P21_6 = 0x0501,
  ENC6_Dir_P20_0 = 0x0600, ENC6_Dir_P10_0 = 0x0601,
}ENC_DirPin_t;


#define ENC_GetTimer(pin)    (unsigned char)(pin >> 8)
void ENC_InitConfig(ENC_InputPin_t InputPin, ENC_DirPin_t DirPin);
signed short ENC_GetCounter(ENC_InputPin_t InputPin);


#endif /* 0_APPSW_TRICORE_APP_LQ_GPT12_H_ */
