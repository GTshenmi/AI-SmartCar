/*
 * capture.c
 *
 *  Created on: 2020��12��14��
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
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (j = 0; j < imagein->Hight; j++)
    {
        for (i = 0; i < imagein->Width; i++)
        {
            HistoGram[imagein->Array[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];   //ǰ�����ص���
        PixelFore = Amount - PixelBack;         //�������ص���
        OmegaBack = (float)PixelBack / Amount;//ǰ�����ذٷֱ�
        OmegaFore = (float)PixelFore / Amount;//�������ذٷֱ�
        PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
        MicroBack = (float)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //���������ֵ;
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
              if(imagein->Array[i][j] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
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
            case 0:/*����*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*��ѯ*/
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

    UART_PutChar(UART0, 0xfe);  //֡ͷ
    UART_PutChar(UART0, 0xef);  //֡ͷ

    for(i = 0; i < IMAGEH; i++)
    {
        for(j = 0; j < IMAGEW; j++)
        {
          //  if(Image_Data[i][j] == 0xfe )  //��ֹ������֡β
            {
          //      Image_Data[i][j] = 0xff;
            }
         //   UART_PutChar(UART0, Image_Data[i][j]); //��������

        }
    }
    UART_PutChar(UART0, 0xef);  //֡β
    UART_PutChar(UART0, 0xfe);  //֡β
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




