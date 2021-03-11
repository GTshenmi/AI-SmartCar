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
    pixel_t **Array;
}image_t;

//typedef struct
//{
//    int Hight;
//    int Width;
//
//    uint16_t **Array;
//}image_t;

typedef struct
{
    int xs;
    int ys;
    int xe;
    int ye;
}image_range_t;

#endif /* OS_DEVICE_CAPTURE_IMAGE_H_ */
