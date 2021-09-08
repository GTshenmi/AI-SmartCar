/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
=================================================================
ʹ��˵����
����ѧ��ʾ���������ڵ�����ֻ������ֳ���
������ԴΪ��
ģ�飺����TC264DA���İ塢����ĸ�塢˫·ȫ�ŵ��������˫��������TFT1.8��Ļ�����������·���ģ�飻
��ģ�����ֻ������־��ɣ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
//#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_MotorServo.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_GPT12_ENC.h>

sint16 ECPULSE1 = 0;          //�ٶ�ȫ�ֱ���
sint16 ECPULSE2 = 0;          //�ٶ�ȫ�ֱ���

//��ʼ������
void InductorInit (void)
{
    ADC_InitConfig(ADC0, 1000000); //��ʼ��
    ADC_InitConfig(ADC1, 1000000); //��ʼ��
    ADC_InitConfig(ADC2, 1000000); //��ʼ��
    ADC_InitConfig(ADC3, 1000000); //��ʼ��
}

/*************************************************************************
 *  �������ƣ�void CCU61_CH0_IRQHandler (void)
 *  ����˵������ʱ��ȡ��������ֵ
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��10��10��
 *  ��    ע��
 *************************************************************************/
void CCU61_CH0_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* �û����� */
    /* ��ȡ������ֵ */
    ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
}

/*************************************************************************
 *  �������ƣ�void ElectroMagneticCar(void)
 *  ����˵������ų�˫������ٿ��ƣ��򵥵ķֶα��������㷨
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��10��28��
 *  ��    ע������2�����
 *************************************************************************/
void ElectroMagneticCar (void)
{
    while (1)
    {
        //�û�����
    }
}

