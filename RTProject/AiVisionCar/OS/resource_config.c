/*
 * resource_config.c
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "resource_config.h"

adc_resource_t ADC_Resources[12];

gpio_resource_t GPIO_Resources[20]=
{ 
  [0]={
    .GPION={N4,PIN_MODE_OUTPUT,1,false,false},
    .Description = "LCD_DC_GPIO",
  },  
  [1]={
    .GPION={P5,PIN_MODE_OUTPUT,1,false,false},
    .Description = "LCD_RST_GPIO",
  },   
  [2]={
    .GPION={P3,PIN_MODE_OUTPUT,1,false,false},
    .Description = "LCD_SDA_GPIO",
  },
  [3]={
    .GPION={L4,PIN_MODE_OUTPUT,1,false,false},
    .Description = "LCD_SCL_GPIO",
  },  
  [4]={
    .GPION={L3,PIN_MODE_OUTPUT,0,false,false},
    .Description = "LCD_CS_GPIO",
  },  

  [5]={
    .GPION={A7,PIN_MODE_OUTPUT,1,false,false},
    .Description = "LED",
  },
  
  [6]={
    .GPION={L11,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 0",
  },  
  [7]={
    .GPION={L10,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 1",
  },   
  [8]={
    .GPION={K7,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 2",
  },
  [9]={
    .GPION={M5,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 3",
  },  
  [10]={
    .GPION={N3,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 4",
  },
  [11]={
    .GPION={M4,PIN_MODE_INPUT,1,false,false},
    .Description = "KEY 5",
  },  
  
  [12]={
    .GPION={J11,PIN_MODE_INPUT,1,false,false},
    .Description = "CSI SCL",
  },
  [13]={
    .GPION={K11,PIN_MODE_INPUT,1,false,false},
    .Description = "CSI SDA",
  },    
  
};

pwm_resource_t PWM_Resources[6] = 
{
  [0] = {
    .PWMN = {PWM3,kPWM_Module_3,kPWM_PwmB,13000,0},
    .Description = "L_PH_IN F1",
  },
  [1] = {
    .PWMN = {PWM1,kPWM_Module_0,kPWM_PwmA,13000,0},
    .Description = "L_EN_IN G2",
  },
  
  [2] = {
    .PWMN = {PWM2,kPWM_Module_2,kPWM_PwmA,13000,0},
    .Description = "R_PH_IN G1",
  } , 
  [3] = {
    .PWMN = {PWM2,kPWM_Module_2,kPWM_PwmB,13000,0},
    .Description = "R_EN_IN G3",
  },
  
  [4] = {
    .PWMN = {PWM4,kPWM_Module_3,kPWM_PwmA,50,0},
    .Description = "Servo1 A4",
  } , 
  [5] = {
    .PWMN = {PWM4,kPWM_Module_2,kPWM_PwmA,50,0},
    .Description = "Servo2 C14",
  },
  
};

enc_resource_t ENC_Resources[2] = 
{
  [0] = {
    .ENCN = {ENC4,true},
    .Description = "L ENC LSB-M11 DIR-G11",
  }, 
  [1] = {
    .ENCN = {ENC3,true},
    .Description = "R ENC LSB-H1 DIR-E4",
  },
};

tim_resource_t TIM_Resources[2];
uart_resource_t UART_Resources[2];
spi_resource_t SPI_Resources[2];

