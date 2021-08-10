/*
 * image.h
 *
 *  Created on: 2021年2月16日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_CAPTURE_IMAGE_H_
#define OS_DEVICE_CAPTURE_IMAGE_H_

#include "platform.h"
#include "driver.h"

typedef struct
{
    int Hight;
    int Width;

    //pixel_t *Array;
    //uint16_t (*Array)[188];
    volatile pixel_t (*Array)[188];
}image_t;

typedef struct
{
    int Hight;
    int Width;

    uint16_t *Array;
    
}image_cache_t;

typedef struct
{
    int xs;
    int ys;
    int xe;
    int ye;
}image_range_t;

typedef struct
{
    int LeftCol[120];
    int RightCol[120];
    int MidCol[120];
}image_Col_t;

extern image_t ImageUsed;
extern image_t ImageBinary;
extern image_t ImageBinary2;

int Image_GetOSTU(image_t *imagein);/*大律法阈值*/
void Image_Binarization(image_t *imagein,image_t *imageout,int Threshold);
void Image_Sobel(image_t *imagein,image_t *imageout, unsigned char Threshold);
void GetUsedImage(image_t *imagein,image_t *imageout);
void Image_SobelAutoThreshold(image_t *imagein,image_t *imageout);

#endif /* OS_DEVICE_CAPTURE_IMAGE_H_ */
