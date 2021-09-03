/*-----------------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��dev.env.��IAR8.30.1�����ϰ汾
--------------------------------------------------------------------*/

#include "LQ_CSI.h"

#include "LQ_MT9V034M.h"

AT_DTCRAM_SECTION(volatile pixel_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH]); 

/* ����ͷʹ��Ӳ����Դ ����û���õ� */
static LQMT9V034_resource_t LQMT9V034Resource;

/* ����ͷ�������ýṹ�� */
camera_device_handle_t cameraDevice = {          
    .resource = &LQMT9V034Resource,
    .ops = &LQMT9V034_ops,
};

/* CSI ������ؽṹ��. */
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
  * @brief    CSI�жϷ�����
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
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
}


/**
  * @brief    ����ͷCSI�ӿڳ�ʼ��
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


//camera_config_t cameraConfig;   //����ͷ���ýṹ��

camera_config_t cameraConfig = {
        .pixelFormat   = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
        .bytesPerPixel = APP_BPP,//   ÿ�����ص㼸������
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //�ֱ���
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //�м��
        .interface     = kCAMERA_InterfaceGatedClock,                            //������ӿ�����
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec   = CAMERA_FPS,                                             //fps 
 };
    
    
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
void CSI_CameraInit(uint8_t fps)  
{
    /* �ֶ�����һ��IICֹͣ�źţ���ֹIIC���� */
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
    
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //��ʼ��csi
    
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //��ʼ������ͷ
    
    CAMERA_DEVICE_Start(&cameraDevice);                                //�������
    
    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++)       //����֡�������ύ������������
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }
    
    //CAMERA_RECEIVER_Start(&cameraReceiver);   // ��������camera����
    
    Systime.Delayms(100);        
}


/**
  * @brief    ��λ����ͼ
  *
  * @param    
  *
  * @return   
  *
  * @note     ���ڴ������ݱȽ��� ��DAP�Դ��Ĵ���ʱ����Ҫһ�ߵ��ԣ�һ�߷�����ݣ��Ῠ��
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
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

        //����ͷCSI����������
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }  
        SCB_DisableDCache();
        SCB_EnableDCache();  //ˢ��D-Cache
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//��������������ύ���������
        CAMERA_RECEIVER_Stop(&cameraReceiver);//����ʱ��Ƚϳ� ��������ͣcsi���� ��ֹͼ��û�з������ˢ������һ֡ͼ��
        
        for(count = 0; count < APP_CAMERA_FRAME_BUFFER_COUNT; count++)    //��ȡ��ǰ����������������
        {
            if(fullCameraBufferAddr == (uint32_t )csiFrameBuf[count])
            {
                break;
            }
            
        }
        
        UART_PutChar(LPUART1, 0xfe);  //֡ͷ
        UART_PutChar(LPUART1, 0xef);  //֡ͷ

        for(int i = 0; i < APP_CAMERA_HEIGHT; i++)  //�ֱ���Խ�� ����ͼԽ��
        {
//��λ���� 7725 RGB565ͼ�� �� ���ۻҶ�ͼ�� 
            for(int j = 0; j < APP_CAMERA_WIDTH; j++)
            {
                if(csiFrameBuf[count][i][j].gray[0] == 0xfe )  //��ֹ������֡β
                {
                    csiFrameBuf[count][i][j].gray[0] = 0xff;  
                }
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[0]); //��������
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[1]); //��������
            }
        }  
        UART_PutChar(LPUART1, 0xef);  //֡β
        UART_PutChar(LPUART1, 0xfe);  //֡β
        
        
        /* ������������camera���� */
        CAMERA_RECEIVER_Start(&cameraReceiver);
	SCB_DisableDCache();
        SCB_EnableDCache();  //ˢ��D-Cache		
        /* �����������պ����һ֡���� */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }
        /* ����֡�ύ�������� */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
        //LED_ColorReverse(blue);

        Systime.Delayms(1000);
    }
}



