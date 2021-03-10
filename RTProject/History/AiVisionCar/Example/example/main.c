/*------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
--------------------------------------------------------*/

#include "include.h"

/**
  * @brief    不精确延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 星期一
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
