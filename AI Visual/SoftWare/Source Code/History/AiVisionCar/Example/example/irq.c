/*-------------------------------------------------------
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
【注   意】 中断服务函数 不知道名字可以去startup_MIMXRT1064.s里面找
---------------------------------------------------------*/
#include "include.h"
#include "fsl_sdmmc_host.h"

/* SD卡 检测是否插卡管脚 */
extern void SDMMCHOST_CARD_DETECT_GPIO_INTERRUPT_HANDLER(void);
/**
  * @brief    GPIO2  pin16-31的中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     检测SD卡是否插入
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void GPIO2_Combined_16_31_IRQHandler(void)
{ 
    
    /* SD卡检测管脚 */
    SDMMCHOST_CARD_DETECT_GPIO_INTERRUPT_HANDLER();

    __DSB();				                            //数据同步屏蔽指令 
}









