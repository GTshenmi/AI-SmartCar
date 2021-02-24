/*
 * resource_config.h
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */

#ifndef OS_RESOURCE_CONFIG_H_
#define OS_RESOURCE_CONFIG_H_

#include "sys_driver.h"

typedef struct
{
    adcx_t ADC[50];
}adc_devices_t;

typedef struct
{
    pwmx_t ATOMPwm[10];
    pwmx_t TOMPwm[2];
}pwm_devices_t;

typedef struct
{
    encx_t ENC[7];
}enc_devices_t;

typedef struct
{
    gpiox_t test_gpio;
    gpiox_t LCD_DC_GPIO ;
    gpiox_t LCD_RST_GPIO ;
    gpiox_t LCD_SDA_GPIO ;
    gpiox_t LCD_SCL_GPIO ;
    gpiox_t LCD_CS_GPIO ;

    gpiox_t Switch1_GPIO;
    gpiox_t Switch2_GPIO;
    gpiox_t Switch3_GPIO;
    gpiox_t Switch4_GPIO;

    gpiox_t GLED_GPIO;
    gpiox_t BLED_GPIO;

    gpiox_t BEEP_GPIO;
    gpiox_t KEY1_GPIO;
    gpiox_t KEY2_GPIO;
    gpiox_t KEY3_GPIO;
    gpiox_t KEY4_GPIO;
    
    gpiox_t Camera_IIC_SCL_GPIO;
    gpiox_t Camera_IIC_SDA_GPIO;
    
}gpio_devices_t;

typedef struct
{
   timx_t SysTimeTIM;
}tim_devices_t;

typedef struct
{
   uartx_t uart_1;
}uart_devices_t;

typedef struct
{
   spix_t spi_1;
   spix_t LCD_SPI;
   spix_t SD_SPI;
}spi_devices_t;


extern adc_devices_t ADC_Devices;
extern gpio_devices_t GPIO_Devices;
extern pwm_devices_t PWM_Devices;
extern enc_devices_t ENC_Devices;
extern tim_devices_t TIM_Devices;
extern uart_devices_t UART_Devices;
extern spi_devices_t SPI_Devices;

#endif /* OS_RESOURCE_CONFIG_H_ */
