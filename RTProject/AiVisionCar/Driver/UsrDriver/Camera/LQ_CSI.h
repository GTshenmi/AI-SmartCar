#ifndef __LQ_CSI_H
#define __LQ_CSI_H

#include "sys_driver.h"
#include "LQ_MT9V034M.h"

#define CSI_IIC_SDA_GPIO &GPIO_Resources[12].GPION//Camera_IIC_SDA_GPIO
#define CSI_IIC_SCL_GPIO &GPIO_Resources[13].GPION//Camera_IIC_SCL_GPIO
#define CSI_IIC_SDA(x) ((x)?GPIOx.Write(CSI_IIC_SDA_GPIO,1):GPIOx.Write(CSI_IIC_SDA_GPIO,0))
#define CSI_IIC_SCL(x) ((x)?GPIOx.Write(CSI_IIC_SCL_GPIO,1):GPIOx.Write(CSI_IIC_SCL_GPIO,0))

#define APP_CAMERA_FRAME_BUFFER_COUNT 4
#define FRAME_BUFFER_ALIGN 64           //对齐 
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿

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
extern camera_config_t cameraConfig;   //摄像头配置结构体



/**
  * @brief    上位机看图
  *
  * @param    
  *
  * @return   
  *
  * @note     串口传输数据比较慢
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void Test_Camera_Reprot(void);


/**
  * @brief    摄像头初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void CSI_CameraInit(void);


#endif

