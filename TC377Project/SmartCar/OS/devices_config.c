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

imu_t IMU = 
{
        .Init = IMU_Init,
        .Self = &IMU,

        .__Init__ = ICM20602_Init,
        .__InitConfig__ = NULL,
        .__ReadAcc__  = ICM20602_ReadAcc,
        .__ReadGyro__ = ICM20602_ReadGyro,
        .__ReadMag__ = NULL,
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

esensor_t LESensor[MAX_LESENSOR_NUM] =/*ok*/ /*9 - 16 Left*/
{
    [0] = {
            .Init = ESensor_Init,
            .Self = &LESensor[0],
            .ADCn = &ADC_Resources[7].ADCN, //AD7
    },
    [1] = {
            .Init = ESensor_Init,
            .Self = &LESensor[1],
            .ADCn = &ADC_Resources[6].ADCN, //AD6
    },
    [2] = {
            .Init = ESensor_Init,
            .Self = &LESensor[2],
            .ADCn = &ADC_Resources[5].ADCN, //AD5
    },
    [3] = {
            .Init = ESensor_Init,
            .Self = &LESensor[3],
            .ADCn = &ADC_Resources[4].ADCN, //AD4
    },
    [4] = {
            .Init = ESensor_Init,
            .Self = &LESensor[4],
            .ADCn = &ADC_Resources[3].ADCN, //AD3
    },

    [5] = {
            .Init = ESensor_Init,
            .Self = &LESensor[5],
            .ADCn = &ADC_Resources[2].ADCN, //AD2
    },
    [6] = {
            .Init = ESensor_Init,
            .Self = &LESensor[6],
            .ADCn = &ADC_Resources[1].ADCN, //AD1
    },
};
esensor_t SESensor[MAX_SESENSOR_NUM] = /*ok*/  /*1 - 8 Right*/
{
    [0] = {
            .Init = ESensor_Init,
            .Self = &SESensor[0],
            .ADCn = &ADC_Resources[8].ADCN,//AD8
    },
    [1] = {
            .Init = ESensor_Init,
            .Self = &SESensor[1],
            .ADCn = &ADC_Resources[9].ADCN,//AD9
    },
    [2] = {
            .Init = ESensor_Init,
            .Self = &SESensor[2],
            .ADCn = &ADC_Resources[10].ADCN,//AD10
    },
    [3] = {
            .Init = ESensor_Init,
            .Self = &SESensor[3],
            .ADCn = &ADC_Resources[11].ADCN,//AD11
    },
    [4] = {
            .Init = ESensor_Init,
            .Self = &SESensor[4],
            .ADCn = &ADC_Resources[12].ADCN,//AD12
    },
    [5] = {
            .Init = ESensor_Init,
            .Self = &SESensor[5],
            .ADCn = &ADC_Resources[13].ADCN,//AD13
    },
    [6] = {
            .Init = ESensor_Init,
            .Self = &SESensor[6],
            .ADCn = &ADC_Resources[14].ADCN,//AD14
    },
    [7] = {
            .Init = ESensor_Init,
            .Self = &SESensor[7],
            .ADCn = &ADC_Resources[15].ADCN,//AD15
    },
};

motor_ctrl_t Motor =/*ok*/
{
        .Init = Motor_Init,
        .Self = &Motor,
        .Pwmn = {&PWM_Resources[1].PWMN,&PWM_Resources[0].PWMN},
        .Encn = &ENC_Resources[0].ENCN,
};

servo_ctrl_t Servo =/*ok*/
{
        .Pwmn = &PWM_Resources[2].PWMN,
        .Init = Servo_Init,
        .Self = &Servo,
};


#pragma section all restore


#pragma section all "cpu1_dsram"

screen_t Screen = /*ok*/
{
        .Hight = 160,
        .Width = 128,
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

memory_t SDCard = /*待定*/
{
        .Init = Mem_Init,

        .__Init__ = SD_Initialize,
        .__InitConfig__ = NULL,
        .__ReadSector__ = SD_ReadSector,
        .__WriteSector__ = SD_WriteSector,
        .__GetSectorNum__ = SD_GetSectorCount,
        .__SectorSize__ = 512,
        .__BlockSize__ = 4096,

        .Self = &SDCard,
};

memory_t EEPROM = /*待定*/
{
        .Init = Mem_Init,

        .__Init__ = NULL,
        .__InitConfig__ = NULL,
        .__ReadSector__ = EEPROM_ReadSector,
        .__WriteSector__ = EEPROM_WriteSector,
        .__GetSectorNum__ = EEPROM_GetSectorNum,
        .__SectorSize__ = 1024,
        .__BlockSize__ = 1024,

        .Self = &EEPROM,
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
        .Init = Com_Init,
        .Self = &CUART,
        .CommunicationType = C_UART,
        .Communicatorn = (void *)&UART_Resources[0].UARTN,
};

communicate_t DebugCom = /*ok*/
{
        .Init = Com_Init,
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

key_t KEY[6] = /*ok*/ /*按键接口换了*/
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

};

#pragma section all restore



