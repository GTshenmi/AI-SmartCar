/*------------------------------------------------------
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
--------------------------------------------------------*/

#include "include.h"

/**
  * @brief    ����ȷ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 ����һ
*/
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 30000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}	
}
 

/*!
 * @brief Main function 
 */
int main(void)
{  
    Core0_Init();
    Core0_Main();
    
    return 0;
}
