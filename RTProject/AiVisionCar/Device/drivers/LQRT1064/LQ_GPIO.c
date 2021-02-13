/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��13��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_Cfg.h"

#define __RT1052_PIN(INDEX, PORT, PIN) {INDEX, PORT, PIN}
#define __RT1052_PIN_DEFAULT {0, 0, 0}

struct rt1052_pin
{
    uint8_t       index;     //Ϊ�˷��㣬�żĴ����ֲ��ŵ�˳��
    GPIO_Type    *gpio;
    uint8_t       pin;
    
};

/* ӳ���ϵ���� */
static struct rt1052_pin rt_pin_map[] =
{
    __RT1052_PIN_DEFAULT,           /* A1 */
	__RT1052_PIN(28, GPIO4, 27),    /* A2 GPIO_EMC_27 */
	__RT1052_PIN(21, GPIO4, 20),    /* A3 GPIO_EMC_20 */
	__RT1052_PIN(18, GPIO4, 17),    /* A4 GPIO_EMC_17 */
	__RT1052_PIN(17, GPIO4, 16),    /* A5 GPIO_EMC_16 */
	__RT1052_PIN(14, GPIO4, 13),    /* A6 GPIO_EMC_13 */
	__RT1052_PIN(41, GPIO3, 26),    /* A7 GPIO_EMC_40 */
	__RT1052_PIN(81, GPIO2,  6),    /* A8 GPIO_B0_06 */
	__RT1052_PIN(82, GPIO2,  7),    /* A9 GPIO_B0_07 */
	__RT1052_PIN(86, GPIO2, 11),    /* A10 GPIO_B0_11 */
	__RT1052_PIN(91, GPIO2, 16),    /* A11 GPIO_B1_00 */
	__RT1052_PIN(99, GPIO2, 24),    /* A12 GPIO_B1_08 */
	__RT1052_PIN(100,GPIO2, 25),    /* A13 GPIO_B1_09 */
	__RT1052_PIN_DEFAULT,           /* A14 */
	

	__RT1052_PIN(16, GPIO4, 15),    /* B1 GPIO_EMC_15 */
	__RT1052_PIN(19, GPIO4, 18),    /* B2 GPIO_EMC_18 */
	__RT1052_PIN(27, GPIO4, 26),    /* B3 GPIO_EMC_26 */
	__RT1052_PIN(20, GPIO4, 19),    /* B4 GPIO_EMC_19 */
	__RT1052_PIN_DEFAULT,           /* B5  */
	__RT1052_PIN(15, GPIO4, 14),    /* B6 GPIO_EMC_14 */
	__RT1052_PIN(40, GPIO3, 25),    /* B7 GPIO_EMC_39 */
	__RT1052_PIN(80, GPIO2,  5),    /* B8 GPIO_B0_05 */
	__RT1052_PIN(83, GPIO2,  8),    /* B9 GPIO_B0_08 */
	__RT1052_PIN_DEFAULT,           /* B10  */
	__RT1052_PIN(92, GPIO2, 17),    /* B11 GPIO_B1_01 */
	__RT1052_PIN(98, GPIO2, 23),    /* B12 GPIO_B1_07 */
	__RT1052_PIN(101,GPIO2, 26),    /* B13 GPIO_B1_10 */
	__RT1052_PIN(106,GPIO2, 31),    /* B14 GPIO_B1_15 */
	

	__RT1052_PIN(22, GPIO4, 21),    /* C1 GPIO_EMC_21 */
	__RT1052_PIN(10, GPIO4,  9),    /* C2 GPIO_EMC_09 */
	__RT1052_PIN(37, GPIO3, 22),    /* C3 GPIO_EMC_36 */
	__RT1052_PIN(34, GPIO3, 19),    /* C4 GPIO_EMC_33 */
	__RT1052_PIN(32, GPIO4, 31),    /* C5 GPIO_EMC_31 */
	__RT1052_PIN(31, GPIO4, 30),    /* C6 GPIO_EMC_30 */
	__RT1052_PIN(42, GPIO3, 27),    /* C7 GPIO_EMC_41 */
	__RT1052_PIN(79, GPIO2,  4),    /* C8 GPIO_B0_04 */
	__RT1052_PIN(84, GPIO2,  9),    /* C9 GPIO_B0_09 */
	__RT1052_PIN(87, GPIO2, 12),    /* C10 GPIO_B0_12 */
	__RT1052_PIN(93, GPIO2, 18),    /* C11 GPIO_B1_02 */
	__RT1052_PIN(97, GPIO2, 22),    /* C12 GPIO_B1_06 */
	__RT1052_PIN(102,GPIO2, 27),    /* C13 GPIO_B1_11 */
	__RT1052_PIN(105,GPIO2, 30),    /* C14 GPIO_B1_14 */
	

	__RT1052_PIN(29, GPIO4, 28),    /* D1 GPIO_EMC_28 */
	__RT1052_PIN(26, GPIO4, 25),    /* D2 GPIO_EMC_25 */
	__RT1052_PIN(25, GPIO4, 24),    /* D3 GPIO_EMC_24 */
	__RT1052_PIN(35, GPIO3, 20),    /* D4 GPIO_EMC_34 */
	__RT1052_PIN(33, GPIO3, 18),    /* D5 GPIO_EMC_32 */
	__RT1052_PIN(39, GPIO3, 24),    /* D6 GPIO_EMC_38 */
	__RT1052_PIN(75, GPIO2,  0),    /* D7 GPIO_B0_00 */
	__RT1052_PIN(78, GPIO2,  3),    /* D8 GPIO_B0_03 */
	__RT1052_PIN(85, GPIO2, 10),    /* D9 GPIO_B0_10 */
	__RT1052_PIN(88, GPIO2, 13),    /* D10 GPIO_B0_13 */
	__RT1052_PIN(94, GPIO2, 19),    /* D11 GPIO_B1_03 */
	__RT1052_PIN(96, GPIO2, 21),    /* D12 GPIO_B1_05 */
	__RT1052_PIN(103,GPIO2, 28),    /* D13 GPIO_B1_12 */
	__RT1052_PIN(104,GPIO2, 29),    /* D14 GPIO_B1_13 */
	

	__RT1052_PIN(30, GPIO4, 29),    /* E1 GPIO_EMC_29 */
	__RT1052_PIN_DEFAULT,           /* E2  */
	__RT1052_PIN(1,  GPIO4,  0),    /* E3 GPIO_EMC_00 */
	__RT1052_PIN(38, GPIO3, 23),    /* E4 GPIO_EMC_37 */
	__RT1052_PIN(36, GPIO3, 21),    /* E5 GPIO_EMC_35 */
	__RT1052_PIN_DEFAULT,           /* E6  */
	__RT1052_PIN(76, GPIO2,  1),    /* E7 GPIO_B0_01 */
	__RT1052_PIN(77, GPIO2,  2),    /* E8 GPIO_B0_02 */
	__RT1052_PIN_DEFAULT,           /* E9  */
	__RT1052_PIN(89, GPIO2, 14),    /* E10 GPIO_B0_14 */
	__RT1052_PIN(90, GPIO2, 15),    /* E11 GPIO_B0_15 */
	__RT1052_PIN(95, GPIO2, 20),    /* E12 GPIO_B1_04 */
	__RT1052_PIN_DEFAULT,           /* E13  */
	__RT1052_PIN(49, GPIO1,  6),    /* E14 GPIO_AD_B0_06 */
	

	__RT1052_PIN(23, GPIO4, 22),    /* F1 GPIO_EMC_22 */
	__RT1052_PIN(5,  GPIO4,  4),    /* F2 GPIO_EMC_04 */
	__RT1052_PIN(2,  GPIO4,  1),    /* F3 GPIO_EMC_01 */
	__RT1052_PIN(3,  GPIO4,  2),    /* F4 GPIO_EMC_02 */
	__RT1052_PIN_DEFAULT,           /* F5  */
	__RT1052_PIN_DEFAULT,           /* F6  */
	__RT1052_PIN_DEFAULT,           /* F7  */
	__RT1052_PIN_DEFAULT,           /* F8  */
	__RT1052_PIN_DEFAULT,           /* F9  */
	__RT1052_PIN_DEFAULT,           /* F10  */
	__RT1052_PIN(47, GPIO1,  4),    /* F11 GPIO_AD_B0_04 */
	__RT1052_PIN(50, GPIO1,  7),    /* F12 GPIO_AD_B0_07 */
	__RT1052_PIN(51, GPIO1,  8),    /* F13 GPIO_AD_B0_08 */
	__RT1052_PIN(52, GPIO1,  9),    /* F14 GPIO_AD_B0_09 */
	

	__RT1052_PIN(11, GPIO4, 10),    /* G1 GPIO_EMC_10 */
	__RT1052_PIN(24, GPIO4, 23),    /* G2 GPIO_EMC_23 */
	__RT1052_PIN(12, GPIO4, 11),    /* G3 GPIO_EMC_11 */
	__RT1052_PIN(4,  GPIO4,  3),    /* G4 GPIO_EMC_03 */
	__RT1052_PIN(6,  GPIO4,  5),    /* G5 GPIO_EMC_05 */
	__RT1052_PIN_DEFAULT,           /* G6  */
	__RT1052_PIN_DEFAULT,           /* G7  */
	__RT1052_PIN_DEFAULT,           /* G8  */
	__RT1052_PIN_DEFAULT,           /* G9  */
	__RT1052_PIN(54, GPIO1, 11),    /* G10 GPIO_AD_B0_11 */
	__RT1052_PIN(46, GPIO1,  3),    /* G11 GPIO_AD_B0_03 */
	__RT1052_PIN(73, GPIO1, 30),    /* G12 GPIO_AD_B1_14 */
	__RT1052_PIN(53, GPIO1, 10),    /* G13 GPIO_AD_B0_10 */
	__RT1052_PIN(48, GPIO1,  5),    /* G14 GPIO_AD_B0_05 */
	

	__RT1052_PIN(13, GPIO4, 12),    /* H1 GPIO_EMC_12 */
	__RT1052_PIN(123,GPIO3, 16),    /* H2 GPIO_SD_B0_04 */
	__RT1052_PIN(9,  GPIO4,  8),    /* H3 GPIO_EMC_08 */
	__RT1052_PIN(8,  GPIO4,  7),    /* H4 GPIO_EMC_07 */
	__RT1052_PIN(7,  GPIO4,  6),    /* H5 GPIO_EMC_06 */
	__RT1052_PIN_DEFAULT,           /* H6  */
	__RT1052_PIN_DEFAULT,           /* H7  */
	__RT1052_PIN_DEFAULT,           /* H8  */
	__RT1052_PIN_DEFAULT,           /* H9  */
	__RT1052_PIN(44, GPIO1,  1),    /* H10 GPIO_AD_B0_01 */
	__RT1052_PIN(72, GPIO1, 29),    /* H11 GPIO_AD_B1_13 */
	__RT1052_PIN(71, GPIO1, 28),    /* H12 GPIO_AD_B1_12 */
	__RT1052_PIN(67, GPIO1, 24),    /* H13 GPIO_AD_B1_08 */
	__RT1052_PIN(57, GPIO1, 14),    /* H14 GPIO_AD_B0_14 */
	

	__RT1052_PIN(121,GPIO3, 14),    /* J1 GPIO_SD_B0_02 */
	__RT1052_PIN(124,GPIO3, 17),    /* J2 GPIO_SD_B0_05 */
	__RT1052_PIN(120,GPIO3, 13),    /* J3 GPIO_SD_B0_01 */
	__RT1052_PIN(119,GPIO3, 12),    /* J4 GPIO_SD_B0_00 */
	__RT1052_PIN_DEFAULT,           /* J5  */
	__RT1052_PIN_DEFAULT,           /* J6  */
	__RT1052_PIN_DEFAULT,           /* J7  */
	__RT1052_PIN_DEFAULT,           /* J8  */
	__RT1052_PIN_DEFAULT,           /* J9  */
	__RT1052_PIN_DEFAULT,           /* J10  */
	__RT1052_PIN(59, GPIO1, 16),    /* J11 GPIO_AD_B1_00 */
	__RT1052_PIN(65, GPIO1, 22),    /* J12 GPIO_AD_B1_06 */
	__RT1052_PIN(70, GPIO1, 27),    /* J13 GPIO_AD_B1_11 */
	__RT1052_PIN(74, GPIO1, 31),    /* J14 GPIO_AD_B1_15 */
	

	__RT1052_PIN(122,GPIO3, 15),    /* K1 GPIO_SD_B0_03 */
	__RT1052_PIN_DEFAULT,           /* K2  */
	__RT1052_PIN_DEFAULT,           /* K3  */
	__RT1052_PIN_DEFAULT,           /* K4  */
	__RT1052_PIN_DEFAULT,           /* K5  */
	__RT1052_PIN_DEFAULT,           /* K6  */
	__RT1052_PIN(126,GPIO5,  1),    /* K7 PMIC_ON_REQ */
	__RT1052_PIN_DEFAULT,           /* K8  */
	__RT1052_PIN_DEFAULT,           /* K9  */
	__RT1052_PIN(66, GPIO1, 23),    /* K10 GPIO_AD_B1_07 */
	__RT1052_PIN(60, GPIO1, 17),    /* K11 GPIO_AD_B1_01 */
	__RT1052_PIN(64, GPIO1, 21),    /* K12 GPIO_AD_B1_05 */
	__RT1052_PIN_DEFAULT,           /* K13  */
	__RT1052_PIN(55, GPIO1, 12),    /* K14 GPIO_AD_B0_12 */
	

	__RT1052_PIN_DEFAULT,           /* L1  */
	__RT1052_PIN_DEFAULT,           /* L2  */
	__RT1052_PIN(113,GPIO3,  6),    /* L3 GPIO_SD_B1_06 */
	__RT1052_PIN(114,GPIO3,  7),    /* L4 GPIO_SD_B1_07 */
	__RT1052_PIN(107,GPIO3,  0),    /* L5 GPIO_SD_B1_00 */
	__RT1052_PIN(125,GPIO5,  0),    /* L6 WAKEUP */
	__RT1052_PIN(127,GPIO5,  2),    /* L7 PMIC_STBY_REQ */
	__RT1052_PIN_DEFAULT,           /* L8 */
	__RT1052_PIN_DEFAULT,           /* L9  */
	__RT1052_PIN(58, GPIO1, 15),    /* L10 GPIO_AD_B0_15 */
	__RT1052_PIN(61, GPIO1, 18),    /* L11 GPIO_AD_B1_02 */
	__RT1052_PIN(63, GPIO1, 20),    /* L12 GPIO_AD_B1_04 */
	__RT1052_PIN(69, GPIO1, 26),    /* L13 GPIO_AD_B1_10 */
	__RT1052_PIN(56, GPIO1, 13),    /* L14 GPIO_AD_B0_13 */
	

	__RT1052_PIN_DEFAULT,           /* M1  */
	__RT1052_PIN_DEFAULT,           /* M2  */
	__RT1052_PIN(109,GPIO3,  2),    /* M3 GPIO_SD_B1_02 */
	__RT1052_PIN(110,GPIO3,  3),    /* M4 GPIO_SD_B1_03 */
	__RT1052_PIN(108,GPIO3,  1),    /* M5 GPIO_SD_B1_01 */
	__RT1052_PIN_DEFAULT,           /* M6  */
	__RT1052_PIN_DEFAULT,           /* M7  */
	__RT1052_PIN_DEFAULT,           /* M8  */
	__RT1052_PIN_DEFAULT,           /* M9  */
	__RT1052_PIN_DEFAULT,           /* M10  */
	__RT1052_PIN(45, GPIO1,  2),    /* M11 GPIO_AD_B0_02 */
	__RT1052_PIN(62, GPIO1, 19),    /* M12 GPIO_AD_B1_03 */
	__RT1052_PIN(68, GPIO1, 25),    /* M13 GPIO_AD_B1_09 */
	__RT1052_PIN(43, GPIO1,  0),    /* M14 GPIO_AD_B0_00 */

    
    __RT1052_PIN_DEFAULT,           /* N1  */
	__RT1052_PIN_DEFAULT,           /* N2  */
	__RT1052_PIN(112,GPIO3,  5),    /* N3 GPIO_SD_B1_05 */
	__RT1052_PIN(116,GPIO3,  9),    /* N4 GPIO_SD_B1_09 */
	__RT1052_PIN_DEFAULT,           /* N5  */
	__RT1052_PIN_DEFAULT,           /* N6  */
	__RT1052_PIN_DEFAULT,           /* N7  */
	__RT1052_PIN_DEFAULT,           /* N8  */
	__RT1052_PIN_DEFAULT,           /* N9  */
	__RT1052_PIN_DEFAULT,           /* N10  */
	__RT1052_PIN_DEFAULT,           /* N11  */
	__RT1052_PIN_DEFAULT,           /* N12  */
	__RT1052_PIN_DEFAULT,           /* N13  */
	__RT1052_PIN_DEFAULT,           /* N14  */


    __RT1052_PIN_DEFAULT,           /* P1  */
	__RT1052_PIN(111,GPIO3,  4),    /* P2 GPIO_SD_B1_04 */
	__RT1052_PIN(115,GPIO3,  8),    /* P3 GPIO_SD_B1_08 */
	__RT1052_PIN(117,GPIO3, 10),    /* P4 GPIO_SD_B1_10 */
    __RT1052_PIN(118,GPIO3, 11),    /* P5 GPIO_SD_B1_11 */
	__RT1052_PIN_DEFAULT,           /* P6  */
	__RT1052_PIN_DEFAULT,           /* P7  */
	__RT1052_PIN_DEFAULT,           /* P8  */
	__RT1052_PIN_DEFAULT,           /* P9  */
	__RT1052_PIN_DEFAULT,           /* P10  */
	__RT1052_PIN_DEFAULT,           /* P11  */
	__RT1052_PIN_DEFAULT,           /* P12  */
	__RT1052_PIN_DEFAULT,           /* P13  */
	__RT1052_PIN_DEFAULT,           /* P14  */
      
};



/*!
  * @brief    GPIO��ʼ��
  *
  * @param    pin      �� �ܽű��  LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO ģʽ   LQ_GPIO.h�е�GPIO ����ģʽ�궨����ѡ��
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
void PIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output)
{
	gpio_pin_config_t gpio;
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + rt_pin_map[pin].index * 4U, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_IomuxcSnvs);
        IOMUXC_SetPinMux(0x400A8000U + (rt_pin_map[pin].index - 125U) * 4, 0x5U, 0, 0, 0, 1);
    }
    
    gpio.outputLogic = output;
    gpio.interruptMode = kGPIO_NoIntmode;
    
    gpio.direction = (gpio_pin_direction_t)((mode & 0x8000000)>>27);

    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + rt_pin_map[pin].index * 4U, mode);
    }
    else
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x400A8018U + (rt_pin_map[pin].index - 125U) * 4, mode);
    }
    
    GPIO_PinInit(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, &gpio);
}


/*!
  * @brief    ��ȡGPIO״̬
  *
  * @param    pin      �� �ܽű�� LQ_GPIO_Cfg.h�ж���
  *
  * @return   ��
  *
  * @note     GPIOҪ��ʼ��Ϊ����ģʽ
  *
  * @example  PIN_Read(A7); //��ȡA7״̬
  *
  * @date     2019/8/19 ����һ
  */
uint8_t PIN_Read(GPIO_Name_t pin)
{
	return GPIO_PinRead(rt_pin_map[pin].gpio, rt_pin_map[pin].pin);
}


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
  * @example  PIN_Write(A7, 1);//A7��Ϊ�ߵ�ƽ
  *
  * @date     2019/8/19 ����һ
  */
void PIN_Write(GPIO_Name_t pin, uint8_t output)
{
	GPIO_PinWrite(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, output);
}


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
void PIN_Dir(GPIO_Name_t pin, uint8_t mode)
{
    if (mode)
    {
        rt_pin_map[pin].gpio->GDIR |= (1U << rt_pin_map[pin].pin);
    }        
    else  
    {
        rt_pin_map[pin].gpio->GDIR &= ~(1U << rt_pin_map[pin].pin);
    }
}


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
void PIN_Exti(GPIO_Name_t pin, uint8_t mode)
{
	gpio_pin_config_t gpio;
    uint32_t config_value = 0; 
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + rt_pin_map[pin].index * 4U, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_IomuxcSnvs);
        IOMUXC_SetPinMux(0x400A8000U + (rt_pin_map[pin].index - 125U) * 4, 0x5U, 0, 0, 0, 1);
    }
    
    gpio.direction     = kGPIO_DigitalInput;
    gpio.outputLogic   = 0;
    
    switch (mode)
    {
      case PIN_IRQ_MODE_RISING:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntRisingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_FALLING:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_RISING_FALLING:
        {
            config_value = 0x0830U;/* Open Drain Enable */
            gpio.interruptMode = kGPIO_IntRisingOrFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_HIGH_LEVEL:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntHighLevel;
        }
        break;
        
      case PIN_IRQ_MODE_LOW_LEVEL:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntLowLevel;
        }
        break;
    }
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + rt_pin_map[pin].index * 4U, config_value);
    }
    else
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x400A8018U + (pin - rt_pin_map[pin].index) * 4U, config_value);
    }

    GPIO_PinInit(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, &gpio);
    GPIO_PortEnableInterrupts(rt_pin_map[pin].gpio, 1U << rt_pin_map[pin].pin);
    
}


/*!< RT1064��GPIO���� GPIO1-GPIO5֮�� GPIO1-GPIO4������ӳ�䵽GPIO6-GPIO9�� 
     GPIO6 - GPIO9������AHB������ ��ת�ٶȱȽϿ� */
#if (defined(CPU_MIMXRT1064CVL5A) || defined(CPU_MIMXRT1064DVL6A))  



/*!
  * @brief    RT1064 ����IO��ʼ��
  *
  * @param    pin      �� �ܽű��  LQ_GPIO_Cfg.h�ж���
  * @param    mode     �� GPIO ģʽ   LQ_GPIO.h�е�GPIO ����ģʽ�궨����ѡ��
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
void FASTPIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output)
{
    gpio_pin_config_t gpio;
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + rt_pin_map[pin].index * 4U, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        assert(0);
    }

    gpio.outputLogic = output;
    gpio.interruptMode = kGPIO_NoIntmode;
    
    gpio.direction = (gpio_pin_direction_t)((mode & 0x8000000)>>27);


    IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + rt_pin_map[pin].index * 4U, mode);

    switch((uint32_t)rt_pin_map[pin].gpio)
    {
      case GPIO1_BASE:
        IOMUXC_GPR->GPR26 = (IOMUXC_GPR->GPR26 
                            | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO1 and GPIO6 share same IO MUX function */
        GPIO_PinInit(GPIO6, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO2_BASE:
        IOMUXC_GPR->GPR27 = (IOMUXC_GPR->GPR27
                            | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO2 and GPIO7 share same IO MUX function */
        GPIO_PinInit(GPIO7, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO3_BASE:
        IOMUXC_GPR->GPR28 = (IOMUXC_GPR->GPR28 
                            | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO3 and GPIO8 share same IO MUX function */
        GPIO_PinInit(GPIO8, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO4_BASE:
        IOMUXC_GPR->GPR29 = (IOMUXC_GPR->GPR29 
                            | IOMUXC_GPR_GPR29_GPIO_MUX4_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO4 and GPIO9 share same IO MUX function */
        GPIO_PinInit(GPIO9, rt_pin_map[pin].pin, &gpio);
        break;
    }
    
}


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
uint8_t FASTPIN_Read(GPIO_Name_t pin)
{
	return GPIO_PinRead((GPIO_Type *)(0x01E48000UL + (uint32_t)rt_pin_map[pin].gpio), rt_pin_map[pin].pin);
}


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
void FASTPIN_Write(GPIO_Name_t pin, uint8_t output)
{
	GPIO_PinWrite((GPIO_Type *)(0x01E48000UL + (uint32_t)rt_pin_map[pin].gpio), rt_pin_map[pin].pin, output);
}


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
void FASTPIN_Dir(GPIO_Name_t pin, uint8_t mode)
{
	if (mode)
    {
        ((GPIO_Type *)(0x01E48000UL + (uint32_t)rt_pin_map[pin].gpio))->GDIR |= (1U << rt_pin_map[pin].pin);
    }        
    else  
    {
        ((GPIO_Type *)(0x01E48000UL + (uint32_t)rt_pin_map[pin].gpio))->GDIR &= ~(1U << rt_pin_map[pin].pin);
    }
}


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
void FASTPIN_Exti(GPIO_Name_t pin, uint8_t mode)
{
	gpio_pin_config_t gpio;
    uint32_t config_value = 0; 
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + rt_pin_map[pin].index * 4U, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        assert(0);
    }
    
    gpio.direction     = kGPIO_DigitalInput;
    gpio.outputLogic   = 0;
    
    switch (mode)
    {
      case PIN_IRQ_MODE_RISING:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntRisingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_FALLING:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_RISING_FALLING:
        {
            config_value = 0x0830U;/* Open Drain Enable */
            gpio.interruptMode = kGPIO_IntRisingOrFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_HIGH_LEVEL:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntHighLevel;
        }
        break;
        
      case PIN_IRQ_MODE_LOW_LEVEL:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntLowLevel;
        }
        break;
    }
    

    IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + rt_pin_map[pin].index * 4U, config_value);

    switch((uint32_t)rt_pin_map[pin].gpio)
    {
      case GPIO1_BASE:
        IOMUXC_GPR->GPR26 = (IOMUXC_GPR->GPR26 
                            | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO1 and GPIO6 share same IO MUX function */
        GPIO_PinInit(GPIO6, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO2_BASE:
        IOMUXC_GPR->GPR27 = (IOMUXC_GPR->GPR27
                            | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO2 and GPIO7 share same IO MUX function */
        GPIO_PinInit(GPIO7, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO3_BASE:
        IOMUXC_GPR->GPR28 = (IOMUXC_GPR->GPR28 
                            | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO3 and GPIO8 share same IO MUX function */
        GPIO_PinInit(GPIO8, rt_pin_map[pin].pin, &gpio);
        break;
      case GPIO4_BASE:
        IOMUXC_GPR->GPR29 = (IOMUXC_GPR->GPR29 
                            | IOMUXC_GPR_GPR29_GPIO_MUX4_GPIO_SEL(1U<<rt_pin_map[pin].pin) );/* GPIO4 and GPIO9 share same IO MUX function */
        GPIO_PinInit(GPIO9, rt_pin_map[pin].pin, &gpio);
        break;
    }

    GPIO_PortEnableInterrupts((GPIO_Type *)(0x01E48000UL + (uint32_t)rt_pin_map[pin].gpio), 1U << rt_pin_map[pin].pin);
    
}


#endif
