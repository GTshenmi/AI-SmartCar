/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC2/3xx���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��1��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC2/3xx
��Crystal�� 20.000Mhz
��SYS PLL�� 200/300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
=================================================================
����ͷ�ӿ�                  �������ۻ���OV7725ģ��
�� ���ݶ˿ڣ�P02.0-P02.7�ڣ���8λ��������ͷ�����ݶ˿ڣ�
�� ʱ�����أ��ⲿ�жϵ�0�飺P00_4��
�� ���źţ��ⲿ�жϵ�3�飺P15_1��
-----------------------------------------------------------------
�Ƽ�GPT12ģ�飬������ʵ��5·�����������������������ݴ�������������źŲɼ�������ѡ����·���ɣ�
P33_7, P33_6   ����TCĸ�������1
P02_8, P33_5   ����TCĸ�������2
P10_3, P10_1   ����TCĸ�������3
P20_3, P20_0   ����TCĸ�������4
-----------------------------------------------------------------
��е�ѹ�ɼ�ģ�������˷�ģ��
�Ƽ�ʹ��AN0-7������·ADC����������chirp�����źż���ų���е�ѹ�ɼ���
AN0-3          ����TC���ĸ���˷�ģ����ߵ��
-----------------------------------------------------------------
Ĭ�ϵ���ӿ�
ʹ��GTMģ�飬ATOM�ĸ�ͨ���ɲ���4*8��32·PWM�����Ҹ���Ƶ�ʺ�ռ�ձȿɵ����Ƽ�ʹ��ATOM0��0-7ͨ����
��һ��˫·ȫ������
P23_1         ����TCĸ��MOTOR1_P
P32_4         ����TCĸ��MOTOR1_N
P21_2         ����TCĸ��MOTOR2_P
P22_3         ����TCĸ��MOTOR2_N
�ڶ���˫·ȫ������
P21_4         ����TCĸ��MOTOR3_P
P21_3         ����TCĸ��MOTOR3_N
P20_8         ����TCĸ��MOTOR4_P
P21_5         ����TCĸ��MOTOR4_N
-----------------------------------------------------------------
Ĭ�϶���ӿ�
ʹ��GTMģ�飬ATOM�ĸ�ͨ���ɲ���4*8��32·PWM�����Ҹ���Ƶ�ʺ�ռ�ձȿɵ����Ƽ�ʹ��ATOM2��
P33_10        ����TCĸ����1
P33_13        ����TCĸ����2
-----------------------------------------------------------------
 Ĭ����Ļ��ʾ�ӿڣ��û�����ʹ��TFT����OLEDģ��
TFTSPI_CS     P20_14     ����TCĸ�� CS�ܽ� Ĭ�����ͣ����Բ���
TFTSPI_SCK    P20_11     ����TCĸ�� SPI SCK�ܽ�
TFTSPI_SDI    P20_10     ����TCĸ�� SPI MOSI�ܽ�
TFTSPI_DC     P20_12     ����TCĸ�� D/C�ܽ�
TFTSPI_RST    P20_13     ����TCĸ�� RESET�ܽ�
-----------------------------------------------------------------
OLED_CK       P20_14     ����TCĸ��OLED CK�ܽ�
OLED_DI       P20_11     ����TCĸ��OLED DI�ܽ�
OLED_RST      P20_10     ����TCĸ��OLED RST�ܽ�
OLED_DC       P20_12     ����TCĸ��OLED DC�ܽ�
OLED_CS       P20_13     ����TCĸ��OLED CS�ܽ� Ĭ�����ͣ����Բ���
----------------------------------------------------------------
Ĭ�ϰ����ӿ�
KEY0p         P22_0      ����TCĸ���ϰ���0
KEY1p         P22_1      ����TCĸ���ϰ���1
KEY2p         P22_2      ����TCĸ���ϰ���2
DSW0p         P33_9      ����TCĸ���ϲ��뿪��0
DSW1p         P33_11     ����TCĸ���ϲ��뿪��1
-----------------------------------------------------------------
Ĭ��LED�ӿ�
LED0p         P10_6      ����TCĸ����İ���LED0 ����
LED1p         P10_5      ����TCĸ����İ���LED1 ����
LED2p         P20_6      ����TCĸ����LED0
LED3p         P20_7      ����TCĸ����LED1
-----------------------------------------------------------------
Ĭ�Ϸ������ӿ�
BEEPp         P33_8      ����TCĸ���Ϸ������ӿ�
-----------------------------------------------------------------
��ʱ��
������CCU6ģ��  ÿ��ģ��������������ʱ��  ������ʱ���ж�
�Ƽ�ʹ��CCU6ģ�飬STM����ϵͳʱ�ӻ�����ʱ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <include.h>//����ģ���ͷ�ļ�
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_MotorServo.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <Platform_Types.h>
#include <stdio.h>

App_Cpu0 g_AppCpu0; // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 ��ʼ����ɱ�־λ
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0ռ��/1�ͷ� TFT

/*************************************************************************
*  �������ƣ�int core0_main (void)
*  ����˵����CPU0������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
int core0_main (void)
{
    //char txt[16];

	// �ر�CPU���ж�
	IfxCpu_disableInterrupts();

	// �رտ��Ź�����������ÿ��Ź�ι����Ҫ�ر�
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// ��ȡ����Ƶ��
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

	TFTSPI_Init(0);               // TFT1.8��ʼ��0:����  1������
	TFTSPI_CLS(u16BLACK);         // ����
	TFTSPI_Show_Logo(0,37);       // ��ʾ����LOGO
	TFTSPI_P16x16Str(0,0,(unsigned char*)"�����������ܿƼ�",u16RED,u16BLUE);// �ַ�����ʾ
	delayms(1000);
	// ������ʼ��
	GPIO_KEY_Init();
	// LED������P10.6��P10.5��ʼ��
	GPIO_LED_Init();

	// ����P14.0�ܽ����,P14.1���룬������115200
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);

	// ����CPU���ж�
	IfxCpu_enableInterrupts();

	// ֪ͨCPU2��CPU1��CPU0��ʼ�����
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);

	// �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
	mutexCpu0TFTIsOk=0;      // CPU1�� 0ռ��/1�ͷ� TFT

    // ����������Ƶ��ַ��https://space.bilibili.com/95313236
    // ���²��Ժ������ڽ���ѭ�����û��ɵ�������ģ��ĳ�ʼ������д������ʵ���Լ���������ؼ���ж�����CPU
	//________________________________________________________________________________
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    Test_ADC_OLED();      //PASS,����ADC����ʱ��  OLED����ʾ ADC����10K��ʱ��
//    Test_ADC_TFT();        //PASS,����ADC����ʱ�� TFT����ʾ ADC����10K��ʱ��
//    Test_ADC_7mic();       //PASS,����ADC\UART0��STM��ʱ�����ƣ�ͨ��UART0��ӡ AN0--AN7��8��ͨ��ADCת������
//    Test_GPIO_Extern_Int();//PASS,�����ⲿ��1���ж�P15.8��P10.6��P10.5����
//    TestMotor();	         //PASS,���Ե��������ͨ�������޸����PWM����TFT1.8��Ļ��ʾ��ͨ���궨��ѡ���Ӧ������ģ��
//    TestServo();	         //PASS,���Զ��������ͨ�������޸����PWM����TFT1.8��Ļ��ʾ
//    TestEncoder();         //PASS,���Ա���������Ļ��ʾ��������ֵ����Ӧĸ���ϱ�����2�ͱ�����4ͨ��
//    Test_GPIO_LED();       //PASS,����GPIO��P10.6��P10.5����
//    Test_GPIO_KEY();       //PASS,�����ⲿ�������룬P22.0--2   ���°��� LED��
//    Test_ComKEY_Tft();     //PASS,�����ⲿ��ϰ������벢TFT1.8��ʾ��P22.0--2
//    Test_OLED();           //PASS,����OLED0.96��ʹ��P20.14--10����ʾ�ַ�������̬����
//    LQ_STM_Timer();        //PASS,���Զ�ʱ�жϡ�����
//    LQ_CCU6_Timer();       //PASS,���Զ�ʱ�жϡ����ƣ�CC60Ĭ�Ϸ�CPU0���ԣ�CCU61Ĭ�Ϸ�CPU1����
//    Test_TFT18();          //PASS,����TFT1.8����ʹ��P20.14--10����ʾ�ַ�������̬����
//    LQ_TIM_InputCature();  //PASS,����GTM_TOM_TIM��P20_9��ΪPWM����ڣ�P15_0��ΪTIM����ڣ����߶̽Ӻ󣬴���P14.0���͵���λ��
//    Test_Bluetooth();      //PASS,����UART0(P14.0RX/P14.1TX)��
//    Test_EEPROM();         //PASS,�����ڲ�EEPROM��д����  OLED��ʾ�Ƿ�д��ɹ�
//    Test_Vl53();           //PASS,����VL53  IIC����   P13_1��SCL  P13_2��SDA OLED��ʾԭʼ����
//    Test_MPU6050();        //PASS,����MPU6050����ICM20602 IIC����   P13_1��SCL  P13_2��SDA OLED��ʾԭʼ����
//    Test_ICM20602();       //PASS,����ICM20602 SPI����   P15_8��SCL  P15_5��SDA  P15_7��SA  P15_2��CS OLED��ʾԭʼ����
//    Test_CAMERA();         //PASS,Ĭ�Ϸ�CPU0�в���������������ͷ������Ļ����ʾ  LQ_CAMERA.h ��ѡ����Ļ
//    Test_SoftFft();        //PASS,����ILLD������FFT������Ӳ��FFT��ҪоƬ��׺��DA�Ĳ���Ӳ��FFT����
//    Test_RDA5807();        //PASS,����RDA5807������������SCL �� P00_1   SDA �� P00_2

	//________________________________________________________________________________
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    while (1)	//��ѭ��
    {
        LED_Ctrl(LED0, RVS);     // LED��˸ ָʾ��������״̬
        delayms(20);
    }
}
