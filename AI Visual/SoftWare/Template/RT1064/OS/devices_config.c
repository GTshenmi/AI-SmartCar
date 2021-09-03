/*
 * devices_config.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
 */
#include "devices_config.h"
#include "driver.h"

esensor_t ESensor[4] = {
        [0] = {
                .Init = ESensorInit,
                .Self = &ESensor[0],
                .ADCn = &ADC_Resources[0].ADCN,
        },
        [1] = {
                .Init = ESensorInit,
                .Self = &ESensor[1],
                .ADCn = &ADC_Resources[1].ADCN,
        },
        [2] = {
                .Init = ESensorInit,
                .Self = &ESensor[2],
                .ADCn = &ADC_Resources[2].ADCN,
        },
        [3] = {
                .Init = ESensorInit,
                .Self = &ESensor[3],
                .ADCn = &ADC_Resources[3].ADCN,
        },
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
        
        .Self = &Capture,

        .ImageCache =
        {
              [0] = 
              {
                 .Hight = 120,
                 .Width = 188,
                 .Array = &csiFrameBuf[0][0],
              },
              [1] = 
              {
                 .Hight = 120,
                 .Width = 188,
                 .Array = &csiFrameBuf[0][1],
              },
              [2] = 
              {
                 .Hight = 120,
                 .Width = 188,
                 .Array = &csiFrameBuf[0][2],
              },
              [3] = 
              {
                 .Hight = 120,
                 .Width = 188,
                 .Array = &csiFrameBuf[0][3],
              },
        },

};

communicate_t DebugCom = 
{
        .Init = ComInit,
        .Self = &DebugCom,
        .CommunicationType = C_UART,
        .Communicatorn = (void *)&UART_Resources[0].UARTN,  
};

communicate_t CUART = /*ok*/
{
        .Init = ComInit,
        .Self = &CUART,
        .CommunicationType = C_UART,
        .Communicatorn = (void *)&UART_Resources[0].UARTN,
};

key_t KEY[6] = /*ok*/
{
        [0] = {
                .Init = KEY_Init,
                .Self = &KEY[0],
                .GPIOn = &GPIO_Resources[6].GPION,
        },
        [1] = {
                .Init = KEY_Init,
                .Self = &KEY[1],
                .GPIOn = &GPIO_Resources[7].GPION,
        },
        [2] = {
                .Init = KEY_Init,
                .Self = &KEY[2],
                .GPIOn = &GPIO_Resources[8].GPION,
        },
        [3] = {
                .Init = KEY_Init,
                .Self = &KEY[3],
                .GPIOn = &GPIO_Resources[9].GPION,
        },
        
        [4] = {
                .Init = KEY_Init,
                .Self = &KEY[4],
                .GPIOn = &GPIO_Resources[10].GPION,
        },
        [5] = {
                .Init = KEY_Init,
                .Self = &KEY[5],
                .GPIOn = &GPIO_Resources[11].GPION,
        },        
        

};


led_t RLED = /*ok*/
{
        .Init = LED_Init,
        .Self = &RLED,
        .GPIOn = &GPIO_Resources[5].GPION,
};

motor_ctrl_t LMotor =/*ok*/
{
        .Init = MotorInit,
        .Self = &LMotor,
        .Pwmn = {&PWM_Resources[2].PWMN,&PWM_Resources[3].PWMN},
        .Encn = &ENC_Resources[1].ENCN,
};
motor_ctrl_t RMotor =/*ok*/
{
        .Init = MotorInit,
        .Self = &RMotor,
        .Pwmn = {&PWM_Resources[0].PWMN,&PWM_Resources[1].PWMN},
        .Encn = &ENC_Resources[0].ENCN,
};


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

        .__DeInit__ = LCD_DeInit,
        .__Init__ = LCD_Init,
        .__InitConfig__ = &LCD_Type,
        .__AddrReset__ = LCD_AddrReset,
        .__SetPixel__ =LCD_DrawPoint,
        
        .__Fill__ = LCD_Fill,
        .__SetEnable__ = LCD_SetEnable,
        .__FastDrawArea__= LCD_FastDrawArea,

        .__FastSetPixel__= LCD_WriteWord,
        .__SetArea__= LCD_SetArea,
        
//        .__Fill__ = NULL,
//        .__SetEnable__ = NULL,
//        .__FastDrawArea__= NULL,
//        .__Fill__= NULL,
//
//        .__FastSetPixel__= NULL,
//        .__SetArea__= NULL,

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

servo_ctrl_t Servo1 =/*ok*/
{
        .Pwmn = &PWM_Resources[4].PWMN,
        .Init = ServoInit,
        .Self = &Servo1,
};

servo_ctrl_t Servo2 =/*ok*/
{
        .Pwmn = &PWM_Resources[5].PWMN,
        .Init = ServoInit,
        .Self = &Servo2,
};


