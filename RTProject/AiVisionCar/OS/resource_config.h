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
      adcx_t ADCN;
      char *Description;
}adc_resource_t;

typedef struct
{
      gpiox_t GPION;
      char *Description;
}gpio_resource_t;

typedef struct
{
      pwmx_t PWMN;
      char *Description;
}pwm_resource_t;

typedef struct
{
      encx_t ENCN;
      char *Description;
}enc_resource_t;

typedef struct
{
      timx_t TIMN;
      char *Description;
}tim_resource_t;

typedef struct
{
      uartx_t UARTN;
      char *Description;
}uart_resource_t;

typedef struct
{
      spix_t SPIN;
      char *Description;
}spi_resource_t;

extern gpio_resource_t GPIO_Resources[20];
extern adc_resource_t ADC_Resources[12];
extern pwm_resource_t PWM_Resources[6];
extern enc_resource_t ENC_Resources[2];
extern tim_resource_t TIM_Resources[3];

extern uart_resource_t UART_Resources[2];
extern spi_resource_t SPI_Resources[2];

#endif /* OS_RESOURCE_CONFIG_H_ */
