/*
 * camera.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "camera.h"

uint16_t fps = 100;

uint8_t CameraInit(void *config)
{
    uint8_t *fps = (uint8_t *)config;
    CSI_CameraInit(*fps);
    return 0;
}

uint8_t CameraStart()
{
     return CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据  
}

uint8_t CameraStop()
{
    return CAMERA_RECEIVER_Stop(&cameraReceiver);
}

uint8_t CameraGetState()
{
    uint8_t res = 1;
 
    if(CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr) == kStatus_Success)
      res = 0;
    else
      res = 1;
    
    return res;
}

uint8_t CameraRead()
{   
    while(CameraGetState());
    
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
    
    while(CameraGetState());
    
    return 0;
}