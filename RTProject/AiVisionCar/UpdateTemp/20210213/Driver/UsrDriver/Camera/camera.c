/*
 * camera.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "camera.h"

uint16_t fps = 100;

uint8_t CameraInit(void *config)
{
    //CSI_CameraInit();
    return 0;
}

uint8_t CameraStart()
{
     //CAMERA_RECEIVER_Start(&cameraReceiver);   // ��������camera����
     return 0;
}

uint8_t CameraStop()
{
    //CAMERA_RECEIVER_Stop(&cameraReceiver);
    return 0;
}

uint8_t CameraRead()
{

    return 0;
}

uint8_t CameraGetState()
{
    //return CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr);
    return 0;
}

