/*
 * devices_config.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
 */
#include "devices_config.h"
#include "driver.h"

#pragma section all "cpu0_dsram"

led_t GLED = /*ok*/
{
        .Init = LED_Init,
        .Self = &GLED,
        .GPIOn = &GPIO_Resources[9].GPION,
};

beep_t BEEP = /*unknow*/
{
        .Init = BEEP_Init,
        .Self = &BEEP,
        .GPIOn = &GPIO_Resources[11].GPION,
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
        .Init = Capture_Init,

        .ImageCache =
        {
                .Hight = 188,
                .Width = 120,
#if ((defined(Chip)) && ((Chip == TC264) || (Chip == TC377)))
                //.Array = (uint8_t **)Image_Data,
#endif
        },

};

esensor_t LESensor[5] =/*ok*/ /*9 - 16 Left*/
{
    [0] = {
            .Init = ESensorInit,
            .Self = &LESensor[0],
            .ADCn = &ADC_Resources[8].ADCN,
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &LESensor[1],
            .ADCn = &ADC_Resources[9].ADCN,
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &LESensor[2],
            .ADCn = &ADC_Resources[10].ADCN,
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &LESensor[3],
            .ADCn = &ADC_Resources[11].ADCN,
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &LESensor[4],
            .ADCn = &ADC_Resources[12].ADCN,
    },
};
esensor_t SESensor[7] = /*ok*/  /*1 - 8 Right*/
{
    [0] = {
            .Init = ESensorInit,
            .Self = &SESensor[0],
            .ADCn = &ADC_Resources[0].ADCN,//AD1
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &SESensor[1],
            .ADCn = &ADC_Resources[1].ADCN,//AD2
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &SESensor[2],
            .ADCn = &ADC_Resources[2].ADCN,//AD3
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &SESensor[3],
            .ADCn = &ADC_Resources[3].ADCN,//AD4
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &SESensor[4],
            .ADCn = &ADC_Resources[4].ADCN,//AD5
    },
    [5] = {
            .Init = ESensorInit,
            .Self = &SESensor[5],
            .ADCn = &ADC_Resources[7].ADCN,//AD6
    },
    [6] = {
            .Init = ESensorInit,
            .Self = &SESensor[6],
            .ADCn = &ADC_Resources[6].ADCN,//AD7
    },
};

motor_ctrl_t Motor =/*ok*/
{
        .Init = MotorInit,
        .Self = &Motor,
        .Pwmn = {&PWM_Resources[1].PWMN,&PWM_Resources[0].PWMN},
        .Encn = &ENC_Resources[0].ENCN,
};

servo_ctrl_t Servo =/*ok*/
{
        .Pwmn = &PWM_Resources[2].PWMN,
        .Init = ServoInit,
        .Self = &Servo,
};


#pragma section all restore


#pragma section all "cpu1_dsram"

screen_t Screen = /*ok*/
{
        .Hight = 162,
        .Width = 130,
        .Font = {
                .Type = 1,
                .Hight = 8,
                .Width = 6,
                .Color = BLACK,
                .Backcolor = WHITE,
        },

        .__Init__ = LCD_Init,
        .__InitConfig__ = &LCD_Type,
        .__SetPixel__ =LCD_DrawPoint,

        .__AddrReset__ = LCD_AddrReset,
        .__DeInit__ = LCD_DeInit,
        .__SetEnable__ = LCD_SetEnable,
        .__FastDrawArea__ = LCD_FastDrawArea,
        .__FastSetPixel__ = LCD_WriteWord,
        .__Fill__ = LCD_Fill,
        .__SetArea__ = LCD_SetArea,


        .Init = Screen_Init,
        .Self = &Screen,
};

led_t BLED =  /*ok*/
{
        .Init = LED_Init,
        .Self = &BLED,
        .GPIOn = &GPIO_Resources[10].GPION,
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


switch_t Switch[8] = /*ok*/
{
    [0] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[5].GPION,
            .Init = Switch_Init,
            .Self = &Switch[0],
    },
    [1] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[6].GPION,
            .Init = Switch_Init,
            .Self = &Switch[1],
    },
    [2] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[7].GPION,
            .Init = Switch_Init,
            .Self = &Switch[2],
    },
    [3] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[8].GPION,
            .Init = Switch_Init,
            .Self = &Switch[3],
    },

    [4] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[18].GPION,
            .Init = Switch_Init,
            .Self = &Switch[4],
    },
    [5] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[19].GPION,
            .Init = Switch_Init,
            .Self = &Switch[5],
    },
    [6] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[20].GPION,
            .Init = Switch_Init,
            .Self = &Switch[6],
    },
    [7] = {
            .DownLevel = 0,
            .GPIOn =  &GPIO_Resources[21].GPION,
            .Init = Switch_Init,
            .Self = &Switch[7],
    },
};

communicate_t CUART = /*ok*/
{
        .Init = ComInit,
        .Self = &CUART,
        .CommunicationType = C_UART,
        .Communicatorn = (void *)&UART_Resources[0].UARTN,
};

communicate_t DebugCom = /*ok*/
{
        .Init = ComInit,
        .Self = &DebugCom,
        .CommunicationType = C_UART,
        .Communicatorn = (void *)&UART_Resources[0].UARTN,
};

dip_switch_t DIPSwitch = /*ok*/
{
       .Init = DIPSwitch_Init,
       .Self = &DIPSwitch,
       .Switch = Switch,
       .SwitchNum = 8,
};

wireless_serial_t WirelessSerial = /*与CUART相同*/
{
        .Init = WirelessSerial_Init,
        .Self = &WirelessSerial
};


#pragma section all restore


#pragma section all "cpu2_dsram"

key_t KEY[7] = /*ok*/ /*按键接口换了*/
{
        [0] = {
                .Init = KEY_Init,
                .Self = &KEY[0],
                .GPIOn = &GPIO_Resources[12].GPION,
        },
        [1] = {
                .Init = KEY_Init,
                .Self = &KEY[1],
                .GPIOn = &GPIO_Resources[13].GPION,
        },
        [2] = {
                .Init = KEY_Init,
                .Self = &KEY[2],
                .GPIOn = &GPIO_Resources[14].GPION,
        },
        [3] = {
                .Init = KEY_Init,
                .Self = &KEY[3],
                .GPIOn = &GPIO_Resources[15].GPION,
        },

        [4] = {
                .Init = KEY_Init,
                .Self = &KEY[4],
                .GPIOn = &GPIO_Resources[16].GPION,
        },

        [5] = {
                .Init = KEY_Init,
                .Self = &KEY[5],
                .GPIOn = &GPIO_Resources[17].GPION,
        },

        [6] = {
                .Init = KEY_Init,
                .Self = &KEY[6],
                .GPIOn = &GPIO_Resources[12].GPION,
        },

};

#pragma section all restore



