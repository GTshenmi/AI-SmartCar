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
  */ 
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include <stdint.h>

extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 ��ʼ����ɱ�־λ  */

extern IfxCpu_mutexLock mutexTFTIsOk;        /** TFT18ʹ�ñ�־λ  */
/**
 * ��ʱ�� 5ms��50ms��־λ
 */
extern volatile unsigned char cpu1Flage5ms;
extern volatile unsigned char cpu1Flage50ms;

/* �����ٶ�  */
extern volatile signed short targetSpeed;
/* ���ϱ�־λ */
extern volatile unsigned char evadibleFlage;

int core0_main (void);

#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
