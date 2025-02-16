/*
 * display_api.h
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */

#ifndef DRIVER_USRDRIVER_DISPLAY_API_H_
#define DRIVER_USRDRIVER_DISPLAY_API_H_

#include "driver.h"

#define SCREEN_MAX_BUF_LEN 32

volatile typedef enum
{
        RED     =0xf800,
        GREEN   =0x07e0,
        LIGHTBLUE=0x87ff,
        BLUE        =0x001f,
        PURPLE  =0xf81f,
        YELLOW  =0xffe0,
        CYAN        =0x07ff,        //蓝绿色
        ORANGE  =0xfc08,
        BLACK   =0x0000,
        WHITE   =0xffff,

}screen_color_t;

typedef struct
{
     uint16_t x;
     uint16_t y;
}screen_cursor_t;

typedef struct
{
        uint8_t Type; //字体，还未写代码
        uint16_t Hight;
        uint16_t Width;
        uint16_t Color;
        uint16_t Backcolor;
}screen_font_t;

typedef struct screen
{
    private

        uint16_t Hight;
        uint16_t Width;

        screen_font_t Font;
        screen_cursor_t Cursor;

        /*The following four functions are from screen(LCD/OLED) driver*/
        uint8_t (*HardWareInit)(void *config);
        void *HInitConfig;

        void (*HardWareDeInit)(void);
        void (*AddrReset)(void);
        void (*SetPixel)(uint16_t x,uint16_t y,uint16_t color);

    public

        uint8_t (*Init)(struct screen *self);
        void (*DeInit)(struct screen *self);
        void (*Clear)(struct screen *self,uint16_t color);
        void (*ShowChar)(struct screen *self,uint16_t row,uint16_t col,uint8_t ch,uint16_t color);
        void (*ShowNum)(struct screen *self,uint16_t row,uint16_t col,sint32_t num,uint16_t color);
        void (*ShowString)(struct screen *self,uint16_t row,uint16_t col,uint8_t *str,uint32_t len,uint16_t color);
        void (*ShowChinese)(void);
        void (*ShowStr)(struct screen *self,uint16_t row,uint16_t col,uint8_t *str,uint16_t color);

        void (*DrawHline)(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color);
        void (*DrawVline)(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color);
        void (*DrawLine)(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color);
        void (*DrawSqr)(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color);
        void (*DrawCircle)(struct screen *self,uint16_t x,uint16_t y,uint16_t r,uint16_t color);

        void (*WriteHalfLine)(struct screen *self,const sint8_t *format,...);
        void (*WriteLine)(struct screen *self,const sint8_t *format,...);
        void (*WriteXLine)(struct screen *self,uint16_t row,const sint8_t *format,...);

        void (*SetCursor)(struct screen *self,uint16_t x,uint16_t y);
        void (*SetFontSize)(struct screen *self,uint16_t width,uint16_t hight);
        void (*SetFontColor)(struct screen *self,uint16_t color);

        void (*Test)(struct screen *self);
        struct screen *Self;

}screen_t;

uint8_t Screen_Init(struct screen *self);

#endif /* DRIVER_USRDRIVER_DISPLAY_API_H_ */
