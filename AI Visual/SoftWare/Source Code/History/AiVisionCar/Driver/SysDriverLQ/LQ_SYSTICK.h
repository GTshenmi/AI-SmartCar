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

/* ���� systime �ṹ��*/
extern systime_t  systime;

typedef struct time{
	
        uint32_t fac_us;                  //us��Ƶϵ��
	uint32_t fac_ms;                  //ms��Ƶϵ��
	volatile uint32_t millisecond;    //��ǰmsֵ
	uint8_t ms_per_tick;              //ÿ������ms�ж�һ��
	
}lq_time_t;

extern lq_time_t lqtimer;

/**
  * @brief    ����systime ��ʱ��
  *
  * @param    
  *
  * @return   
  *
  * @note     ʾ���� A7   500Hz��ת
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void Test_Systick(void);

void systime_init(void);


#endif //__LQ_SYSTICK_H




