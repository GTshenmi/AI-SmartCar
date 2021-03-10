#ifndef __LQ_CSI_H
#define __LQ_CSI_H

#include "sys_driver.h"
#include "LQ_MT9V034M.h"

#define CSI_IIC_SDA_GPIO &GPIO_Resources[12].GPION//Camera_IIC_SDA_GPIO
#define CSI_IIC_SCL_GPIO &GPIO_Resources[13].GPION//Camera_IIC_SCL_GPIO
#define CSI_IIC_SDA(x) ((x)?GPIOx.Write(CSI_IIC_SDA_GPIO,1):GPIOx.Write(CSI_IIC_SDA_GPIO,0))
#define CSI_IIC_SCL(x) ((x)?GPIOx.Write(CSI_IIC_SCL_GPIO,1):GPIOx.Write(CSI_IIC_SCL_GPIO,0))

#define APP_CAMERA_FRAME_BUFFER_COUNT 4
#define FRAME_BUFFER_ALIGN 64           //���� 
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //ʹ������ͷ��������

typedef struct
{ 
    uint16_t b         : 5;
    uint16_t g         : 6;
    uint16_t r         : 5;
}rgb565_t;


typedef union 
{
    uint16_t rgb565;
    uint8_t  gray[2];
    rgb565_t rgb;
}pixel_t;

extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
extern volatile pixel_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];
extern camera_config_t cameraConfig;   //����ͷ���ýṹ��



/**
  * @brief    ��λ����ͼ
  *
  * @param    
  *
  * @return   
  *
  * @note     ���ڴ������ݱȽ���
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void Test_Camera_Reprot(void);


/**
  * @brief    ����ͷ��ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void CSI_CameraInit(void);


#endif

