/*
 * Screen.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
/*                          LCD Control Structure                                           */
#include <screen.h>
#include "driver.h"

void Screen_SetPixel(struct screen *self,uint16_t x,uint16_t y,uint16_t color)
{
#if defined(Chip) && Chip == TC377 || Chip == TC264
    while(!Screen_AcquireMutex(&self->Is_Busy));

    Screen_LockMutex(&self->Is_Busy);
#endif

    if(x >= self->Width)
        x = 0;
    if(y >= self->Hight)
        y = 0;

    self->__SetPixel__(x,y,color);

#if defined(Chip) && Chip == TC377 || Chip == TC264
    Screen_ReleaseMutex(&self->Is_Busy);
#endif
}
void Screen_DeInit(struct screen *self)
{
    self->__DeInit__();
}
/**
    *
    * @brief  画水平线
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    len：线的长度
    * @param    width：线的宽度
    * @param    color:线条颜色
    * @retval 无
    *
  */
void Screen_DrawHLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//水平线
{
    uint16_t i,j;

    for(i = xs; i <( len + xs );i++)      //160
    {
       for(j= ys ;j <( ys + width );j++)
       {
           self->SetPixel(self,i,j,color);
       }
    }
}
/**
    *
    * @brief  画垂线
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    len：线的长度
    * @param    width：线的宽度
    * @param    color:线条颜色
    * @retval 无
    *
  */
void Screen_DrawVLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//垂线
{
    uint16_t i,j;

    for(j=ys;j < (len + ys);j++)      //160
    {
        for(i=xs; i < (width + xs) ;i++)
        {
            self->SetPixel(self,i,j,color);
        }
    }
}
/**
    *
    * @brief  画线
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    xe：结束x坐标
    * @param    ye：结束y坐标
    * @param    color:线条颜色
    * @retval 无
    *
  */
void Screen_DrawLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    uint16_t x;
    uint16_t k;
    if(xs!=xe)
    {
        k=(ye-ys)/(xe-xs);
        for(x=xs;x<=xe;x++)
        {
             self->SetPixel(self,x,k*(x-xs)+ys,color);
        }
    }
    else
    {
        for(x=ys;x<=ye;x++)
        {
            self->SetPixel(self,xs,x,color);
        }
    }
}
/**
    *
    * @brief  画矩形
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    xe：结束x坐标
    * @param    ye：结束y坐标
    * @param    color:线条颜色
    * @retval 无
    *
  */
void Screen_DrawSqr(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    self->DrawLine(self,xs,ys,xe,ys,color);
    self->DrawLine(self,xs,ys,xs,ye,color);
    self->DrawLine(self,xs,ye,xe,ye,color);
    self->DrawLine(self,xe,ys,xe,ye,color);
}
/**
    *
    * @brief  画圆
    * @param    x：圆心x坐标
    * @param    y：圆心y坐标
    * @param    r：圆半径
    * @param    color:线条颜色
    * @retval 无
    *
  */
void Screen_DrawCircle(struct screen *self,uint16_t x,uint16_t y,uint16_t r,uint16_t color)
{
    uint16_t dx, dy = r;

    if((x>=r) &&((self->Hight-x)>=r) && (y>=r) && ((self->Width-y)>=r))       //确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
    {
        for(dx = 0; dx <= r; dx++)
        {
            while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
            self->SetPixel(self,x + dx, y - dy, color);
            self->SetPixel(self,x - dx, y - dy, color);
            self->SetPixel(self,x - dx, y + dy, color);
            self->SetPixel(self,x + dx, y + dy, color);

            self->SetPixel(self,x + dy, y - dx, color);
            self->SetPixel(self,x - dy, y - dx, color);
            self->SetPixel(self,x - dy, y + dx, color);
            self->SetPixel(self,x + dy, y + dx, color);
        }
    }
}
/**
    *
    * @brief    清屏
    * @param    color:背景颜色
    * @retval 无
    *
  */
void Screen_Clear(struct screen *self,uint16_t color)
{
    uint16_t i,j;

    for(i=0;i<self->Width;i++)      //160
    {
        for(j=0;j<self->Hight;j++)
        {
            self->SetPixel(self,i,j,color);
        }
    }
}
/**
    *
    * @brief    范围填充
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    xe：结束x坐标
    * @param    ye：结束y坐标
    * @param    color:填充颜色
    * @retval 无
    *
  */
void Screen_Fill(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    uint16_t i,j;
    for(i=xs;i<xe;i++)
        for(j=ys;j<=ye;j++)
            self->SetPixel(self,i,j,color);
}
/**
    *
    * @brief  显示字符
    * @param    x：      x坐标
    * @param    y：      y坐标
    * @param    ch：         要显示的字符
    * @param    color：  字符颜色
    * @retval 无
    *
  */
void Screen_ShowChar(struct screen *self,uint16_t x,uint16_t y,uint8_t ch,uint16_t color)
{
    uint16_t i,j;

    if(x>self->Width)
       y+=self->Font.Hight,x=0;

    for(j=0; j< self->Font.Hight; j++)
    {
        for(i=0; i< self->Font.Width; i++)
        {
          if((Font_code8[ch-32][i]) & (0x01<<j))
              self->SetPixel(self,x+i,y+j,color);
          else
              self->SetPixel(self,x+i,y+j,self->Font.Backcolor);
        }
    }
}
/**
    *
    * @brief  显示字符
    * @param    x：  x坐标
    * @param    y：  y坐标
    * @param    num：     要显示的数字
    * @param    color：  字符颜色
    * @retval 无
    *
  */
void Screen_ShowNum(struct screen *self,uint16_t x,uint16_t y,sint32_t num,uint16_t color)
{
    unsigned int len=0;
    long temp=num;
    unsigned int i=0;

    if(num<0)
    {
        self->ShowChar(self,x,y,'-',color);
        num=-num;
        x += self->Font.Width;
    }

    if(num==0)
        self->ShowChar(self,x,y,'0',color);
    else
    {
        while(temp)
        {
            len++;
            temp/=10;
        }

        for(i=0;i<len;i++)
        {
            self->ShowChar(self,x + (len-i-1)*self->Font.Width,y,(num%10+'0'),color);
            num/=10;
        }
    }
}
/**
    *
    * @brief  显示汉字
    * @param    无
    * @retval 无
    *
  */
void Screen_ShowChinese(struct screen *self,uint16_t x,uint16_t y,uint16_t Chinese,uint16_t color)
{
    ;
}

/**
    *
    * @brief  显示字符串
    * @param    x：  x坐标
    * @param    y：  y坐标
    * @param    string：字符串
    * @param    color：  字符颜色
    * @retval 无
    *
  */
void Screen_ShowString(struct screen *self,uint16_t x,uint16_t y,uint8_t *string,uint16_t color)
{
    uint32_t len=0;
    uint8_t *p=string;

    while(*p!='\0')
    {
        self->ShowChar(self,(uint16_t)(x + len*self->Font.Width),(uint16_t)(y),*p,color);
        p++;
        len++;
    }
}
/**
    *
    * @brief  显示字符串
    * @param    x：  x坐标
    * @param    y：  y坐标
    * @param    string：字符串
    * @param    len：     字符串长度
    * @param    color：  字符颜色
    * @retval 无
    *
  */
void Screen_ShowStr(struct screen *self,uint16_t x,uint16_t y,uint8_t *str,uint32_t len,uint16_t color)
{
    uint32_t i;

    for(i=0;i<len;i++)
    {
        self->ShowChar(self,(uint16_t)(x + i*self->Font.Width),(uint16_t)(y),str[i],color);
    }
}

/**
    *
    * @brief  从某个坐标开始写一串格式化字符
    * @param    x：  x坐标
    * @param    y：  y坐标
    * @param  format： 格式
    * @param  ...：        可变参数
    * @retval 无
    * @attention 用法同printf
    *
  */
void Screen_Write(struct screen *self,uint16_t x,uint16_t y,const sint8_t *format,...)
{
    va_list(arg);
    va_start(arg,format);

    self->Cursor.x = x;
    self->Cursor.y = y;

    char writebuf[SCREEN_MAX_BUF_LEN];

    uint32_t len = vsprintf(writebuf,format,arg);

    self->ShowStr(self,(uint16_t)x,(uint16_t)y,(uint8_t *)writebuf,len,self->Font.Color);

    va_end(arg);
}

/**
    *
    * @brief  写一行
    * @param    format： 格式
    * @param    ...：        可变参数
    * @retval 无
    * @attention 用法同printf，会自动换行
    *
  */
void Screen_WriteLine(struct screen *self,const sint8_t *format,...)
{

    va_list(arg);
    va_start(arg,format);

    char writebuf[SCREEN_MAX_BUF_LEN];

    uint32_t len = vsprintf(writebuf,format,arg);

    self->ShowStr(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint8_t *)writebuf,len,self->Font.Color);

    self->Cursor.x=0;
    self->Cursor.y+=self->Font.Hight;

    va_end(arg);

}
/**
    *
    * @brief  写第X行
    * @param    format： 格式
    * @param    ...：        可变参数
    * @retval 无
    * @attention 用法同printf，从最左侧开始，固定写第row行,不支持自动换行，其中 y = row * self.Font.Hight
    * @          row = 1 实际的y坐标就是self.Font.Hight
    * @                  此函数中x坐标始终为0
    *
  */
void Screen_WriteXLine(struct screen *self,uint16_t row,const sint8_t *format,...)
{
    va_list(arg);
    va_start(arg,format);

    uint16_t x = 0;
    uint16_t y = row*(self->Font.Hight);

    self->Cursor.x = x;
    self->Cursor.y= y;

    char writebuf[SCREEN_MAX_BUF_LEN];

    uint32_t len = vsprintf(writebuf,format,arg);

    self->ShowStr(self,(uint16_t)x,(uint16_t)y,(uint8_t *)writebuf,len,self->Font.Color);

    self->Cursor.y+=self->Font.Hight;
    self->Cursor.x=0;

    va_end(arg);
}

/**
    *
    * @brief    设置Screen光标
    * @param    x：光标x坐标
    * @param    y：光标y坐标
    * @retval 无
    * @attention： 仅用于Screen_WriteLine函数，其他函数调用前设置光标不起作用
    *
  */
void Screen_SetCursor(struct screen *self,uint16_t x,uint16_t y)
{
    self->Cursor.x=x;
    self->Cursor.y=y;
}

/**
    *
    * @brief    设置显示的字体大小
    * @param    width：字体的宽度
    * @param    hight：字体的高度
  * @retval 无
    *
  */
void Screen_SetFontSize(struct screen *self,uint16_t width,uint16_t hight)
{
    self->Font.Hight=hight;
    self->Font.Width=width;
}
/**
    *
    * @brief    设置显示的字体颜色
    * @param    color:字体颜色
  * @retval 无
    *   @attention： 仅用于Printf 函数及 write函数
    *
  */
void Screen_SetFontColor(struct screen *self,uint16_t color)
{
    self->Font.Color=color;
}

/**
    *
    * @brief  LCD测试
    * @param    无
  * @retval 无
    *
  */
void Screen_Test(struct screen *self)
{
    self->Init(self);
    self->Clear(self,WHITE);

    self->ShowNum(self,0,0,1,BLACK);
    self->ShowString(self,self->Font.Width,0,(uint8_t *)"#include",YELLOW);
    self->ShowString(self,9*self->Font.Width,0,(uint8_t *)"<stdio.h>",PURPLE);

    self->ShowNum(self,0,self->Font.Hight,2,BLACK);
    self->ShowString(self,self->Font.Width,self->Font.Hight,(uint8_t *)"int",RED);
    self->ShowString(self,5*self->Font.Width,self->Font.Hight,(uint8_t *)"main()",BLACK);

    self->ShowNum(self,0,2*self->Font.Hight,3,BLACK);
    self->ShowChar(self,1*self->Font.Width,2*self->Font.Hight,'{',RED);

    self->ShowNum(self,0,3*self->Font.Hight,4,BLACK);
    self->ShowString(self,2*self->Font.Width,3*self->Font.Hight,(uint8_t *)"Printf(",BLACK);
    self->ShowChar(self,9*self->Font.Width,3*self->Font.Hight,'"',PURPLE);
    self->ShowString(self,10*self->Font.Width,3*self->Font.Hight,(uint8_t *)"Hello,World!",PURPLE);
    self->ShowChar(self,21*self->Font.Width,3*self->Font.Hight,'"',PURPLE);
    self->ShowString(self,22*self->Font.Width,3*self->Font.Hight,(uint8_t *)");",BLACK);

    self->WriteXLine(self->Self,4,"5 return");
    self->ShowNum(self,9*self->Font.Width,4*self->Font.Hight,0,BLUE);
    self->ShowChar(self,10*self->Font.Width,4*self->Font.Hight,';',BLACK);

    self->ShowNum(self,0,5*self->Font.Hight,6,BLACK);
    self->ShowChar(self,1*self->Font.Width,self->Font.Hight*5,'}',RED);

    self->DrawHline(self,0,0,(uint16_t)self->Width,2,BLUE);
    self->DrawHline(self,0,(uint16_t)(self->Hight/2-10),(uint16_t)self->Width,2,BLUE);
    self->DrawVline(self,self->Width/2,self->Hight/2,self->Hight/2-self->Font.Hight,1,PURPLE);

    self->DrawSqr(self,1,self->Hight/2,self->Width-1,self->Hight-5,YELLOW);

    self->SetFontColor(self,GREEN);
    self->SetCursor(self,10*self->Font.Width,11*self->Font.Hight);
    self->WriteLine(self,"//%d",-1234);

    self->SetCursor(self,10*self->Font.Width,13*self->Font.Hight);
    self->WriteLine(self,"//%.3f",1.23);

    self->WriteLine(self,"\n//Dog Rui");
    self->Write(self,10*self->Font.Width,15*self->Font.Hight,"//Dog Rui");

    self->DrawCircle(self,30,95,10,BLUE);

    while(1);
}

uint8_t Screen_Init(struct screen *self)
{
    self->DeInit = Screen_DeInit;
    self->Clear = Screen_Clear;
    self->ShowChar = Screen_ShowChar;
    self->ShowNum = Screen_ShowNum;
    self->ShowChinese = Screen_ShowChinese;
    self->ShowStr = Screen_ShowStr;

    self->DrawHline = Screen_DrawHLine;
    self->DrawVline = Screen_DrawVLine;
    self->DrawLine = Screen_DrawLine;
    self->DrawSqr = Screen_DrawSqr;
    self->DrawCircle = Screen_DrawCircle;

    self->Write = Screen_Write;
    self->WriteLine = Screen_WriteLine;
    self->WriteXLine =Screen_WriteXLine;

    self->SetCursor = Screen_SetCursor;
    self->SetFontColor = Screen_SetFontColor;
    self->SetFontSize = Screen_SetFontSize;

    self->Test = Screen_Test;
    self->Cursor.x = 0;
    self->Cursor.y = 0;
    self->ShowString = Screen_ShowString;

    self->Fill = Screen_Fill;

    self->Is_Busy = false;

#if defined(Chip) && Chip == TC377 || Chip == TC264
    Screen_ReleaseMutex(&self->Is_Busy);
#endif

    self->SetPixel = Screen_SetPixel;

    uint8_t res = 0;
    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);

    self->Clear(self,WHITE);

    return res;
}

