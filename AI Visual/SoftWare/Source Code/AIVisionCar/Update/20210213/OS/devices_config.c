/*
 * devices_config.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
 */
#include "devices_config.h"

beep_t BEEP = /*unknow*/
{
        .Init = BEEP_Init,
        .Self = &BEEP,
        .GPIODevice = &GPIO_Resources[11].GPION,
};

bluetooth_t Bluetooth = /*unknow*/
{
        .Init = Bluetooth_Init,
        .Self = &Bluetooth,
};

capture_t Capture = /*unknow*/
{
        .__Init__ = CameraInit,
        .__InitConfig__ = &fps,
        .__Start__ = CameraStart,
        .__Stop__ = CameraStop,
        .__GetState__ = CameraGetState,
        .__Read__ = CameraRead,
        .Init = Cap_Init,

        .ImageCache =
        {
                .Hight = 120,
                .Weight = 128,
                .Array = (uint8_t **)Image_Data
        },

};

communicate_t CUART = /*ok*/
{
        .Init = CommInit,
        .Self = &CUART,
        .CommunicationType = C_UART,
        .CommunicationDevice = (void *)&UART_Resources[0].UARTN,
};

esensor_t LESensor[5] =/*ok*/
{
    [0] = {
            .Init = ESensorInit,
            .Self = &LESensor[0],
            .ADCDevice = &ADC_Resources[7].ADCN,//AD9
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &LESensor[1],
            .ADCDevice = &ADC_Resources[8].ADCN,//AD10
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &LESensor[2],
            .ADCDevice = &ADC_Resources[9].ADCN,//AD11
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &LESensor[3],
            .ADCDevice = &ADC_Resources[10].ADCN,//AD12
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &LESensor[4],
            .ADCDevice = &ADC_Resources[11].ADCN,//AD13
    },
};
esensor_t SESensor[7] = /*ok*/
{
    [0] = {
            .Init = ESensorInit,
            .Self = &SESensor[0],
            .ADCDevice = &ADC_Resources[0].ADCN,//AD1
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &SESensor[1],
            .ADCDevice = &ADC_Resources[1].ADCN,//AD2
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &SESensor[2],
            .ADCDevice = &ADC_Resources[2].ADCN,//AD3
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &SESensor[3],
            .ADCDevice = &ADC_Resources[3].ADCN,//AD4
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &SESensor[4],
            .ADCDevice = &ADC_Resources[4].ADCN,//AD5
    },
    [5] = {
            .Init = ESensorInit,
            .Self = &SESensor[5],
            .ADCDevice = &ADC_Resources[5].ADCN,//AD6
    },
    [6] = {
            .Init = ESensorInit,
            .Self = &SESensor[6],
            .ADCDevice = &ADC_Resources[6].ADCN,//AD7
    },
};

key_t KEY[6] = /*ok*/
{
        [0] = {
                .Init = KEY_Init,
                .Self = &KEY[0],
                .GPIODevice = &GPIO_Resources[12].GPION,
        },
        [1] = {
                .Init = KEY_Init,
                .Self = &KEY[1],
                .GPIODevice = &GPIO_Resources[13].GPION,
        },
        [2] = {
                .Init = KEY_Init,
                .Self = &KEY[2],
                .GPIODevice = &GPIO_Resources[14].GPION,
        },
        [3] = {
                .Init = KEY_Init,
                .Self = &KEY[3],
                .GPIODevice = &GPIO_Resources[15].GPION,
        },

};


led_t GLED = /*ok*/
{
        .Init = LED_Init,
        .Self = &GLED,
        .GPIODevice = &GPIO_Resources[9].GPION,
};
led_t BLED =  /*ok*/
{
        .Init = LED_Init,
        .Self = &BLED,
        .GPIODevice = &GPIO_Resources[10].GPION,
};

motor_ctrl_t Motor =/*ok*/
{
        .Init = MotorInit,
        .Self = &Motor,
        .PwmDevice = {&PWM_Resources[0].PWMN,&PWM_Resources[1].PWMN},
        .EncDevice = &ENC_Resources[0].ENCN,
};

screen_t Screen = /*ok*/
{
        .Hight = 158,
        .Width = 128,
        .Font = {
                .Type = 1,
                .Hight = 8,
                .Width = 6,
                .Color = BLACK,
                .Backcolor = WHITE,
        },

        .__DeInit__ = LCD_DeInit,
        .__Init__ = LCD_Init,
        .__InitConfig__ = &LCD_Type,
        .__AddrReset__ = LCD_AddrReset,
        .__SetPixel__ =LCD_DrawPoint,

        .Init = Screen_Init,
        .Self = &Screen,
};

sdcard_t SDCard = /*待定*/
{
        .Init = SDCard_Init,

        .__Init__ = SD_Initialize,
        .__InitConfig__ = NULL,
        .__ReadSector__ = SD_ReadSector,
        .__WriteSector__ = SD_WriteSector,
        .__GetSectorNum__ = SD_GetSectorCount,

        .Self = &SDCard,
};

servo_ctrl_t Servo =/*ok*/
{
        .PwmDevice = &PWM_Resources[2].PWMN,
        .Init = ServoInit,
        .Self = &Servo,
};

switch_t Switch[4] = /*ok*/
{
    [0] = {
            .DownLevel = 0,
            .GPIODevice =  &GPIO_Resources[5].GPION,
            .Init = Switch_Init,
            .Self = &Switch[0],
    },
    [1] = {
            .DownLevel = 0,
            .GPIODevice =  &GPIO_Resources[6].GPION,
            .Init = Switch_Init,
            .Self = &Switch[1],
    },
    [2] = {
            .DownLevel = 0,
            .GPIODevice =  &GPIO_Resources[7].GPION,
            .Init = Switch_Init,
            .Self = &Switch[2],
    },
    [3] = {
            .DownLevel = 0,
            .GPIODevice =  &GPIO_Resources[8].GPION,
            .Init = Switch_Init,
            .Self = &Switch[3],
    },
};

dip_switch_t DIPSwitch = /*ok*/
{
       .Init = DIPSwitch_Init,
       .Self = &DIPSwitch,
       .Switch = Switch,
};

wireless_serial_t WirelessSerial = /*与CUART相同*/
{
        .Init = WirelessSerial_Init,
        .Self = &WirelessSerial
};


