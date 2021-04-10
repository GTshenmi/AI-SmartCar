/*
 * lcd_driver.h
 *
 *  Created on: 2020年12月6日
 *      Author: GT_shenmi
 */

#ifndef DRIVER_USRDRIVER_LCD_LCD_DRIVER_H_
#define DRIVER_USRDRIVER_LCD_LCD_DRIVER_H_

#include "sys_driver.h"

#define USING_HARDWARE_CONTROLER false

/*                      !Delay!                         */
#define LCD_delayus(us) Systime.Delayus(us);            //Delay function interface->delay x us
#define LCD_delayms(ms) Systime.Delayms(ms);            //Delay function interface->delay x ms
/*                      !Delay!                         */

/*                      !SPI Control!                           */
#if !USING_HARDWARE_CONTROLER

#define LCD_WriteByte   LCD_SoftWareWriteByte //send byte function interface->LCD_SoftWareWriteByte
#define LCD_WriteWord   LCD_SoftWareWriteWord //send word function interface->LCD_SoftWareWriteWord
#define LCD_WriteCmd    LCD_SoftWareWriteCmd    //send command function interface->LCD_SoftWareWriteCmd
#define _GPIO_WritePin  GPIOx.Write

#define LCD_DC_GPIO     &GPIO_Resources[0].GPION
#define LCD_RST_GPIO    &GPIO_Resources[1].GPION
#define LCD_SDA_GPIO    &GPIO_Resources[2].GPION
#define LCD_SCL_GPIO    &GPIO_Resources[3].GPION
#define LCD_CS_GPIO     &GPIO_Resources[4].GPION

#define LCD_DC(x)   ((x==1)?_GPIO_WritePin(LCD_DC_GPIO,1):_GPIO_WritePin(LCD_DC_GPIO,0))
#define LCD_RST(x)  ((x==1)?_GPIO_WritePin(LCD_RST_GPIO,1):_GPIO_WritePin(LCD_RST_GPIO,0))
#define LCD_SDA(x)  ((x==1)?_GPIO_WritePin(LCD_SDA_GPIO,1):_GPIO_WritePin(LCD_SDA_GPIO,0))
#define LCD_SCL(x)  ((x==1)?_GPIO_WritePin(LCD_SCL_GPIO,1):_GPIO_WritePin(LCD_SCL_GPIO,0))
#define LCD_CS(x)   ((x==1)?_GPIO_WritePin(LCD_CS_GPIO,1):_GPIO_WritePin(LCD_CS_GPIO,0))

#else

#define LCD_SPI         &SPI_Resources[0].SPIN
#define LCD_WriteByte   LCD_HardWareWriteByte //send byte function interface->LCD_SoftWareWriteByte
#define LCD_WriteWord   LCD_HardWareWriteWord //send word function interface->LCD_SoftWareWriteWord
#define LCD_WriteCmd    LCD_HardWareWriteCmd  //send command function interface->LCD_SoftWareWriteCmd

#define _GPIO_WritePin  GPIOx.Write

#define LCD_DC_GPIO     &GPIO_Resources[0].GPION
#define LCD_RST_GPIO    &GPIO_Resources[1].GPION
#define LCD_CS_GPIO     &GPIO_Resources[4].GPION

#define LCD_DC(x)   ((x==1)?_GPIO_WritePin(LCD_DC_GPIO,1):_GPIO_WritePin(LCD_DC_GPIO,0))
#define LCD_RST(x)  ((x==1)?_GPIO_WritePin(LCD_RST_GPIO,1):_GPIO_WritePin(LCD_RST_GPIO,0))
#define LCD_CS(x)   ((x==1)?_GPIO_WritePin(LCD_CS_GPIO,1):_GPIO_WritePin(LCD_CS_GPIO,0))

#endif
/*                      !SPI Control!                           */

#if !USING_HARDWARE_CONTROLER
void LCD_SoftWareWriteByte(uint8_t data);
void LCD_SoftWareWriteWord(uint16_t data);
void LCD_SoftWareWriteCmd(uint8_t cmd);
#else
void LCD_HardWareSPIInit(uint32_t baudrate);
void LCD_HardWareWriteByte(uint8_t data);
void LCD_HardWareWriteWord(uint16_t data);
void LCD_HardWareWriteCmd(uint8_t cmd);
#endif


/*Public Function.*/
extern uint8_t LCD_Type;
uint8_t LCD_Init(void *config);
void LCD_DeInit(void);
void LCD_AddrReset(void);
void LCD_DrawPoint(uint16_t xs,uint16_t ys,uint16_t color);
void LCD_SetEnable(bool enable);
void LCD_SetArea(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye);
void LCD_FastDrawArea(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t **array);
void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color);
/*Public Function.*/

/*                                                                                              字体库                                                                                                 */
extern uint8_t Font_code8[][6];
/*                                                                                              字体库                                                                                                 */

#endif /* DRIVER_USRDRIVER_LCD_LCD_DRIVER_H_ */
