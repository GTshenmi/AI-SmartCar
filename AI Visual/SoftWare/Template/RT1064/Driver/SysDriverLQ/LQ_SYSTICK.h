#ifndef __LQ_SYSTICK_H
#define __LQ_SYSTICK_H
#include "fsl_common.h"

typedef struct
{	
	void (* init) (void);  
	uint32_t (* get_time_us) (void);
	uint32_t (* get_time_ms) (void);
	void (* delay_us)(uint32_t);
	void (* delay_ms)(uint32_t);
}systime_t;

/* 声明 systime 结构体*/
extern systime_t  systime;

typedef struct time{
	
        uint32_t fac_us;                  //us分频系数
	uint32_t fac_ms;                  //ms分频系数
	volatile uint32_t millisecond;    //当前ms值
	uint8_t ms_per_tick;              //每隔多少ms中断一次
	
}lq_time_t;

extern lq_time_t lqtimer;

/**
  * @brief    测试systime 计时器
  *
  * @param    
  *
  * @return   
  *
  * @note     示波器 A7   500Hz翻转
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void Test_Systick(void);

void systime_init(void);


#endif //__LQ_SYSTICK_H




