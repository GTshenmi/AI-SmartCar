/*-------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��ע   �⡿ �жϷ����� ��֪�����ֿ���ȥstartup_MIMXRT1064.s������
---------------------------------------------------------*/
#include "include.h"
#include "fsl_sdmmc_host.h"

/* SD�� ����Ƿ�忨�ܽ� */
extern void SDMMCHOST_CARD_DETECT_GPIO_INTERRUPT_HANDLER(void);
/**
  * @brief    GPIO2  pin16-31���жϷ�����
  *
  * @param    
  *
  * @return   
  *
  * @note     ���SD���Ƿ����
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void GPIO2_Combined_16_31_IRQHandler(void)
{ 
    
    /* SD�����ܽ� */
    SDMMCHOST_CARD_DETECT_GPIO_INTERRUPT_HANDLER();

    __DSB();				                            //����ͬ������ָ�� 
}









