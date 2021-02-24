/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include "stdint.h"
#include "LQ_GPIO_Cfg.h"

/*!< GPIO ���� */
typedef enum
{
    /*ת���ٶ��ֶ� 
    ����һ�����Ե����ߵ͵�ƽ�л��������½�ʱ���ٶȵ������
    ��һ���Ӧ����ʹ��оƬĬ�ϵ����þͿ����ˡ�
    ����Ҫ΢��SI��EMI����ʱ���Գ����޸����� */
    SlewRateSlow         = 0x0,     /*!< Ĭ�� */
    SlewRateFast         = 0x1,
    
    /* ���������ֶΣ���Ϊ���ʱ��Ч */
    DriveStrengthDisable = 0x0,     /*!< ��ֹ���ǿ������ */
    DriveStrengthR0      = 0x8,     /*!< R0��260ŷķ@3.3v��150ŷķ@1.8v  */
    DriveStrengthR0_2    = 0x10,    /*!< R0/2 */
    DriveStrengthR0_3    = 0x18,    /*!< R0/3 */
    DriveStrengthR0_4    = 0x20,    /*!< R0/4 */
    DriveStrengthR0_5    = 0x28,    /*!< R0/5 */
    DriveStrengthR0_6    = 0x30,    /*!< Ĭ��R0/6 */
    DriveStrengthR0_7    = 0x38,    /*!< R0/7 */
    
    /* �����ٶ� �������ָIO��ת�ٶȣ�����ͨ��IO�ڲ��β�ʧ��Ĵ��� */
    Speed50Mhz           = 0x0,     /*!< 50MHz */
    Speed100Mhz          = 0x80,    /*!< Ĭ��100MHz */
    Speed200Mhz          = 0xC0,    /*!< 200MHz */
    
    /* ©����· ��Ϊ���ʱ��Ч�������ⲿ��ѹ������������ */
    OpenDrainDisable     = 0x0,     /*!< Ĭ�Ϲر� */
    OpenDrainEnable      = 0x800,   /*!< ���� */
    
    /* ��/����ʹ���ֶ� */
    PullKeeperDisable    = 0x0,     /*!< �ر� */
    PullKeeperEnable     = 0x1000,  /*!< Ĭ�Ͽ��� */
    
    /* ѡ�������߱��� 
    ���֣�ʹ������״̬������������IO����NVCC_xxx�ص�֮��ʹ���뻺����������Զ�ά���ڹص�֮ǰ���߼�״̬�����ֵ�����ڵ͹��ĵ�Ӧ���еõ����֡� 
    ����  ʹ�����״̬�������������ں˹���ص�֮��ʹIO������Զ�ά���ڹص�֮ǰ���߼�״̬(��Ҫע��������״̬������������������ͬʱ����)�����ֵͬ�����������ڵ͹��ĵ�Ӧ���� */
    KeeperSelect         = 0x0,     /*!< Ĭ��ѡ��Keeper */
    PullSelect           = 0x2000,  /*!< ѡ��Pull */
        
    /* ������ ����ʱ��Ч */
    Down100K             = 0x0,     /*!< Ĭ��100K�������� */
    UP47K                = 0x4000,  /*!< 47K�������� */
    UP100K               = 0x8000,  /*!< 100K�������� */
    UP22K                = 0xC000,  /*!< 22K�������� */
        
    /* ����ʹ���ֶ� ��Ϊ����ʱ��Ч */
    HysteresisDisable    = 0x0,     /*!< Ĭ�Ϲر� */
    HysteresisEnable     = 0x10000, /*!< ���� */
    
    /* ���� ���(Ϊ����ʹ���Լ���ӵģ��Ĵ�����û�����) */
    OUTPUT               = 0x8000000,    /*!< ��� */
    INPUT                = 0,            /*!< ���� */
    
}PinConfig_t;


/* GPIO ����ģʽ */
#define PIN_MODE_OUTPUT         OUTPUT|DriveStrengthR0_6                                           //�������
#define PIN_MODE_INPUT          INPUT|DriveStrengthR0_6|OpenDrainEnable                            //��������
#define PIN_MODE_INPUT_PULLUP   INPUT|DriveStrengthR0_6|PullKeeperEnable|PullSelect|UP100K         //��������
#define PIN_MODE_INPUT_PULLDOWN INPUT|DriveStrengthR0_6|PullKeeperEnable|PullSelect|Down100K       //��������
#define PIN_MODE_OUTPUT_OD      OUTPUT|DriveStrengthR0_6|OpenDrainEnable                           //��©���

/* GPIO �жϴ���ģʽ */
#define PIN_IRQ_MODE_RISING             0x00 //�����أ������������ж�  
#define PIN_IRQ_MODE_FALLING            0x01 //�½��أ������������ж�
#define PIN_IRQ_MODE_RISING_FALLING     0x02 //˫���أ���©�������ж�
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03 //�ߵ�ƽ�������������ж�
#define PIN_IRQ_MODE_LOW_LEVEL          0x04 //�͵�ƽ�������������ж�



/*!
  * @brief    GPIO��ʼ��
  *
  * @param    pin      �� �ܽű��  LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO ģʽ   LQ_GPIO.h�еĺ궨����ѡ��
  * @param    output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_InitConfig(A7, PIN_MODE_OUTPUT, 1);//���ùܽ�A7Ϊ���ģʽ �ߵ�ƽ
  *
  * @date     2019/8/19 ����һ
  */
void PIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output);



/*!
  * @brief    ��ȡGPIO״̬
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  *
  * @return   
  *
  * @note     GPIOҪ��ʼ��Ϊ����ģʽ
  *
  * @example  PIN_Read(A7); //��ȡA7״̬
  *
  * @date     2019/8/19 ����һ
  */
uint8_t PIN_Read(GPIO_Name_t pin);



/*!
  * @brief    ����GPIO���״̬
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    output   �� 1:�ߵ�ƽ  0���͵�ƽ
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_Write(A7, 1);//A7��Ϊ�ߵ�ƽ
  *
  * @date     2019/8/19 ����һ
  */
void PIN_Write(GPIO_Name_t pin, uint8_t output);


/*!
  * @brief    ����GPIO�������
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� 1:���  0������
  *
  * @return   ��
  *
  * @note     ʹ��ǰ���ȳ�ʼ��
  *
  * @example  PIN_Dir(A7, 1);//A7����Ϊ���
  *
  * @date     2019/8/19 ����һ
  */
void PIN_Dir(GPIO_Name_t pin, uint8_t mode);


/*!
  * @brief    GPIO�ⲿ�жϳ�ʼ��
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO �ж�ģʽ   LQ_GPIO.h�еĺ궨����ѡ��
  *
  * @return   
  *
  * @note     
  *
  * @example  PIN_Exti(C13, PIN_IRQ_MODE_FALLING);//����C13 �½��ش����ж�
  * @example  ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
  * @example  NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //ʹ��GPIO2_16~31IO���ж�  
  *
  * @date     2019/8/19 ����һ
  */
void PIN_Exti(GPIO_Name_t pin, uint8_t mode);



/*!< RT1064��GPIO���� GPIO1-GPIO5֮�� GPIO1-GPIO4������ӳ�䵽GPIO6-GPIO9�� 
     GPIO6 - GPIO9������AHB������ ��ת�ٶȱȽϿ� */
#if (defined(CPU_MIMXRT1064CVL5A) || defined(CPU_MIMXRT1064DVL6A)) 
/*!
  * @brief    RT1064 ����IO��ʼ��
  *
  * @param    pin      �� �ܽű��  LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO ģʽ   LQ_GPIO.h�еĺ궨����ѡ��
  * @param    output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
  *
  * @return   
  *
  * @note     L6/L7/K7����ʹ�øú���
  *
  * @example  FASTPIN_InitConfig(A7, PIN_MODE_OUTPUT, 1);//���ùܽ�A7Ϊ���ģʽ �ߵ�ƽ
  *
  * @date     2019/8/19 ����һ
  */
void FASTPIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output);


/*!
  * @brief    ��ȡGPIO״̬
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  *
  * @return   ��
  *
  * @note     GPIOҪ��ʼ��Ϊ����ģʽ
  *
  * @example  FASTPIN_Read(A7); //��ȡA7״̬
  *
  * @date     2019/8/19 ����һ
  */
uint8_t FASTPIN_Read(GPIO_Name_t pin);


/*!
  * @brief    ����GPIO���״̬
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    output   �� 1:�ߵ�ƽ  0���͵�ƽ
  *
  * @return   ��
  *
  * @note     
  *
  * @example  FASTPIN_Write(A7, 1);//A7��Ϊ�ߵ�ƽ
  *
  * @date     2019/8/19 ����һ
  */
void FASTPIN_Write(GPIO_Name_t pin, uint8_t output);


/*!
  * @brief    ����GPIO�������
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� 1:���  0������
  *
  * @return   ��
  *
  * @note     ʹ��ǰ���ȳ�ʼ��
  *
  * @example  FASTPIN_Dir(A7, 1);//A7����Ϊ���
  *
  * @date     2019/8/19 ����һ
  */
void FASTPIN_Dir(GPIO_Name_t pin, uint8_t mode);


/*!
  * @brief    GPIO�ⲿ�жϳ�ʼ��
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO �ж�ģʽ   LQ_GPIO.h�еĺ궨����ѡ��
  *
  * @return   
  *
  * @note     ��ӳ���ϵ�����У����ԭ����GPIO1��ʹ�ô˺����������ж� ���õ���GPIO6
  * @note     GPIO1  ---> GPIO6
  * @note     GPIO2  ---> GPIO7
  * @note     GPIO3  ---> GPIO8
  * @note     GPIO4  ---> GPIO9
  *
  * @example  FASTPIN_Exti(C13, PIN_IRQ_MODE_FALLING);//����C13 �½��ش����ж�
  * @example  ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
  * @example  NVIC_SetPriority(GPIO,7_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO7_Combined_16_31_IRQn);			          //ʹ��GPIO7_16~31IO���ж�  
  *
  * @date     2019/8/19 ����һ
  */
void FASTPIN_Exti(GPIO_Name_t pin, uint8_t mode);

#endif

#endif  
