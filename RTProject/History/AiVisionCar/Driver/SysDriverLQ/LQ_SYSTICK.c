/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年03月13日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_common.h"
#include "LQ_SYSTICK.h"
//#include "include.h"

#define EACH_PER_MS    25   //每隔 25 ms 中断一次  systick 定时器是24位向下计数的定时器  最大装载值16777215 / 600 000 000= 0.2796 最大计时27ms

lq_time_t lqtimer;

/**
  * @brief    systime 初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     默认优先级最低 可以修改
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void systime_init(void)
{
	lqtimer.fac_us = SystemCoreClock / 1000000;
	lqtimer.fac_ms = SystemCoreClock / 1000;
	lqtimer.ms_per_tick = EACH_PER_MS;
        lqtimer.millisecond = 100;
	SysTick_Config((SystemCoreClock / 1000) * lqtimer.ms_per_tick );   //开启systick中断
    
//    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
//    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
}


/**
  * @brief    获取当前时间
  *
  * @param    
  *
  * @return   当前ms值
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
static inline uint32_t systime_get_current_time_ms(void)
{
    register uint32_t val, ms;
    do
    {
        ms  = lqtimer.millisecond;
        val = SysTick->VAL; 
    }while(ms != lqtimer.millisecond);
    
	return ms  -  val/lqtimer.fac_ms;
}



/**
  * @brief    获取当前时间
  *
  * @param    
  *
  * @return   当前us值
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
static inline uint32_t systime_get_current_time_us(void)
{
    register uint32_t val, ms;
    do
    {
        ms  = lqtimer.millisecond;
        val = SysTick->VAL;
    }while(ms != lqtimer.millisecond);
	return (uint32_t)((uint32_t)(ms * 1000) -  val / lqtimer.fac_us);
}




/**
  * @brief    systime 延时函数
  *
  * @param    
  *
  * @return   
  *
  * @note     最大延时不要超过 4292s
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void systime_delay_us(uint32_t us)     
{
    uint32_t now = systime.get_time_us();
	uint32_t end_time = now + us - 1;
	while( systime.get_time_us() <= end_time)
    {
        ;
    }
}



/**
  * @brief    延时函数
  *
  * @param    ms :  延时时间
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void systime_delay_ms(uint32_t ms) 
{
    while(ms--)
    {
        systime.delay_us(1000);
    }
	
}


systime_t  systime = 
{
	systime_init,
	systime_get_current_time_us,
	systime_get_current_time_ms,
	systime_delay_us,
	systime_delay_ms
};



