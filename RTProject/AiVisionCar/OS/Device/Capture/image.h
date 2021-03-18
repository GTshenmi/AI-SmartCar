/*
 * image.h
 *
 *  Created on: 2021Äê2ÔÂ16ÈÕ
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

extern image_t ImageUsed;
extern image_t ImageBinary;

void Image_Binarization(image_t *imagein,image_t *imageout,int Threshold);
void Image_Sobel(image_t *imagein,image_t *imageout, unsigned char Threshold);
void GetUsedImage(image_t *imagein,image_t *imageout);

#endif /* OS_DEVICE_CAPTURE_IMAGE_H_ */
