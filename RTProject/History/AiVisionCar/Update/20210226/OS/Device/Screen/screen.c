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
    self->__SetPixel__(x,y,color);
}
void Screen_DeInit(struct screen *self)
{
    self->__DeInit__();
}
/**
    *
    * @brief  ��ˮƽ��
    * @param    xs����ʼx����
    * @param    ys����ʼy����
    * @param    len���ߵĳ���
    * @param    width���ߵĿ��
    * @param    color:������ɫ
  * @retval ��
    *
  */
void Screen_DrawHLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//ˮƽ��
{
  uint16_t i,j;

  for(i=0;i<=len;i++)      //160
  {
     for(j=0;j<width;j++)
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
    * @param    width���ߵĿ��
    * @param    color:������ɫ
  * @retval ��
    *
  */
void Screen_DrawVLine(struct screen *self,uint16_t xs,uint16_t ys,uint16_t len,uint16_t width,uint16_t color)//����
{
        uint16_t i,j;

        for(i=0;i<=len;i++)      //160
        {
            for(j=0;j<width;j++)
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

        for(i=0;i<self->Hight;i++)      //160
        {
            for(j=0;j<self->Width;j++)
            {
                self->SetPixel(self,i,j,color);
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
        for(i=xs;i<xe;i++)
            for(j=ys;j<=ye;j++)
                self->SetPixel(self,i,j,color);
}
/**
    *
    * @brief  ��ʾ�ַ�
    * @param    x��      x����
    * @param    y��      y����
    * @param    ch��         Ҫ��ʾ���ַ�
    * @param    color��  �ַ���ɫ
  * @retval ��
    *   @attention ʵ������x=row*������,ʵ������y=col*����߶�
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
    * @brief  ��ʾ�ַ�
    * @param    row��        ��
    * @param    col��        ��
    * @param    num��            Ҫ��ʾ������
    * @param    color��  �ַ���ɫ
  * @retval ��
    *   @attention ʵ������x=row*������,ʵ������y=col*����߶�
    *
  */
void Screen_ShowNum(struct screen *self,uint16_t row,uint16_t col,sint32_t num,uint16_t color)
{
            unsigned int len=0;
            long temp=num;
            unsigned int i=0;
            if(num<0)
            {
                    self->ShowChar(self,row*self->Font.Width,col*self->Font.Hight,'-',color);
                    num=-num;
                    row++;
            }
            if(num==0)
                self->ShowChar(self,row*self->Font.Width,col*self->Font.Hight,'0',color);
            else
            {
                    while(temp)
                    {
                            len++;
                            temp/=10;
                    }
                    for(i=0;i<len;i++)
                    {
                        self->ShowChar(self,(row+len-i-1)*self->Font.Width,col*self->Font.Hight,(num%10+'0'),color);
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
void Screen_ShowChinese()
{
    ;
}
/**
    *
    * @brief  ��ʾ�ַ���
    * @param    row��        ��
    * @param    col��        ��
    * @param    *str��       Ҫ��ʾ���ַ����׵�ַ
    * @param    color��  �ַ���ɫ
  * @retval ��
    *   @attention ʵ������x=row*������,ʵ������y=col*����߶�
    *
  */
void Screen_ShowStr(struct screen *self,uint16_t row,uint16_t col,uint8_t *str,uint16_t color)
{
        uint32_t len=0,i;
        uint8_t *p=str;
        while(*p!='\0')
        {
                p++;
                len++;
        }
        for(i=0;i<len;i++)
        {
            self->ShowChar(self,(uint16_t)((row+i)*self->Font.Width),(uint16_t)(col*self->Font.Hight),str[i],color);
        }
}
void Screen_ShowString(struct screen *self,uint16_t row,uint16_t col,uint8_t *str,uint32_t len,uint16_t color)
{
    uint32_t i;

    for(i=0;i<len;i++)
    {
        self->ShowChar(self,(uint16_t)((row+i)*self->Font.Width),(uint16_t)(col*self->Font.Hight),str[i],color);
    }
}

//int fputc(int ch, FILE *f)
//{
//            struct screen *self = Screen.Self;
//
//            if(ch == '\n')    //����
//            {
//                    self->Cursor.x = 0;
//                    self->Cursor.y  += self->Font.Hight;
//                    return ch;
//            }
//            if(self->Cursor.x > self->Width -self->Font.Width)
//            {
//                    self->Cursor.x = 0;   //x����
//                    self->Cursor.y += self->Font.Hight;  //y������һ��
//            }
//             if(self->Cursor.y > self->Hight - self->Font.Hight)
//             {
//                 self->Cursor.x=0;
//                 self->Cursor.y=0;
//                 self->Clear(self,WHITE);
//                 return ch;  //ֱ���˳�
//             }
//            self->ShowChar(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint16_t)ch,self->Font.Color);;    //��ӡ�ַ�ch
//            self->Cursor.x += self->Font.Width;                 //��ת����һ��λ��, �Ƿ�Խ�������溯���ж�
//
//            return ch;
//}

/**
    *
    * @brief  д����
    * @param    format�� ��ʽ
    * @param    ...��        �ɱ����
  * @retval ��
    *   @attention �÷�ͬprintf
    *
  */
void Screen_WriteHalfLine(struct screen *self,const sint8_t *format,...)
{

        va_list arg;
        va_start(arg,format);

        char writebuf[SCREEN_MAX_BUF_LEN];

        uint32_t len = vsprintf(writebuf,format,arg);

        if(self->Cursor.x<(self->Width/2))
        {
                self->ShowString(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint8_t *)writebuf,len,self->Font.Color);
                self->Cursor.x=self->Width/2;
        }
        else
        {
                self->Cursor.x=(self->Width/2);
                self->ShowString(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint8_t *)writebuf,len,self->Font.Color);
                self->Cursor.x=self->Font.Width;
                self->Cursor.y+=self->Font.Hight;
        }
        va_end(arg);

}
/**
    *
    * @brief  дһ��
    * @param    format�� ��ʽ
    * @param    ...��        �ɱ����
  * @retval ��
    *   @attention �÷�ͬprintf�����Զ�����
    *
  */
void Screen_WriteLine(struct screen *self,const sint8_t *format,...)
{

        va_list(arg);
        va_start(arg,format);

        char writebuf[SCREEN_MAX_BUF_LEN];

        uint32_t len = vsprintf(writebuf,format,arg);

        self->ShowString(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint8_t *)writebuf,len,self->Font.Color);

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
    *   @attention �̶�д��X��,��֧���Զ����У��ַ�Խ��������˸��ʾ��
    *
  */
void Screen_WriteXLine(struct screen *self,uint16_t row,const sint8_t *format,...)
{
    va_list(arg);
    va_start(arg,format);

    self->Cursor.y=row*(self->Font.Hight);

    char writebuf[SCREEN_MAX_BUF_LEN];

    uint32_t len = vsprintf(writebuf,format,arg);

    self->ShowString(self,(uint16_t)self->Cursor.x,(uint16_t)self->Cursor.y,(uint8_t *)writebuf,len,self->Font.Color);

    self->Cursor.y+=self->Font.Hight;
    self->Cursor.x=0;

    va_end(arg);
}

/**
    *
    * @brief    ����LCD���
    * @param    x�����x����
    * @param    y�����y����
  * @retval ��
    *   @attention�� ������Printf ������ write����
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
    * @param    width������Ŀ��
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

        self->ShowNum(self,0,1,1,BLACK);
        self->ShowStr(self,1,1,(uint8_t *)"#include",YELLOW);
        self->ShowStr(self,9,1,(uint8_t *)"<stdio.h>",PURPLE);

        self->ShowNum(self,0,2,2,BLACK);
        self->ShowStr(self,1,2,(uint8_t *)"int",RED);
        self->ShowStr(self,5,2,(uint8_t *)"main()",BLACK);

        self->ShowNum(self,0,3,3,BLACK);
        self->ShowChar(self,1*self->Font.Width,self->Font.Hight*3,'{',RED);

        self->ShowNum(self,0,4,3,BLACK);
        self->ShowStr(self,2,4,(uint8_t *)"Printf(",BLACK);
        self->ShowChar(self,9*self->Font.Width,self->Font.Hight*4,'"',PURPLE);
        self->ShowStr(self,10,4,(uint8_t *)"Hello,World!",PURPLE);
        self->ShowChar(self,21*self->Font.Width,self->Font.Hight*4,'"',PURPLE);
        self->ShowStr(self,22,4,(uint8_t *)");",BLACK);

        self->WriteLine(self->Self,(char *)"4 return");
        self->ShowNum(self,9,5,0,BLUE);
        self->ShowChar(self,10*self->Font.Width,self->Font.Hight*5,';',BLACK);

        self->ShowNum(self,0,6,5,BLACK);
        self->ShowChar(self,1*self->Font.Width,self->Font.Hight*6,'}',RED);

        self->DrawHline(self,0,0,(uint16_t)self->Width,2,BLUE);
        self->DrawHline(self,0,(uint16_t)(self->Hight/2-10),(uint16_t)self->Width,2,BLUE);
        self->DrawVline(self,self->Width/2,self->Hight/2,self->Hight/2-self->Font.Hight,1,PURPLE);

        self->DrawSqr(self,1,self->Hight/2,self->Width-1,self->Hight-5,YELLOW);

        self->SetFontColor(self,GREEN);
        self->SetCursor(self,10*self->Font.Width,11*self->Font.Hight);
        self->WriteLine(self,"//%d",-1234);

        self->SetCursor(self,10*self->Font.Width,13*self->Font.Hight);
        self->WriteHalfLine(self,"//%.3f",1.23);

        self->WriteLine(self,"\n//Dog Rui");
        self->WriteLine(self,"//Dog Rui");

        self->WriteHalfLine(self,"//Doge");
        self->WriteHalfLine(self,"//Doge");

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

    self->WriteHalfLine = Screen_WriteHalfLine;
    self->WriteLine = Screen_WriteLine;
    self->WriteXLine =Screen_WriteXLine;

    self->SetCursor = Screen_SetCursor;
    self->SetFontColor = Screen_SetFontColor;
    self->SetFontSize = Screen_SetFontSize;

    self->Test = Screen_Test;
    self->Cursor.x = 0;
    self->Cursor.y = 0;
    self->ShowString = Screen_ShowString;

    self->SetPixel = Screen_SetPixel;

    uint8_t res = 0;
    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);
    return res;
}

