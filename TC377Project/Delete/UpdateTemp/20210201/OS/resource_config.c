/*
 * resource_config.c
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "resource_config.h"

adc_devices_t ADC_Devices =
{
        .ADC[0] = {{ADC0,10000}},
        .ADC[1] = {{ADC1,10000}},
        .ADC[2] = {{ADC2,10000}},
        .ADC[3] = {{ADC3,10000}},
        .ADC[4] = {{ADC4,10000}},
        .ADC[5] = {{ADC5,10000}},
        .ADC[6] = {{ADC6,10000}},
        .ADC[7] = {{ADC7,10000}},
        .ADC[8] = {{ADC8,10000}},
        .ADC[10] = {{ADC10,10000}},
        .ADC[11] = {{ADC11,10000}},
        .ADC[12] = {{ADC12,10000}},
        .ADC[13] = {{ADC13,10000}},
        .ADC[16] = {{ADC16,10000}},
        .ADC[17] = {{ADC17,10000}},
        .ADC[20] = {{ADC20,10000}},
        .ADC[25] = {{ADC25,10000}},
        .ADC[35] = {{ADC35,10000}},
        .ADC[36] = {{ADC36,10000}},
        .ADC[37] = {{ADC37,10000}},
        .ADC[38] = {{ADC38,10000}},
        .ADC[39] = {{ADC39,10000}},
        .ADC[44] = {{ADC44,10000}},
        .ADC[45] = {{ADC45,10000}},
        .ADC[46] = {{ADC46,10000}},
        .ADC[47] = {{ADC47,10000}},
//        .ADC[48] = {{ADC48,10000}},
//        .ADC[49] = {{ADC49,10000}},
};

gpio_devices_t GPIO_Devices =
{


    .LCD_DC_GPIO = {P14_4, PIN_MODE_OUTPUT,1,false,false},
    .LCD_RST_GPIO = {P15_8, PIN_MODE_OUTPUT,1,false,false},
    .LCD_SDA_GPIO = {P13_0, PIN_MODE_OUTPUT,1,false,false},
    .LCD_SCL_GPIO = {P14_5, PIN_MODE_OUTPUT,1,false,false},
    .LCD_CS_GPIO = {P13_2, PIN_MODE_OUTPUT,1,false,false},

    .Switch1_GPIO = {P22_0, PIN_MODE_OUTPUT,1,false,false},
    .Switch2_GPIO = {P33_12, PIN_MODE_OUTPUT,1,false,false},
    .Switch3_GPIO = {P32_4, PIN_MODE_OUTPUT,1,false,false},
    .Switch4_GPIO = {P33_13, PIN_MODE_OUTPUT,1,false,false},

    .GLED_GPIO = {P10_6, PIN_MODE_OUTPUT,1,false,false},
    .BLED_GPIO = {P10_5, PIN_MODE_OUTPUT,1,false,false},
    .BEEP_GPIO = {P00_7, PIN_MODE_OUTPUT,1,false,false},

    .KEY1_GPIO = {P00_9, PIN_MODE_INPUT,1,false,false},
    .KEY2_GPIO = {P00_6, PIN_MODE_INPUT,1,false,false},
    .KEY3_GPIO = {P00_8, PIN_MODE_INPUT,1,false,false},
    .KEY4_GPIO = {P00_5, PIN_MODE_INPUT,1,false,false},
};

pwm_devices_t PWM_Devices =
{
    .TOMPwm[0]  = {{(void *)&IfxGtm_TOM0_0_TOUT32_P33_10_OUT,5000,100,TOM}},
//    .TOMPwm[1]  = {{(void *)&IfxGtm_TOM0_13_TOUT35_P33_13_OUT,5000,100,TOM}},
//
//    .ATOMPwm[0] = {{(void *)&IfxGtm_ATOM0_2_TOUT107_P10_5_OUT,5000,100,ATOM}},
    .ATOMPwm[1] = {{(void *)&IfxGtm_ATOM0_5_TOUT40_P32_4_OUT,5000,100,ATOM}},
    .ATOMPwm[2] = {{(void *)&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,5000,100,ATOM}},
    .ATOMPwm[3] = {{(void *)&IfxGtm_ATOM0_4_TOUT50_P22_3_OUT,5000,100,ATOM}},
    .ATOMPwm[4] = {{(void *)&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,5000,100,ATOM}},
    .ATOMPwm[5] = {{(void *)&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,5000,100,ATOM}},
    .ATOMPwm[6] = {{(void *)&IfxGtm_ATOM0_7_TOUT64_P20_8_OUT,5000,100,ATOM}},
    .ATOMPwm[7] = {{(void *)&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,13000,0,ATOM}},
    .ATOMPwm[8] = {{(void *)&IfxGtm_ATOM2_0_TOUT32_P33_10_OUT,13000,0,ATOM}},
    .ATOMPwm[9] = {{(void *)&IfxGtm_ATOM2_5_TOUT35_P33_13_OUT,5000,100,ATOM}},
};

enc_devices_t ENC_Devices =
{
    .ENC[2] = {{ENC2_InPut_P00_7/*ENC2_InPut_P33_7*/, ENC2_Dir_P00_8/*ENC2_Dir_P33_6*/}},
    .ENC[3] = {{ENC3_InPut_P02_6, ENC3_Dir_P02_7}},
    .ENC[4] = {{ENC4_InPut_P02_8, ENC4_Dir_P00_9/*ENC4_Dir_P33_5*/}},
    .ENC[5] = {{ENC5_InPut_P10_3, ENC5_Dir_P10_1}},
    .ENC[6] = {{ENC6_InPut_P20_3/*ENC6_InPut_P10_2*/, ENC6_Dir_P20_0}},
};

tim_devices_t TIM_Devices =
{
    .SysTimeTIM = {STM0,STM_Channel_0,1000,true,true,STM},
};

uart_devices_t UART_Devices =
{
        .uart_1 = {UART1,UART1_RX_P11_10,UART1_TX_P11_12,115200},
};

spi_devices_t SPI_Devices =
{
        .spi_1 = {SPI2,QSPI2_CLK_P15_8,QSPI2_MISO_P15_7,QSPI2_MOSI_P15_5, QSPI2_CS_P15_2,3,QSPI,5000000},
};

