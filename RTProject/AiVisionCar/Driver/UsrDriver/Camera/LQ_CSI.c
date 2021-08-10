/*-----------------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【dev.env.】IAR8.30.1及以上版本
--------------------------------------------------------------------*/

#include "LQ_CSI.h"

#include "LQ_MT9V034M.h"

AT_DTCRAM_SECTION(volatile pixel_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH]); 

/* 摄像头使用硬件资源 这里没有用到 */
static LQMT9V034_resource_t LQMT9V034Resource;

/* 摄像头驱动配置结构体 */
camera_device_handle_t cameraDevice = {          
    .resource = &LQMT9V034Resource,
    .ops = &LQMT9V034_ops,
};

/* CSI 配置相关结构体. */
static csi_resource_t csiResource = {   
    .csiBase = CSI,
};

static csi_private_data_t csiPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, 
    .ops = &csi_ops, 
    .privateData = &csiPrivateData,
};


extern void CSI_DriverIRQHandler(void);


/**
  * @brief    CSI中断服务函数
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
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
}


/**
  * @brief    摄像头CSI接口初始化
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
void CSI_PinsInit(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);        
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK,0U);                                                                      
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_CSI_DATA09,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_CSI_DATA08,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_CSI_DATA07,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_CSI_DATA06,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_CSI_DATA05,0U);                                     
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_CSI_DATA04,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_CSI_DATA03,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_CSI_DATA02,0U);    
                                  
    /* CSI MCLK select 120M. */
   /*
    * CSI clock source: 
    * 00 derive clock from osc_clk (24M)
    * 01 derive clock from PLL2 PFD2
    * 10 derive clock from pll3_120M
    * 11 derive clock from PLL3 PFD1
    */
    CLOCK_SetMux(kCLOCK_CsiMux, 2);

    CLOCK_SetDiv(kCLOCK_CsiDiv, 1);
    
}


//camera_config_t cameraConfig;   //摄像头配置结构体

camera_config_t cameraConfig = {
        .pixelFormat   = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
        .bytesPerPixel = APP_BPP,//   每个像素点几个数据
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //分辨率
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //行间隔
        .interface     = kCAMERA_InterfaceGatedClock,                            //摄像机接口类型
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec   = CAMERA_FPS,                                             //fps 
 };
    
    
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
void CSI_CameraInit(uint8_t fps)  
{
    /* 手动产生一个IIC停止信号，防止IIC卡死 */
    PIN_InitConfig(J11, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(K11, PIN_MODE_OUTPUT, 1);
    //GPIOx.Init(CSI_IIC_SDA_GPIO);
    //GPIOx.Init(CSI_IIC_SCL_GPIO);
    
    PIN_Write(K11, 0);
    //GPIOx.Write(CSI_IIC_SCL_GPIO,0);
    Systime.Delayms(5);
    
    //GPIOx.Write(CSI_IIC_SCL_GPIO,1);
    PIN_Write(K11, 1);
    Systime.Delayms(5);
    
    IIC_InitConfig(LPI2C1, 50000);
    
    CSI_PinsInit();
    
    cameraConfig.framePerSec = fps;
    
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //初始化csi
    
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //初始化摄像头
    
    CAMERA_DEVICE_Start(&cameraDevice);                                //启动相机
    
    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++)       //将空帧缓冲区提交到缓冲区队列
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }
    
    //CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据
    
    Systime.Delayms(100);        
}


/**
  * @brief    上位机看图
  *
  * @param    
  *
  * @return   
  *
  * @note     串口传输数据比较慢 用DAP自带的串口时，不要一边调试，一边疯狂发数据，会卡死
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void Test_Camera_Reprot(void)
{
    uint32_t fullCameraBufferAddr;  

    UART_InitConfig(LPUART1, 115200);
    CSI_CameraInit(100);
    //LED_Init();       
    uint8_t count = 0;

    while (1)
    {     

        //摄像头CSI缓存区已满
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }  
        SCB_DisableDCache();
        SCB_EnableDCache();  //刷新D-Cache
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
        CAMERA_RECEIVER_Stop(&cameraReceiver);//发送时间比较长 所以先暂停csi接收 防止图像还没有发送完就刷新了下一帧图像
        
        for(count = 0; count < APP_CAMERA_FRAME_BUFFER_COUNT; count++)    //获取当前缓冲满的数组索引
        {
            if(fullCameraBufferAddr == (uint32_t )csiFrameBuf[count])
            {
                break;
            }
            
        }
        
        UART_PutChar(LPUART1, 0xfe);  //帧头
        UART_PutChar(LPUART1, 0xef);  //帧头

        for(int i = 0; i < APP_CAMERA_HEIGHT; i++)  //分辨率越高 ，出图越慢
        {
//上位机看 7725 RGB565图像 和 神眼灰度图像 
            for(int j = 0; j < APP_CAMERA_WIDTH; j++)
            {
                if(csiFrameBuf[count][i][j].gray[0] == 0xfe )  //防止错误发送帧尾
                {
                    csiFrameBuf[count][i][j].gray[0] = 0xff;  
                }
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[0]); //发送数据
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[1]); //发送数据
            }
        }  
        UART_PutChar(LPUART1, 0xef);  //帧尾
        UART_PutChar(LPUART1, 0xfe);  //帧尾
        
        
        /* 重新启动接收camera数据 */
        CAMERA_RECEIVER_Start(&cameraReceiver);
	SCB_DisableDCache();
        SCB_EnableDCache();  //刷新D-Cache		
        /* 重新启动接收后接收一帧数据 */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }
        /* 将空帧提交到缓冲区 */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
        //LED_ColorReverse(blue);

        Systime.Delayms(1000);
    }
}



