/*
 *  resource_config.c
 *
 *  Created on: 2020��12��8��
 *      Author: 936305695
 */
#include "resource_config.h"

adc_resource_t ADC_Resources[20] =
{
        [0] = {
               .ADCN = {ADC27,10000},
               .Description = "AD1"
               //.Description = "Reversed"
        },

        [1] = {
               .ADCN = {ADC29,10000},
               .Description = "AD2"
        },
        [2] = {
               .ADCN = {ADC26,10000},
               .Description = "AD3"
        },
        [3] = {
               .ADCN = {ADC25,10000},
               .Description = "AD4"
        },
        [4] = {
               .ADCN = {ADC28,10000},
               .Description = "AD5"
        },
        [5] = {
               .ADCN = {ADC12,10000},
               .Description = "AD6"
        },
        [6] = {
               .ADCN = {ADC11,10000},
               .Description = "AD7"
        },

        [7] = {
               .ADCN = {ADC10,10000},
               .Description = "AD8"
        },



        [8] = {
               .ADCN = {ADC1,10000},
               .Description = "AD9"
        },
        [9] = {
               .ADCN = {ADC0,10000},
               .Description = "AD10"
        },
        [10] = {
               .ADCN = {ADC4,10000},
               .Description = "AD11"
        },
        [11] = {
               .ADCN = {ADC2,10000},
               .Description = "AD12"
        },
        [12] = {
               .ADCN = {ADC7,10000},
               .Description = "AD13"
        },
        [13] = {
               .ADCN = {ADC5,10000},
               .Description = "AD14"
        },
        [14] = {
               .ADCN = {ADC8,10000},
               .Description = "AD15"
        },
        [15] = {
               .ADCN = {ADC3,10000},
               .Description = "AD16"
        },

//        [0] = {
//               //.ADCN = {ADC6,10000},
//               .ADCN = {ADC1,10000},
//               .Description = "Short ESensor[0]->Reversed..."        //x dui
//        },
//        [1] = {
//               //.ADCN = {ADC11,10000},
//               .ADCN = {ADC0,10000},
//               .Description = "Short ESensor[1]->AD2"
//        },
//        [2] = {
//               //.ADCN = {ADC10,10000},
//               .ADCN = {ADC4,10000},
//               .Description = "Short ESensor[2]->AD3-"       //x  dui
//        },
//        [3] = {
//               //.ADCN = {ADC21,10000},
//               .ADCN = {ADC2,10000},
//               .Description = "Short ESensor[3]->AD4"
//        },
//        [4] = {
//               //.ADCN = {ADC12,10000},
//               .ADCN = {ADC7,10000},
//               .Description = "Short ESensor[4]->AD5"
//        },
//        [5] = {
//               //.ADCN = {ADC26,10000},
//               .ADCN = {ADC5,10000},
//               .Description = "Short ESensor[5]->AD6"
//        },
//        [6] = {
//               //.ADCN = {ADC24,10000},
//               .ADCN = {ADC8,10000},
//               .Description = "Short ESensor[6]->AD7-"
//        },
//
//        [7] = {
//               //.ADCN = {ADC27,10000},
//               .ADCN = {ADC3,10000},
//               .Description = "Short ESensor[7]->AD8-"       //x  dui
//        },
//
//
//
//        [8] = {
//               .ADCN = {ADC29,10000},
//               .Description = "Long ESensor[0]->AD9"
//        },
//
//
//        [9] = {
//               .ADCN = {ADC26,10000},
//               .Description = "Long ESensor[1]->AD10"
//        },
//        [10] = {
//               .ADCN = {ADC25,10000},
//               .Description = "Long ESensor[2]->AD11"
//        },
//        [11] = {
//               .ADCN = {ADC28,10000},
//               .Description = "Long ESensor[3]->AD12"
//        },
//        [12] = {
//               .ADCN = {ADC12,10000},
//               .Description = "Long ESensor[4]->AD13"
//        },
//        [13] = {
//               .ADCN = {ADC11,10000},
//               .Description = "Long ESensor[5]->AD14"
//        },
//        [14] = {
//               .ADCN = {ADC10,10000},
//               .Description = "Long ESensor[6]->AD15"
//        },
//        [15] = {
//               .ADCN = {ADC3,10000},
//               .Description = "Reserved.->AD16"
//        },

};

gpio_resource_t GPIO_Resources[30] =
{
        [0] = {
               .GPION = {P15_3, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_DC"
        },
        [1] = {
               .GPION = {P15_1, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_RST"
        },
//        [2] = {
//               .GPION = {P15_7, PIN_MODE_OUTPUT,1,false,false},
//               .Description = "LCD_SDA-OLD"
//        },
//        [3] = {
//               .GPION = {P15_8, PIN_MODE_OUTPUT,1,false,false},
//               .Description = "LCD_SCL-OLD"
//        },
        [2] = {
               .GPION = {P15_6, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_SDA"
        },
        [3] = {
               .GPION = {P15_8, PIN_MODE_OUTPUT,1,false,false},
               .Description = "LCD_SCL"
        },
        [4] = {
               .GPION = {P15_2, PIN_MODE_OUTPUT,0,false,false},
               .Description = "LCD_CS"
        },
        [5] = {
               .GPION = {P01_3, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[0]"
        },
        [6] = {
               .GPION = {P02_11, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[1]"
        },
        [7] = {
               .GPION = {P00_11, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[2]"
        },
        [8] = {
               .GPION = {P00_8, PIN_MODE_INPUT,1,false,false},
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
               .GPION = {P20_13, PIN_MODE_OUTPUT,1,false,false},
               .Description = "BEEP"
        },
        [12] = {
               .GPION = {P33_6, PIN_MODE_INPUT,1,false,false},
               .Description = "KEY 0 Cursor UP. "
        },
        [13] = {
               .GPION = {P33_9, PIN_MODE_INPUT,1,false,false},
               .Description = "KEY 1 Cursor Down."
        },
        [14] = {
               .GPION = {P33_10, PIN_MODE_INPUT,1,false,false},
               .Description = "Key 2 Cancel."
        },
        [15] = {
               .GPION = {P33_11, PIN_MODE_INPUT,1,false,false},
               .Description = "Key 3 Confirm."
        },
        [16] = {
               .GPION = {P33_12, PIN_MODE_INPUT,1,false,false},
               .Description = "Key 4 Page UP."
        },
        [17] = {
               .GPION = {P32_4, PIN_MODE_INPUT,1,false,false},
               .Description = "Key 5 Page Down."
        },

        [18] = {
               .GPION = {P00_5, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[4]"
        },
        [19] = {
               .GPION = {P00_2, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[5]"
        },
        [20] = {
               .GPION = {P02_8, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[6]"
        },
        [21] = {
               .GPION = {P02_5, PIN_MODE_INPUT,1,false,false},
               .Description = "Switch[7]"
        },


        [22] = {
               .GPION = {P11_10, PIN_MODE_OUTPUT,1,false,false},
               .Description = "SD CS"
        },
        [23] = {
               .GPION = {P11_3, PIN_MODE_OUTPUT,1,false,false},
               .Description = "SD MISO"
        },
        [24] = {
               .GPION = {P22_3, PIN_MODE_OUTPUT,1,false,false},
               .Description = "ICM CS"
        },

        // QSPI0_CLK_P20_11,QSPI0_MISO_P20_12,QSPI0_MOSI_P20_14,QSPI0_CS_P20_8
         [25] = {
                .GPION = {P20_11, PIN_MODE_OUTPUT,1,false,false},
                .Description = "MPU SCL"
         },
         [26] = {
                .GPION = {P20_14, PIN_MODE_OUTPUT_OD,1,false,false},
                .Description = "MPU SDA"
         },

         [27] = {
                .GPION = {P10_2, PIN_MODE_INPUT_PULLUP, 1, false, false},
                .Description = "ReedTube"
         },

};

pwm_resource_t PWM_Resources[3] =
{
        [0] = {
               .PWMN = {(void *)&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,13000,0,ATOM,0,10000},
               .Description = "Motor PH"
        },
        [1] = {
               .PWMN = {(void *)&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,13000,0,ATOM,0,10000},
               .Description = "Motor EN"
        },
        [2] = {
               .PWMN = {(void *)&IfxGtm_ATOM2_5_TOUT35_P33_13_OUT,50,750,ATOM,250,1250}, //750 - 500,750 + 500
               .Description = "Servo"
        },
};

enc_resource_t ENC_Resources[2] =
{
        [0] = {
               .ENCN = {ENC3_InPut_P02_6/*ENC6_InPut_P10_2*/, ENC3_Dir_P02_7},
               .Description = "Motor Encoder"
        },
};

tim_resource_t TIM_Resources[8] =
{
        [0] = {
               .TIMN = {STM0,STM_Channel_0,1000,false,true,STM},
               .Description = "System TIM---Managed By Core2."
        },
        [1] = {
               .TIMN = {STM0,STM_Channel_1,2000,true,true,STM},
               .Description = "Reversed.---Managed By Core2."
        },

        [2] = {
               .TIMN = {STM1,STM_Channel_0,2000,true,true,STM},
               .Description = "Interrupt Timer For Calculate Bias.---Managed By Core0."
        },
        [3] = {
               .TIMN = {STM1,STM_Channel_1,20000,true,true,STM},
               .Description = "Interrupt Timer For Servo Control.---Managed By Core0."
        },

        [4] = {
               .TIMN = {CCU60,CCU6_Channel0,2000,true,true,CCU6},
               .Description = "Interrupt Timer For Motor Control.---Managed By Core1."
        },

        [5] = {
               .TIMN = {CCU60,CCU6_Channel1,2000,true,true,CCU6},
               .Description = "Interrupt Timer For IMU Update.---Managed By Core1."
        },

};

uart_resource_t UART_Resources[2] =
{
        [0] = {
               .UARTN = {UART3,UART3_RX_P00_1,UART3_TX_P00_0,115200,0},
               .Description = "CUART And Console."
        },
};

spi_resource_t SPI_Resources[3] =
{
        [0] = {
               .SPIN = {SPI2,QSPI2_CLK_P15_8,QSPI2_MISO_P15_7/*not used*/,QSPI2_MOSI_P15_6,QSPI2_CS_P15_2,2,QSPI,32000000},
               .Description = "LCD SPI"
        },
        [1] = {
                .SPIN = {SPI1,QSPI1_CLK_P11_6,QSPI1_MISO_P11_3,QSPI1_MOSI_P11_9, QSPI1_CS_P33_5,2,QSPI,300000},
                .Description = "SD Card SPI",
        },
        [2] = {
                .SPIN = {SPI0,QSPI0_CLK_P20_11,QSPI0_MISO_P20_12,QSPI0_MOSI_P20_14,QSPI0_CS_P20_8,3,QSPI,8000000},
                .Description = "ICM20602",
        },
};

