/*
 * image.c
 *
 *  Created on: 2021��2��16��
 *      Author: 936305695
 *      Brief: ͼ���������⺯��
 */
#include "image.h"

image_t ImageUsed = 
{
    .Hight = 90,
    .Width = 120,
    .Array = Image_Use,
};

image_t ImageBinary = 
{
    .Hight = 90,
    .Width = 120,
    .Array = Pixle,  
};
/*!
  * @example :
  *             global uint16_t array[120][188];
  *
  *             Image_Init(image,&array[0][0],120,188);
  */
void Image_Init(image_t *image,uint16_t *array_addr/*ȫ�ֶ��������*/,uint Hight,uint Width)
{
    //image->Array = array_addr;
    image->Hight = Hight;
    image->Width = Width;
}

uint8_t Image_AssertRange(image_t image,image_range_t range)
{
    uint8_t res = 0;

    if(range.xs < 0 || range.xe < 0 || range.ys < 0 || range.ye < 0)
        res += 1;

    if(range.xs > image.Width || range.xe > image.Width || range.ys > image.Hight || range.ye > image.Hight)
        res += 1;

    if(range.xs > range.xe || range.ys > range.ye)
        res += 1;

    return res;
}

//imageout.array[y-Hight][x-Width];
void Image_Cut(image_t *imagein,image_t *imageout,image_range_t range)
{
//    if(Image_AssertRange(*imagein,range) || Image_AssertRange(*imageout,range))
//    {
//        exit(0);
//    }
    
    for(int i = range.xs ; i < range.xe ;i++)
    {
        for(int j = range.ys ; j < range.ye ;j++)
        {                     
             imageout->Array[j][i] = imagein->Array[j][i];               
        }
    }
}

void GetUsedImage(image_t *imagein,image_t *imageout)
{
    //ע�⣬ʹ��csiFrameBuf����ʱ�����ˢ��һ��D-Cache ��Ȼ�ϴ����ݿ��ܻ�����cache���棬������ݴ���
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }
    SCB_EnableDCache();
    
    uint8_t div_h, div_w;
    uint32_t temp_h = 0;
    uint32_t temp_w = 0;
    uint32_t row_start = 0;
    uint32_t lin_start = 0;
    
    div_h = imagein->Hight/imageout->Hight;
    div_w = imagein->Width/imageout->Width;
    
    /* ������ȡͼ�� */
    if(imageout->Hight * div_h != imagein->Hight)
    {
        row_start = (imagein->Hight - imageout->Hight * div_h)/2;
        temp_h = (uint32_t)(row_start/2) * APP_CAMERA_WIDTH;
    }
    if(imageout->Width * div_w != imagein->Width)
    {
        lin_start = (imagein->Width - imageout->Width * div_w)/2;       
    }
    for(int i = 0; i < imageout->Hight; i += 2)
    {

        temp_w = lin_start;
        for(int j = 0; j < imageout->Width; j++)
        {  
            Image_Use[i][j].gray[0] = (*((uint8_t *)fullCameraBufferAddr +  temp_h + temp_w));
            Image_Use[i+1][j].gray[0] = (*((uint8_t *)fullCameraBufferAddr +  temp_h + temp_w + APP_CAMERA_WIDTH));
            //Image_Use[i][j].gray[0] = (*((uint8_t *)imagein +  temp_h + temp_w));
            //Image_Use[i+1][j].gray[0] = (*((uint8_t *)imagein +  temp_h + temp_w + APP_CAMERA_WIDTH));

            temp_w += div_w;
        }
        temp_h += div_h * 2 * APP_CAMERA_WIDTH;
    }
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
}

void Image_Zip(image_t *imagein,image_t *imageout)
{
    unsigned int div_h, div_w;
    unsigned long temp_h = 0;
    unsigned long temp_w = 0;
    unsigned long row_start = 0;
    unsigned long lin_start = 0;

    div_h = imagein->Hight/imageout->Hight;
    div_w = imagein->Width/imageout->Width;

#if ((defined(Chip)) && ((Chip == TC264) || (Chip == TC377)))
#pragma warning 549         // ���ξ���
#endif
    /* ������ȡͼ�� */
    if(imageout->Hight * div_h != imagein->Hight)
    {
        row_start = (imagein->Hight - imageout->Hight * div_h)/2;
        temp_h = row_start;
    }
    if(imageout->Width * div_w != imagein->Width)
    {
        lin_start = (imagein->Width - imageout->Width * div_w)/2;
    }
    for(uint i = 0; i < imageout->Hight; i++)
    {
        temp_w = lin_start;
        for(uint j = 0; j < imageout->Width; j++)
        {
            imageout->Array[i][j] = imagein->Array[temp_h][temp_w];  
            
            temp_w += div_w;
        }
        temp_h += div_h;
    }
#if ((defined(Chip)) && ((Chip == TC264) || (Chip == TC377)))
#pragma warning 549         // �򿪾���
#endif
}

int Imagee_GetAverageThreshold(image_t *imagein,int base)/*ƽ����ֵ*/
{
    int Threshold = 0;
    int tv = 0;
    int i = 0,j = 0;
    
    pixel_t pixel;

    //�ۼ�
    for(i = 0; i < imagein->Hight; i++)
    {
        for(j = 0; j < imagein->Width; j++)
        {
            pixel = imagein->Array[i][j];
            
            tv+= pixel.gray[0];   //�ۼ�
        }
    }

    Threshold=tv/(imagein->Hight * imagein->Width);        //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100

    Threshold=Threshold + base;      //�˴���ֵ���ã����ݻ����Ĺ������趨

    return Threshold;
}

int Image_GetOSTU(image_t *imagein)/*���ɷ���ֵ*/
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
            HistoGram[imagein->Array[j][i].gray[0]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
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

void Image_Binarization(image_t *imagein,image_t *imageout,int Threshold)/*��ֵ��*/
{
      int i = 0,j = 0;

//      if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
//      {
//          exit(0);
//      }

      for(i = 0 ; i < imagein->Hight ; i++)
      {
          for(j = 0 ; j < imagein->Width ; j++)
          {
              if(imagein->Array[i][j].gray[0] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
                imageout->Array[i][j].binary = 0;
              else
                imageout->Array[i][j].binary = 1;
          }
      }

}


/*!
  * @brief    ����soble���ؼ�����ӵ�һ�ֱ��ؼ��
  *
  * @param    imageIn    ����ͼ��
  *           imageOut   ���ͼ��      ����Ķ�ֵ����ı�����Ϣ
  *           Threshold  ��ֵ
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void Image_Sobel(image_t *imagein,image_t *imageout, unsigned char Threshold)
{
//    if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
//    {
//        exit(0);
//    }

    /** �����˴�С */
    int KERNEL_SIZE = 3;
    int xStart = KERNEL_SIZE/2;
    int xEnd   = imagein->Width - KERNEL_SIZE/2;
    int yStart = KERNEL_SIZE/2;
    int yEnd   = imagein->Hight - KERNEL_SIZE/2;
    int i, j, k;
    int temp[4];
    for(i = yStart; i < yEnd; i++)
    {
        for(j = xStart; j < xEnd; j++)
        {
            /* ���㲻ͬ�����ݶȷ�ֵ  */
            temp[0] = -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i-1][j+1].gray[0]     //{{-1, 0, 1},
                      -(int)imagein->Array[i][j-1].gray[0]   +(int)imagein->Array[i][j+1].gray[0]        // {-1, 0, 1},
                      -(int)imagein->Array[i+1][j-1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    // {-1, 0, 1}};

            temp[1] = -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i+1][j-1].gray[0]     //{{-1, -1, -1},
                      -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       // { 0,  0,  0},
                      -(int)imagein->Array[i-1][j+1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    // { 1,  1,  1}};


            temp[2] = -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i][j-1].gray[0]       //  0, -1, -1
                      -(int)imagein->Array[i][j+1].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       //  1,  0, -1
                      -(int)imagein->Array[i-1][j+1].gray[0] + (int)imagein->Array[i+1][j-1].gray[0];    //  1,  1,  0

            temp[3] = -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i][j+1].gray[0]      // -1, -1,  0
                      -(int)imagein->Array[i][j-1].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       // -1,  0,  1
                      -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* �ҳ��ݶȷ�ֵ���ֵ  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if(temp[0] > Threshold)
            {
                imageout->Array[i][j].binary = 1;
            }
            else
            {
                imageout->Array[i][j].binary = 0;
            }
        }
    }
}

/*!
  * @brief    ����soble���ؼ�����ӵ�һ���Զ���ֵ���ؼ��
  *
  * @param    imageIn    ��������
  *           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void Image_SobelAutoThreshold(image_t *imagein,image_t *imageout)
{
//    if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
//    {
//        exit(0);
//    }
    /** �����˴�С */
    int KERNEL_SIZE = 3;
    int xStart = KERNEL_SIZE/2;
    int xEnd   = imagein->Width - KERNEL_SIZE/2;
    int yStart = KERNEL_SIZE/2;
    int yEnd   = imagein->Hight - KERNEL_SIZE/2;
    int i, j, k;
    int temp[4];
    for(i = yStart; i < yEnd; i++)
    {
        for(j = xStart; j < xEnd; j++)
        {
            /* ���㲻ͬ�����ݶȷ�ֵ  */
            temp[0] = -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i-1][j+1].gray[0]     //{{-1, 0, 1},
                      -(int)imagein->Array[i][j-1].gray[0]   + (int)imagein->Array[i][j+1].gray[0]       // {-1, 0, 1},
                      -(int)imagein->Array[i+1][j-1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    // {-1, 0, 1}};

            temp[1] = -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i+1][j-1].gray[0]     //{{-1, -1, -1},
                      -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       // { 0,  0,  0},
                      -(int)imagein->Array[i-1][j+1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    // { 1,  1,  1}};


            temp[2] = -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i][j-1].gray[0]       //  0, -1, -1
                      -(int)imagein->Array[i][j+1].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       //  1,  0, -1
                      -(int)imagein->Array[i-1][j+1].gray[0] + (int)imagein->Array[i+1][j-1].gray[0];    //  1,  1,  0

            temp[3] = -(int)imagein->Array[i-1][j].gray[0]   + (int)imagein->Array[i][j+1].gray[0]       // -1, -1,  0
                      -(int)imagein->Array[i][j-1].gray[0]   + (int)imagein->Array[i+1][j].gray[0]       // -1,  0,  1
                      -(int)imagein->Array[i-1][j-1].gray[0] + (int)imagein->Array[i+1][j+1].gray[0];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* �ҳ��ݶȷ�ֵ���ֵ  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            /* ʹ�����ص����������ص�֮�͵�һ������    ��Ϊ��ֵ  */
            temp[3] = (int)imagein->Array[i-1][j-1].gray[0]   + (int)imagein->Array[i-1][j].gray[0] +  (int)imagein->Array[i-1][j+1].gray[0]
                     +(int)imagein->Array[i  ][j-1].gray[0]   + (int)imagein->Array[i  ][j].gray[0] +  (int)imagein->Array[i  ][j+1].gray[0]
                     +(int)imagein->Array[i+1][j-1].gray[0]   + (int)imagein->Array[i+1][j].gray[0] +  (int)imagein->Array[i+1][j+1].gray[0];

            if(temp[0] > temp[3]/6.0f)
            {
                imageout->Array[i][j].binary = 1;
            }
            else
            {
                imageout->Array[i][j].binary = 0;
            }
        }
    }
}
