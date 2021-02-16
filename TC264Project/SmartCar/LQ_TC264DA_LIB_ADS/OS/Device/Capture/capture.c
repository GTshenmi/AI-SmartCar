/*
 * capture.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "capture.h"

int Image_GetOSTU(image_t *imagein)
{
    signed short i,j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelIntegralBack = 0;
    unsigned long PixelIntegral = 0;
    signed long PixelIntegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < imagein->Hight; j++)
    {
        for (i = 0; i < imagein->Width; i++)
        {
            HistoGram[imagein->Array[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];   //前景像素点数
        PixelFore = Amount - PixelBack;         //背景像素点数
        OmegaBack = (float)PixelBack / Amount;//前景像素百分比
        OmegaFore = (float)PixelFore / Amount;//背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}

void ImageBinarization(image_t *imagein,image_t *imageout,int Threshold)
{
      int i = 0,j = 0;
      //unsigned long  tv=0;
      //char txt[16];

      if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
      {
          //exit(0);
      }

      for(i = 0 ; i < imagein->Hight ; i++)
      {
          for(j = 0 ; j < imagein->Width ; j++)
          {
              if(imagein->Array[i][j] >Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                imageout->Array[i][j] =1;
              else
                imageout->Array[i][j] =0;
          }
      }

}
/*
 *                                                  Capture.Stop()
 *           ___________________________________________________________________________________________________
 *          |                                  |                                |                               |
 *          |                                  |                                |                               |
 *         \|/         Capture.Start();        |          Capture.Read()        |           Read Finish.        |
 *      Capture_Stop-------------------->Capture_Start-------------------->Capture_Busy-------------------->Capture_Fin
 *                                            /|\                                                               |
 *                                             |                                                                |
 *                                             |________________________________________________________________|
 *                                                                Capture.ClearFinFlag()
 * */
image_t Cap_Read(struct capture *self,uint16_t flags)
{
    if(self->State == Capture_Start)
    {
        switch(flags)
        {
            case 0:/*阻塞*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*查询*/
                self->State = Capture_Busy;
                self->__Start__();
                break;
            default:
                break;
        }
    }

    return self->ImageCache;
}

void Cap_ClearReadFinFlag(struct capture *self)
{
    if(self->State == Capture_Fin)
        self->State = Capture_Start;
}

cap_state_t Cap_GetState(struct capture *self)
{
    if(self->State == Capture_Busy)
    {
       if(self->__GetState__())
           self->State = Capture_Fin;
    }

    return self->State;
}

void Cap_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self,0);
    self->Report(self);
}

void Cap_Start(struct capture *self)
{
    if(self->State == Capture_Stop)
        self->State = Capture_Start;
}

void Cap_Stop(struct capture *self)
{
    if(self->State != Capture_Stop)
        self->__Stop__();

    self->State = Capture_Stop;
}

void Cap_Report(struct capture *self)
{
    int j,i;

    UART_PutChar(UART0, 0xfe);  //帧头
    UART_PutChar(UART0, 0xef);  //帧头

    for(i = 0; i < IMAGEH; i++)
    {
        for(j = 0; j < IMAGEW; j++)
        {
          //  if(Image_Data[i][j] == 0xfe )  //防止错误发送帧尾
            {
          //      Image_Data[i][j] = 0xff;
            }
         //   UART_PutChar(UART0, Image_Data[i][j]); //发送数据

        }
    }
    UART_PutChar(UART0, 0xef);  //帧尾
    UART_PutChar(UART0, 0xfe);  //帧尾
}

void Cap_Show(struct capture *self)
{

}

uint8_t Cap_Init(struct capture *self,uint8_t fps)
{
    self->Read = Cap_Read;

    self->GetState = Cap_GetState;

    self->Test = Cap_Test;

    self->Report = Cap_Report;

    self->Show = Cap_Show;

    self->Reg = 0;

    uint8_t res = 0;

    uint8_t *init_config = (uint8_t *)self->__InitConfig__;

    *init_config = fps;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




