#include "rtthread.h"
#include "include.h"

#pragma section all "cpu0_dsram"

//uint16 num;
IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;//事件同步变量

int core0_main(void)
{
extern int rtthread_startup(void);

    // 关闭CPU总中断
    //IfxCpu_disableInterrupts();
    disableInterrupts();
    // 关闭看门狗，如果不设置看门狗喂狗需要关闭
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    // 读取总线频率
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

    rtthread_startup();

    rt_kprintf("Failed to start rt-thread.\n");

    while(1);

}

void GPIO_Demo_init(void)
{
    /* configure P20.8 as general output */
    IfxPort_setPinMode(&MODULE_P10, 6,  IfxPort_Mode_outputPushPullGeneral);
    /* configure P20.9 as general output */
    IfxPort_setPinMode(&MODULE_P10, 5,  IfxPort_Mode_outputPushPullGeneral);
//    /* configure P21.4 as general output */
//    IfxPort_setPinMode(&MODULE_P21, 4,  IfxPort_Mode_outputPushPullGeneral);
//    /* configure P21.5 as general output */
//    IfxPort_setPinMode(&MODULE_P21, 5,  IfxPort_Mode_outputPushPullGeneral);
}

static rt_uint8_t led_thread_stack[2048];
struct rt_thread led_thread_thread;
void led_thread_entry(void *parameter)
{
    GPIO_Demo_init();
    //GLED.Init(GLED.Self);
    while(1)
    {
        IfxPort_togglePin(&MODULE_P10, 6);
        IfxPort_togglePin(&MODULE_P10, 5);
        //GLED.Toggle(GLED.Self);
        rt_thread_mdelay(1000);
//        IfxPort_togglePin(&MODULE_P21, 4);
//        IfxPort_togglePin(&MODULE_P21, 5);
    }
}


int main(void)
{
    rt_thread_t tid;
    rt_err_t result;

    rt_uint32_t count;

    tid = &led_thread_thread;
    result = rt_thread_init(tid, "led", led_thread_entry, RT_NULL,
                led_thread_stack, sizeof(led_thread_stack), 10, 20);
    RT_ASSERT(result == RT_EOK);
    rt_thread_startup(tid);
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("hello rt-thread! %d\n",count++);
    }
}


#pragma section all restore
