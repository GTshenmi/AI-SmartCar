/*
 * resource_config.c
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "resource_config.h"

adc_resource_t ADC_Resources[12] =
{
        [0] = {
               .ADCN = {ADC13,10000},
               .Description = "Short ESensor[0]->AD1"
        },
        [1] = {
               .ADCN = {ADC12,10000},
               .Description = "Short ESensor[1]->AD2"
        },
        [2] = {
               .ADCN = {ADC20,10000},
               .Description = "Short ESensor[2]->AD3 Problem"
        },
        [3] = {
               .ADCN = {ADC10,10000},
               .Description = "Short ESensor[3]->AD4"
        },
        [4] = {
               .ADCN = {ADC11,10000},
               .Description = "Short ESensor[4]->AD5"
        },
        [5] = {
               .ADCN = {ADC6,10000},
               .Description = "Short ESensor[5]->AD6"
        },
        [6] = {
               .ADCN = {ADC3,10000},
               .Description = "Short ESensor[6]->AD7"
        },


        [7] = {
               .ADCN = {ADC5,10000},
               .Description = "Long ESensor[0]->AD9"
        },
        [8] = {
               .ADCN = {ADC7,10000},
               .Description = "Long ESensor[1]->AD10"
        },
        [9] = {
               .ADCN = {ADC2,10000},
               .Description = "Long ESensor[2]->AD11"
        },
        [10] = {
               .ADCN = {ADC4,10000},
               .Description = "Long ESensor[3]->AD12"
        },
        [11] = {
               .ADCN = {ADC0,10000},
               .Description = "Long ESensor[4]->AD13"
        },
};

gpio_resource_t GPIO_Resources[20] =
{
        [0] = {
               .GPION = {P15_8, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_DC"
        },
        [1] = {
               .GPION = {P15_4, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_RST"
        },
        [2] = {
               .GPION = {P15_5, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_SDA"
        },
        [3] = {
               .GPION = {P15_3, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_SCL"
        },
        [4] = {
               .GPION = {P15_2, PIN_MODE_OUTPUT,0,false,false},
               .Description = "LCD_CS"
        },
        [5] = {
               .GPION = {P22_0, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Switch[0]"
        },
        [6] = {
               .GPION = {P33_12, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Switch[1]"
        },
        [7] = {
               .GPION = {P32_4, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Switch[2]"
        },
        [8] = {
               .GPION = {P33_7, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Switch[3]"
        },
        [9] = {
               .GPION = {P10_6, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Green LED"
        },
        [10] = {
               .GPION = {P10_5, PIN_MODE_OUTPUT,1,false,false},
               .Description = "Blue LED"
        },
        [11] = {
               .GPION = {P00_7, PIN_MODE_OUTPUT,1,false,false},
               .Description = "BEEP"
        },
        [12] = {
               .GPION = {P00_9, PIN_MODE_INPUT,1,false,false},
               .Description = "KEY Up"
        },
        [13] = {
               .GPION = {P00_6, PIN_MODE_INPUT,1,false,false},
               .Description = "KEY Down"
        },
        [14] = {
               .GPION = {P00_8, PIN_MODE_INPUT,1,false,false},
               .Description = "Key Exit"
        },
        [15] = {
               .GPION = {P00_5, PIN_MODE_INPUT,1,false,false},
               .Description = "Key Confirm"
        },
};

pwm_resource_t PWM_Resources[3] =
{
        [0] = {
               .PWMN = {(void *)&IfxGtm_ATOM2_0_TOUT32_P33_10_OUT,13000,0,ATOM},
               .Description = "Motor PH"
        },
        [1] = {
               .PWMN = {(void *)&IfxGtm_ATOM2_5_TOUT35_P33_13_OUT,13000,0,ATOM},
               .Description = "Motor EN"
        },
        [2] = {
               .PWMN = {(void *)&IfxGtm_ATOM0_7_TOUT64_P20_8_OUT,13000,0,ATOM},
               .Description = "Servo"
        },
};

enc_resource_t ENC_Resources[2] =
{
        [0] = {
               .ENCN = {ENC6_InPut_P20_3/*ENC6_InPut_P10_2*/, ENC6_Dir_P20_0},
               .Description = "Motor Encoder"
        },
};

tim_resource_t TIM_Resources[2] =
{
        [0] = {
               .TIMN = {STM0,STM_Channel_0,1000,true,true,STM},
               .Description = "System TIM"
        },
};
uart_resource_t UART_Resources[2] =
{
        [0] = {
               .UARTN = {UART1,UART1_RX_P11_10,UART1_TX_P11_12,115200},
               .Description = "CUART And Console."
        },
};
spi_resource_t SPI_Resources[2] =
{
        [0] = {
               .SPIN = {SPI2,QSPI2_CLK_P15_3,QSPI2_MISO_P15_7/*not used*/,QSPI2_MOSI_P15_5, QSPI2_CS_P15_2,3,QSPI,5000000},
               .Description = "LCD SPI"
        },
        [1] = {
                .SPIN = {SPI0,QSPI0_CLK_P20_13,QSPI0_MISO_P20_12,QSPI0_MOSI_P20_14, QSPI0_CS_P20_8,3,QSPI,5000000},
                .Description = "SD Card SPI",
        },
};

