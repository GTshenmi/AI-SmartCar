/*
 * Screen.c
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */
/*                          LCD Control Structure                                           */
#include <screen.h>
#include "driver.h"

void Screen_SetPixel(struct screen *self,uint16_t x,uint16_t y,uint16_t color)
{
    if(x >= self->Width)
        x = 0;
    if(y >= self->Hight)
        y = 0;

    self->__SetPixel__(x,y,color);
}
void Screen_DeInit(struct screen *self)
{
    if(self->__DeInit__ != NULL)
        self->__DeInit__();
}

void Screen_SetEnable(struct screen *self,bool enable)
{
    if(self->__SetEnable__ != NULL)
    {
        if(enable == false)
            self->Clear(self,WHITE);
        self->__SetEnable__(enable);
    }
}
/**
    *
    * @brief  ��ˮƽ��
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    len���ߵĳ���
    * @param    width���ߵĿ���
    * @param    color:������ɫ
    * @retval ��
    *
  */
void Screen_DrawHLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//ˮƽ��
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
    * @brief  ������
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    len���ߵĳ���
    * @param    width���ߵĿ���
    * @param    color:������ɫ
    * @retval ��
    *
  */
void Screen_DrawVLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//����
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
    * @brief  ����
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    xe������x����
    * @param    ye������y����
    * @param    color:������ɫ
    * @retval ��
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
    * @brief  ������
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    xe������x����
    * @param    ye������y����
    * @param    color:������ɫ
    * @retval ��
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
    * @brief  ��Բ
    * @param    x��Բ��x����
    * @param    y��Բ��y����
    * @param    r��Բ�뾶
    * @param    color:������ɫ
    * @retval ��
    *
  */
void Screen_DrawCircle(struct screen *self,uint16_t x,uint16_t y,uint16_t r,uint16_t color)
{
    uint16_t dx, dy = r;

    if((x>=r) &&((self->Hight-x)>=r) && (y>=r) && ((self->Width-y)>=r))       //ȷ������Բ����Ļ��Χ�ڣ�û�г�������ߣ�(�ݲ�֧������Ļ���ཻ)
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
    * @brief    ����
    * @param    color:������ɫ
    * @retval ��
    *
  */
void Screen_Clear(struct screen *self,uint16_t color)
{
    uint16_t i,j;

    if(self->__Fill__ != NULL)
    {
        self->__Fill__(0,0,self->Width,self->Hight,color);
    }
    else
    {
        for(i=0;i<self->Width;i++)      //160
        {
            for(j=0;j<self->Hight;j++)
            {
                self->SetPixel(self,i,j,color);
            }
        }
    }
}
/**
    *
    * @brief    ��Χ���
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    xe������x����
    * @param    ye������y����
    * @param    color:�����ɫ
    * @retval ��
    *
  */
void Screen_Fill(struct screen *self,uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    uint16_t i,j;
    if(self->__Fill__ != NULL)
    {
        self->__Fill__(xs,ys,xe,ye,color);
    }
    else
    {
        for(i=xs;i<xe;i++)
            for(j=ys;j<=ye;j++)
                self->SetPixel(self,i,j,color);
    }
}
/**
    *
    * @brief  ��ʾ�ַ�
    * @param    x��      x����
    * @param    y��      y����
    * @param    ch��         Ҫ��ʾ���ַ�
    * @param    color��  �ַ���ɫ
    * @retval ��
    *
  */
void Screen_ShowChar(struct screen *self,uint16_t x,uint16_t y,uint8_t ch,uint16_t color)
{
    uint16_t i,j;

    if(x>self->Width)
       y+=self->Font.Hight,x=0;

//    if(self->__SetArea__ != NULL && self->__FastSetPixel__ != NULL)
//    {
//        self->__SetArea__(x,y,x+self->Font.Width - 1,y+self->Font.Hight - 1);
//
//        for(j=0; j< self->Font.Hight; j++)
//        {
//            for(i=0; i< self->Font.Width; i++)
//            {
//              if((Font_code8[ch-32][i]) & (0x01<<j))
//                  self->__FastSetPixel__(color);
//              else
//                  self->__FastSetPixel__(self->Font.Backcolor);
//            }
//        }
//    }
//    else
//    {
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
//    }
      if(ch == '\n')
      {
          self->Cursor.y += self->Font.Hight;
          self->Cursor.x = 0;
      }
}
/**
    *
    * @brief  ��ʾ�ַ�
    * @param    x��  x����
    * @param    y��  y����
    * @param    num��     Ҫ��ʾ������
    * @param    color��  �ַ���ɫ
    * @retval ��
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
    * @brief  ��ʾ����
    * @param    ��
    * @retval ��
    *
  */
void Screen_ShowChinese(struct screen *self,uint16_t x,uint16_t y,uint16_t Chinese,uint16_t color)
{
    ;
}

/**
    *
    * @brief  ��ʾ�ַ���
    * @param    x��  x����
    * @param    y��  y����
    * @param    string���ַ���
    * @param    color��  �ַ���ɫ
    * @retval ��
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
    * @brief  ��ʾ�ַ���
    * @param    x��  x����
    * @param    y��  y����
    * @param    string���ַ���
    * @param    len��     �ַ�������
    * @param    color��  �ַ���ɫ
    * @retval ��
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
    * @brief  ��ĳ�����꿪ʼдһ����ʽ���ַ�
    * @param    x��  x����
    * @param    y��  y����
    * @param  format�� ��ʽ
    * @param  ...��        �ɱ����
    * @retval ��
    * @attention �÷�ͬprintf
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
    * @brief  дһ��
    * @param    format�� ��ʽ
    * @param    ...��        �ɱ����
    * @retval ��
    * @attention �÷�ͬprintf�����Զ�����
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
    * @brief  д��X��
    * @param    format�� ��ʽ
    * @param    ...��        �ɱ����
    * @retval ��
    * @attention �÷�ͬprintf��������࿪ʼ���̶�д��row��,��֧���Զ����У����� y = row * self.Font.Hight
    * @          row = 1 ʵ�ʵ�y�������self.Font.Hight
    * @                  �˺�����x����ʼ��Ϊ0
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


void Screen_ClearLine(struct screen *self,uint16_t row,uint16_t color)
{
    self->Fill(self,0,row * self->Font.Hight,self->Width,(row + 1) * self->Font.Hight,color);
}

void Screen_ClearLines(struct screen *self,uint16_t row_start,uint16_t row_end,uint16_t color)
{
    self->Fill(self,0,row_start * self->Font.Hight,self->Width,(row_end + 1) * self->Font.Hight,color);
}



/**
    *
    * @brief    ����Screen���
    * @param    x�����x����
    * @param    y�����y����
    * @retval ��
    * @attention�� ������Screen_WriteLine������������������ǰ���ù�겻������
    *
  */
void Screen_SetCursor(struct screen *self,uint16_t x,uint16_t y)
{
    self->Cursor.x=x;
    self->Cursor.y=y;
}

/**
    *
    * @brief    ������ʾ�������С
    * @param    width������Ŀ���
    * @param    hight������ĸ߶�
  * @retval ��
    *
  */
void Screen_SetFontSize(struct screen *self,uint16_t width,uint16_t hight)
{
    self->Font.Hight=hight;
    self->Font.Width=width;
}
/**
    *
    * @brief    ������ʾ��������ɫ
    * @param    color:������ɫ
  * @retval ��
    *   @attention�� ������Printf ������ write����
    *
  */
void Screen_SetFontColor(struct screen *self,uint16_t color)
{
    self->Font.Color=color;
}

/**
    *
    * @brief  LCD����
    * @param    ��
  * @retval ��
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

    self->ClearLine = Screen_ClearLine;
    self->ClearLines = Screen_ClearLines;

    self->SetCursor = Screen_SetCursor;
    self->SetFontColor = Screen_SetFontColor;
    self->SetFontSize = Screen_SetFontSize;

    self->Test = Screen_Test;
    self->Cursor.x = 0;
    self->Cursor.y = 0;
    self->ShowString = Screen_ShowString;

    self->Fill = Screen_Fill;

    self->SetPixel = Screen_SetPixel;

    self->SetEnable = Screen_SetEnable;

    self->CenterLine = self->Hight/(self->Font.Hight * 2);

    uint8_t res = 0;
    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);

    self->Clear(self,WHITE);

    return res;
}
