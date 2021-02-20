/*
 * devices_config.c
 *
 *  Created on: 2021Äê1ÔÂ22ÈÕ
 *      Author: 936305695
 */
#include "devices_config.h"

beep_t BEEP =
{
        .Init = BEEP_Init,
        .Self = &BEEP,
        .GPIODevice = &GPIO_Devices.BEEP_GPIO,
};

bluetooth_t Bluetooth =
{
        .Init = Bluetooth_Init,
        .Self = &Bluetooth,
};

capture_t Capture =
{
        .HardWareInit = Camera_Init,
        .Init = CAP_Init,
        .IMG_Hight = 120,
        .IMG_Weight = 128,
//        .IMG_Data = &Image_Data,
};

communicate_t CUART =
{
        .Init = CommInit,
        .Self = &CUART,
        .CommunicationType = C_UART,
        .CommunicationDevice = (void *)&UART_Devices.uart_1,
};
communicate_t CSPI =
{
        .Init = CommInit,
        .Self = &CSPI,
        .CommunicationType = C_SPI,
        .CommunicationDevice = (void *)&SPI_Devices.spi_1,
};

esensor_t LESensor[5] =
{
    [0] = {
            .Init = ESensorInit,
            .Self = &LESensor[0],
            .ADCDevice = &ADC_Devices.ADC[5],//AD9
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &LESensor[1],
            .ADCDevice = &ADC_Devices.ADC[7],//AD10
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &LESensor[2],
            .ADCDevice = &ADC_Devices.ADC[2],//AD11
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &LESensor[3],
            .ADCDevice = &ADC_Devices.ADC[4],//AD12
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &LESensor[4],
            .ADCDevice = &ADC_Devices.ADC[0],//AD13
    },
};
esensor_t SESensor[7] =
{
    [0] = {
            .Init = ESensorInit,
            .Self = &SESensor[0],
            .ADCDevice = &ADC_Devices.ADC[13],//AD1
    },
    [1] = {
            .Init = ESensorInit,
            .Self = &SESensor[1],
            .ADCDevice = &ADC_Devices.ADC[12],//AD2
    },
    [2] = {
            .Init = ESensorInit,
            .Self = &SESensor[2],
            .ADCDevice = &ADC_Devices.ADC[21],//AD3
    },
    [3] = {
            .Init = ESensorInit,
            .Self = &SESensor[3],
            .ADCDevice = &ADC_Devices.ADC[10],//AD4
    },
    [4] = {
            .Init = ESensorInit,
            .Self = &SESensor[4],
            .ADCDevice = &ADC_Devices.ADC[11],//AD5
    },
    [5] = {
            .Init = ESensorInit,
            .Self = &SESensor[3],
            .ADCDevice = &ADC_Devices.ADC[6],//AD6
    },
    [6] = {
            .Init = ESensorInit,
            .Self = &SESensor[4],
            .ADCDevice = &ADC_Devices.ADC[3],//AD7
    },
};

key_t KEY[4] = /*ok*/
{
        [0] = {
                .Init = KEY_Init,
                .KeyShield = 0,
                .DownLevel = 0,
                .Self = &KEY[0],
                .GPIODevice = &GPIO_Devices.KEY1_GPIO,
        },
        [1] = {
                .Init = KEY_Init,
                .KeyShield = 0,
                .DownLevel = 0,
                .Self = &KEY[1],
                .GPIODevice = &GPIO_Devices.KEY2_GPIO,
        },
        [2] = {
                .Init = KEY_Init,
                .KeyShield = 0,
                .DownLevel = 0,
                .Self = &KEY[2],
                .GPIODevice = &GPIO_Devices.KEY3_GPIO,
        },
        [3] = {
                .Init = KEY_Init,
                .KeyShield = 0,
                .DownLevel = 0,
                .Self = &KEY[3],
                .GPIODevice = &GPIO_Devices.KEY4_GPIO,
        },

};


led_t GLED = /*ok*/
{
        .Init = LED_Init,
        .Self = &GLED,
        .GPIODevice = &GPIO_Devices.GLED_GPIO,
};
led_t BLED =  /*ok*/
{
        .Init = LED_Init,
        .Self = &BLED,
        .GPIODevice = &GPIO_Devices.BLED_GPIO,
};

motor_ctrl_t Motor =/*ok*/
{
        .Init = MotorInit,
        .Self = &Motor,
        .UsrData = NULL,
        .PwmDevice = {&PWM_Devices.ATOMPwm[8],&PWM_Devices.ATOMPwm[7]},
        .EncDevice = &ENC_Devices.ENC[6],
};

screen_t Screen =
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

        .HardWareDeInit = LCD_DeInit,
        .HardWareInit = LCD_Init,
        .AddrReset = LCD_AddrReset,
        .SetPixel =LCD_DrawPoint,

        .Init = Screen_Init,
        .Self = &Screen,
};

sdcard_t SDCard =
{
        .Init = SDCard_Init,
        //.HardWareInit = NULL,
        //.ReadSector =
        //.WriteSector =
        .Self = &SDCard,
        .CardSectorSize = 512,
        .CardBlockSize = 1024,
        .CardCapacity = 2048,
};

servo_ctrl_t Servo =/*ok*/
{
        .PwmDevice = &PWM_Devices.ATOMPwm[3],
        .Init = ServoInit,
        .Self = &Servo,
};

switch_t Switch[4] =
{
    [0] = {
            .DownLevel = 0,
            .GPIODevice =  NULL,
            .Init = Switch_Init,
            .Self = &Switch[0],
    },
    [1] = {
            .DownLevel = 0,
            .GPIODevice =  NULL,
            .Init = Switch_Init,
            .Self = &Switch[1],
    },
    [2] = {
            .DownLevel = 0,
            .GPIODevice =  NULL,
            .Init = Switch_Init,
            .Self = &Switch[2],
    },
    [3] = {
            .DownLevel = 0,
            .GPIODevice =  NULL,
            .Init = Switch_Init,
            .Self = &Switch[3],
    },
};

dip_switch_t DIPSwitch =
{
       .Bits = 0,
       .SwitchNum = 4,
       .Init = DIPSwitch_Init,
       .Self = &DIPSwitch,
       .Switch = Switch,
};

wireless_serial_t WirelessSerial =
{
        .Init = WirelessSerial_Init,
        .Self = &WirelessSerial
};


