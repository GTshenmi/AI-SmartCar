/*
 * image.c
 *
 *  Created on: 2021年2月16日
 *      Author: 936305695
 *      Brief: 图像处理基础库函数
 */
#include "image.h"

/*!
  * @example :
  *             global uint8_t array[120][188];
  *
  *             Image_Init(image,array,120,188);
  */
void Image_Init(image_t *image,uint8_t **array/*全局定义的数组*/,uint Hight,uint Width)
{
    image->Array = array;
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

void Image_Cut(image_t *imagein,image_t *imageout,image_range_t range)
{
    if(Image_AssertRange(*imagein,range) || Image_AssertRange(*imageout,range))
    {
        exit(0);
    }
    for(int i = range.xs ; i < range.xe ;i++)
    {
        for(int j = range.ys ; j < range.ye ;j++)
        {
             imageout->Array[i][j] = imagein->Array[i][j];
        }
    }
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

#pragma warning 549         // 屏蔽警告
    /* 从中心取图像 */
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
#pragma warning 549         // 打开警告
}

int Imagee_GetAverageThreshold(image_t *imagein,int base)/*平均阈值*/
{
    int Threshold = 0;
    int tv = 0;
    int i = 0,j = 0;

    //累加
    for(i = 0; i < imagein->Hight; i++)
    {
        for(j = 0; j < imagein->Width; j++)
        {
            tv+=imagein->Array[i][j];   //累加
        }
    }

    Threshold=tv/(imagein->Hight * imagein->Width);        //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100

    Threshold=Threshold + base;      //此处阈值设置，根据环境的光线来设定

    return Threshold;
}

int Image_GetOSTU(image_t *imagein)/*大律法阈值*/
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

void ImageBinarization(image_t *imagein,image_t *imageout,int Threshold)/*二值化*/
{
      int i = 0,j = 0;

      if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
      {
          exit(0);
      }

      for(i = 0 ; i < imagein->Hight ; i++)
      {
          for(j = 0 ; j < imagein->Width ; j++)
          {
              if(imagein->Array[i][j] >Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                imageout->Array[i][j] = 1;
              else
                imageout->Array[i][j] = 0;
          }
      }

}


/*!
  * @brief    基于soble边沿检测算子的一种边沿检测
  *
  * @param    imageIn    输入图像
  *           imageOut   输出图像      保存的二值化后的边沿信息
  *           Threshold  阈值
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
    if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
    {
        exit(0);
    }

    /** 卷积核大小 */
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
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i-1][j+1]     //{{-1, 0, 1},
                      -(int)imagein->Array[i][j-1]   +(int)imagein->Array[i][j+1]        // {-1, 0, 1},
                      -(int)imagein->Array[i+1][j-1] + (int)imagein->Array[i+1][j+1];    // {-1, 0, 1}};

            temp[1] = -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i+1][j-1]     //{{-1, -1, -1},
                      -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i+1][j]       // { 0,  0,  0},
                      -(int)imagein->Array[i-1][j+1] + (int)imagein->Array[i+1][j+1];    // { 1,  1,  1}};


            temp[2] = -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i][j-1]       //  0, -1, -1
                      -(int)imagein->Array[i][j+1]   + (int)imagein->Array[i+1][j]       //  1,  0, -1
                      -(int)imagein->Array[i-1][j+1] + (int)imagein->Array[i+1][j-1];    //  1,  1,  0

            temp[3] = -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i][j+1]       // -1, -1,  0
                      -(int)imagein->Array[i][j-1]   + (int)imagein->Array[i+1][j]       // -1,  0,  1
                      -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i+1][j+1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if(temp[0] > Threshold)
            {
                imageout->Array[i][j] = 1;
            }
            else
            {
                imageout->Array[i][j] = 0;
            }
        }
    }
}

/*!
  * @brief    基于soble边沿检测算子的一种自动阈值边沿检测
  *
  * @param    imageIn    输入数组
  *           imageOut   输出数组      保存的二值化后的边沿信息
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
    if(imagein->Hight != imageout->Hight || imagein->Width != imageout->Width)
    {
        exit(0);
    }
    /** 卷积核大小 */
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
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i-1][j+1]     //{{-1, 0, 1},
                      -(int)imagein->Array[i][j-1]   + (int)imagein->Array[i][j+1]       // {-1, 0, 1},
                      -(int)imagein->Array[i+1][j-1] + (int)imagein->Array[i+1][j+1];    // {-1, 0, 1}};

            temp[1] = -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i+1][j-1]     //{{-1, -1, -1},
                      -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i+1][j]       // { 0,  0,  0},
                      -(int)imagein->Array[i-1][j+1] + (int)imagein->Array[i+1][j+1];    // { 1,  1,  1}};


            temp[2] = -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i][j-1]       //  0, -1, -1
                      -(int)imagein->Array[i][j+1]   + (int)imagein->Array[i+1][j]       //  1,  0, -1
                      -(int)imagein->Array[i-1][j+1] + (int)imagein->Array[i+1][j-1];    //  1,  1,  0

            temp[3] = -(int)imagein->Array[i-1][j]   + (int)imagein->Array[i][j+1]       // -1, -1,  0
                      -(int)imagein->Array[i][j-1]   + (int)imagein->Array[i+1][j]       // -1,  0,  1
                      -(int)imagein->Array[i-1][j-1] + (int)imagein->Array[i+1][j+1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            /* 使用像素点邻域内像素点之和的一定比例    作为阈值  */
            temp[3] = (int)imagein->Array[i-1][j-1]   + (int)imagein->Array[i-1][j] +  (int)imagein->Array[i-1][j+1]
                     +(int)imagein->Array[i  ][j-1]   + (int)imagein->Array[i  ][j] +  (int)imagein->Array[i  ][j+1]
                     +(int)imagein->Array[i+1][j-1]   + (int)imagein->Array[i+1][j] +  (int)imagein->Array[i+1][j+1];

            if(temp[0] > temp[3]/12.0f)
            {
                imageout->Array[i][j] = 1;
            }
            else
            {
                imageout->Array[i][j] = 0;
            }
        }
    }
}

