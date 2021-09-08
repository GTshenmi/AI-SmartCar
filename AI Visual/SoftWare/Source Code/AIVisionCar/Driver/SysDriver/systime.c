/*
 * systime.c
 *
 *  Created on: 2020年12月26日
 *      Author: 936305695
 */
#include "systime.h"

/**
  * @brief    systime 中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void SysTick_Handler(void)
{
    lqtimer.millisecond += lqtimer.ms_per_tick;
    
    if(Systime.InterruptCallBack != NULL)
    {
         Systime.InterruptCallBack();
    }
    
#ifdef USE_SD
    /* SD卡用到的 使用SD卡需要使用 不用SD卡删除即可 */
    extern volatile uint32_t g_eventTimeMilliseconds;
    g_eventTimeMilliseconds += EACH_PER_MS;
#endif
}

uint8_t SysTime_Init()
{
    systime_init();
    Systime.InterruptCallBack = NULL;
    return 0;
}
uint32_t SysTime_Get_Timeus()
{
   return systime.get_time_us();
}

uint32_t SysTime_Get_Timems()
{
  return systime.get_time_ms();
}
uint8_t SysTime_Delayus(uint32_t us)
{
    systime.delay_us(us);
    return 0;
}
uint8_t SysTime_Delayms(uint32_t ms)
{
    systime.delay_ms(ms);
    return 0;
}

systimex_t Systime =
{
        .Init = SysTime_Init,
        .Get_Timeus = SysTime_Get_Timeus,
        .Get_Timems = SysTime_Get_Timems,
        .Delayms = SysTime_Delayms,
        .Delayus = SysTime_Delayus,
};

