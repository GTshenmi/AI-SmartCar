/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_flexio.h"
#include "fsl_flexio_camera.h"
#include "fsl_flexio_camera_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "LQ_FlexCamera.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "include.h"
#include "LQ_FlexioMT9V034.h"
#include "LQ_GPIO.h"
#include "LQ_SCCB.h"
#include "LQ_UART.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
#include "stdio.h"
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);
void FLEXIO_CameraPinInit(FLEXIO_Type *Base, FLEXIO_DN pin);

/**  @brief    FLEXIO ģ�� ע��FLEXIO3 ���ܴ���DMA  */
#define FLEXIO_CAMERA_BASE   FLEXIO1

/**  @brief    FLEXIO ����ͷ8λ���ݹܽ���ʼ�ܽ� ��������ݹܽŰ�FLEXIO�ܽ�˳��˳��  */
#define FLEXIO_CAMERA_DATA_START   FLEXIO1_D00_E3

/**  @brief    FLEXIO ����ͷ PCLK�ܽ�  */
#define FLEXIO_CAMERA_PIX          FLEXIO1_D08_H3

/**  @brief    FLEXIO ����ͷ���źŹܽ�  */
#define FLEXIO_CAMERA_LINE         FLEXIO1_D09_C2

/**  @brief    ����ͷ DMA�������ýṹ�� Ŀ�ĵ�ַ�ʹ��䳤��  */
flexio_camera_transfer_t flexio_rxfer;
flexio_camera_edma_handle_t flexio_camera_edma_handle;
FLEXIO_CAMERA_Type FlexioCamera;
edma_handle_t rxEdmaHandle;

/* ���ڴ����Ҫʹ������ */
AT_DTCRAM_SECTION(uint8_t FlexioImage_Use[Use_FLEXIO_ROWS][Use_FLEXIO_Line]); 

/* ��Ŷ�ֵ��������� */
AT_DTCRAM_SECTION(uint8_t FlexioPixle[Use_FLEXIO_ROWS][Use_FLEXIO_Line]);  

/*!< �Ҷ�����ͷ���� */
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint8_t FlexioFrameBuf[APP_FLEXIO_CAMERA_HEIGHT][APP_FLEXIO_CAMERA_WIDTH], 64);

/**  @brief    FLEXIO CAMERA������ɱ�־λ  */
volatile bool flexio_camera_flag = false;


/*!
  * @brief    ����ͷflexio dma������ɻص�����
  *
  * @param    �û������ϵ
  *
  * @return   
  *
  * @note     flexio dma�������һ֡ͼ�� ����ô˺��� ����־λflexio_camera_flag��1
  *
  * @see      
  *
  * @date     2019/8/23 ������
  */
void callback(FLEXIO_CAMERA_Type *base, flexio_camera_edma_handle_t *handle, status_t status, void *userData)
{
    flexio_camera_flag = true;
	
	/* ���յ�һ֡���� �رճ��ж� �����������ٴ� */
    GPIO_PortDisableInterrupts(GPIO2, 1U << 3);
    /* �û����� */
    
}

/*!
  * @brief    FLEXIO ��������ͷ��ʼ��
  *
  * @param    dma_ch      :flexio camera ʹ�õ�DMAͨ��
  * @param    dataAddress :flexio camera �洢buf�ĵ�ַ
  * @param    len         :flexio camera ���䳤��
  *
  * @return   ��
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/23 ������
  */
void FLEXIO_CameraInit(uint8_t dma_ch, uint32_t dataAddress, uint32_t len)
{
    LQ_SCCB_Init();
      
    /* ����ͷ�Ĵ�����ʼ�� */
    LQMT9V034_SCCBInit();

    /* FLEXIO ����ͬ��ʱ��PCLK �ܽų�ʼ�� */
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, FLEXIO_CAMERA_PIX);
    
    /* FLEXIO ���ź� �ܽų�ʼ�� */
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, FLEXIO_CAMERA_LINE);
    
    /* FLEXIO ��λ���ݲ��� �ܽų�ʼ�� */
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, FLEXIO_CAMERA_DATA_START);
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 1));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 2));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 3));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 4));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 5));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 6));
    FLEXIO_CameraPinInit(FLEXIO_CAMERA_BASE, (FLEXIO_DN)(FLEXIO_CAMERA_DATA_START + 7));
    
    /* FLEXIO ��ʼ�� */
    FlexioCamera.flexioBase           = FLEXIO_CAMERA_BASE;       
    FlexioCamera.datPinStartIdx       = FLEXIO_CAMERA_DATA_START;
    FlexioCamera.pclkPinIdx           = FLEXIO_CAMERA_PIX;
    FlexioCamera.hrefPinIdx           = FLEXIO_CAMERA_LINE;
    FlexioCamera.shifterStartIdx      = 3;
    FlexioCamera.shifterCount         = 1;
    FlexioCamera.timerIdx             = 0;
    
    flexio_camera_config_t flexio_config;
    FLEXIO_CAMERA_GetDefaultConfig(&flexio_config);
    FLEXIO_CAMERA_Init(&FlexioCamera, &flexio_config);
    
    /* �����־λ */
    FLEXIO_CAMERA_ClearStatusFlags(&FlexioCamera, kFLEXIO_CAMERA_RxDataRegFullFlag | kFLEXIO_CAMERA_RxErrorFlag);
    FLEXIO_CAMERA_Enable(&FlexioCamera, true);

    /* ��ʼ��DMA��·������ */
    DMAMUX_Init(DMAMUX);
    
    /* ����DMAͨ��Դ */
    DMAMUX_SetSource(DMAMUX, dma_ch, kDmaRequestMuxFlexIO1Request2Request3); 
   
    /* ʹ��DMAͨ�� */
    DMAMUX_EnableChannel(DMAMUX, dma_ch);
    
    /* ��ʼ�� EDMA ģ�� */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(DMA0, &edma_config);
    EDMA_CreateHandle(&rxEdmaHandle, DMA0, dma_ch);
    
    
    /* ����DMA���. ���ص���������Ϣ��ֵ�� flexio_camera_edma_handle �ṹ�� */
    FLEXIO_CAMERA_TransferCreateHandleEDMA(&FlexioCamera, &flexio_camera_edma_handle, callback, NULL, &rxEdmaHandle);
    
    /* ���ý��յ�ַ �� ���ճ��� */
    flexio_rxfer.dataAddress = dataAddress;
    flexio_rxfer.dataNum     = len;
    
    /* ʹ�ܳ��ж� */
    PIN_Exti(D8, PIN_IRQ_MODE_FALLING);
    
    /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(GPIO2_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(GPIO2_Combined_0_15_IRQn);			          //ʹ��GPIO2_0~15IO���ж�  
    
}



/**
  * @brief    ��ȡ��Ҫʹ�õ�ͼ������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void FLEXIO_Get_Use_Image(void)
{
  
	uint8_t div_h, div_w;
    uint32_t temp_w = 0;
    uint32_t row_start = 0;
    uint32_t lin_start = 0;
    
	div_h = APP_FLEXIO_CAMERA_HEIGHT/Use_FLEXIO_ROWS;
	div_w = APP_FLEXIO_CAMERA_WIDTH/Use_FLEXIO_Line;
    
    /* ������ȡͼ�� */
    if(Use_FLEXIO_ROWS * div_h != APP_FLEXIO_CAMERA_HEIGHT)
    {
        row_start = (APP_FLEXIO_CAMERA_HEIGHT - Use_FLEXIO_ROWS * div_h)/2;
    }
    if(Use_FLEXIO_Line * div_w != APP_FLEXIO_CAMERA_WIDTH)
    {
        lin_start = (APP_FLEXIO_CAMERA_WIDTH - Use_FLEXIO_Line * div_w)/2;       
    }
    for(int i = 0; i < Use_FLEXIO_ROWS; i ++)
    {
        temp_w = lin_start;
        for(int j = 0; j < Use_FLEXIO_Line; j++)
        {  
            FlexioImage_Use[i][j] = FlexioFrameBuf[row_start][temp_w];
            temp_w += div_w;
        }
        row_start += div_h;
    }
      
}


/*!
  * @brief    �����ϱ���λ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/9/24 ���ڶ�
  */
void FLEXIO_CameraReprot(void)
{
  
    UART_PutChar(LPUART1, 0xfe);  //֡ͷ
    UART_PutChar(LPUART1, 0xef);  //֡ͷ
    
    for(int i = 0; i < APP_FLEXIO_CAMERA_HEIGHT; i++)
    {
        for(int j = 0; j < APP_FLEXIO_CAMERA_WIDTH; j++)
        {
            if(FlexioFrameBuf[i][j] == 0xfe )  //��ֹ������֡β
            {
                FlexioFrameBuf[i][j] = 0xff;  
            }
            UART_PutChar(LPUART1, FlexioFrameBuf[i][j]); //��������
            
        }
    }
    UART_PutChar(LPUART1, 0xef);  //֡β
    UART_PutChar(LPUART1, 0xfe);  //֡β

}






/**
  * @brief    ��ֵ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  FlexIO_Get_01_Value(); //ʹ�ô�򷨶�ֵ��
  *
  * @date     2019/6/25 ���ڶ�
  */
void FlexIO_Get_01_Value(void)
{
  int i = 0,j = 0;
  uint8_t Threshold;
  uint32_t  tv=0;
  char txt[16];
  //�ۼ�
  for(i = 0; i <Use_FLEXIO_ROWS; i++)
  {    
      for(j = 0; j <Use_FLEXIO_Line; j++)
      {                            
          tv+=FlexioImage_Use[i][j];   //�ۼ�  
      } 
  }
  Threshold=tv/Use_FLEXIO_ROWS/Use_FLEXIO_Line;        //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
//  Threshold=Threshold*7/10+10;   //�˴���ֵ���ã����ݻ����Ĺ������趨 
  

#ifdef OLED
  sprintf(txt,"%03d",Threshold);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  LCD_P6x8Str(80,0,txt);
#else
  sprintf(txt,"%03d",Threshold);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  TFTSPI_P8X8Str(0,15, txt,u16RED,u16BLUE);
#endif
  
  for(i = 0; i < Use_ROWS; i++)
  {
    for(j = 0; j < Use_Line; j++)
    {                                
      if(FlexioImage_Use[i][j] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        FlexioPixle[i][j] =1;        
      else                                        
        FlexioPixle[i][j] =0;
    }    
  }
}



/**
  * @brief    OLED ��ʾͼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void FlexIO_Draw_Road(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56�� 
  {
#ifdef LQOV7725
    LCD_Set_Pos(24,i/8+1);
#else 
    LCD_Set_Pos(17,i/8+1);
#endif
    for(j=0;j<Use_FLEXIO_Line;j++) 
    { 
      temp=0;
      if(FlexioPixle[0+i][j]) temp|=1;
      if(FlexioPixle[1+i][j]) temp|=2;
      if(FlexioPixle[2+i][j]) temp|=4;
      if(FlexioPixle[3+i][j]) temp|=8;
      if(FlexioPixle[4+i][j]) temp|=0x10;
      if(FlexioPixle[5+i][j]) temp|=0x20;
      if(FlexioPixle[6+i][j]) temp|=0x40;
      if(FlexioPixle[7+i][j]) temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}


/*!
  * @brief    FLEXIO ����ͷ�ܽų�ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/23 ������
  */
void FLEXIO_CameraPinInit(FLEXIO_Type *Base, FLEXIO_DN pin)
{
    switch((uint32_t)Base)
    {
      case (uint32_t)FLEXIO1:
        assert(pin<16);
        switch(pin)
        {
          case 0:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_FLEXIO1_FLEXIO00,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_FLEXIO1_FLEXIO00,0x10B0u); 
            break;
            
          case 1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_FLEXIO1_FLEXIO01,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_FLEXIO1_FLEXIO01,0x10B0u); 
            break;
            
          case 2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_FLEXIO1_FLEXIO02,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_FLEXIO1_FLEXIO02,0x10B0u); 
            break;
            
          case 3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_FLEXIO1_FLEXIO03,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_FLEXIO1_FLEXIO03,0x10B0u); 
            break;
            
          case 4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_FLEXIO1_FLEXIO04,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_FLEXIO1_FLEXIO04,0x10B0u); 
            break;
            
          case 5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_FLEXIO1_FLEXIO05,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_FLEXIO1_FLEXIO05,0x10B0u); 
            break;
            
          case 6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_FLEXIO1_FLEXIO06,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_FLEXIO1_FLEXIO06,0x10B0u); 
            break;
            
          case 7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_FLEXIO1_FLEXIO07,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_FLEXIO1_FLEXIO07,0x10B0u); 
            break;
            
          case 8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_08_FLEXIO1_FLEXIO08,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_08_FLEXIO1_FLEXIO08,0x10B0u); 
            break;
            
          case 9:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_FLEXIO1_FLEXIO09,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_09_FLEXIO1_FLEXIO09,0x10B0u); 
            break;
            
          case 10:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_10_FLEXIO1_FLEXIO10,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_10_FLEXIO1_FLEXIO10,0x10B0u); 
            break;
            
          case 11:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_FLEXIO1_FLEXIO11,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_11_FLEXIO1_FLEXIO11,0x10B0u); 
            break;
            
          case 12:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_26_FLEXIO1_FLEXIO12,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_26_FLEXIO1_FLEXIO12,0x10B0u); 
            break;
            
          case 13:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_27_FLEXIO1_FLEXIO13,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_27_FLEXIO1_FLEXIO13,0x10B0u); 
            break;
            
          case 14:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_28_FLEXIO1_FLEXIO14,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_28_FLEXIO1_FLEXIO14,0x10B0u); 
            break;
            
          case 15:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_29_FLEXIO1_FLEXIO15,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_29_FLEXIO1_FLEXIO15,0x10B0u); 
            break;
        }
        break;
    
      case (uint32_t)FLEXIO2:
        assert(pin<32);
        switch(pin)
        {
          case 0:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_FLEXIO2_FLEXIO00,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_FLEXIO2_FLEXIO00,0x10B0u); 
            break;
            
          case 1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_FLEXIO2_FLEXIO01,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_FLEXIO2_FLEXIO01,0x10B0u); 
            break;
            
          case 2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_FLEXIO2_FLEXIO02,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_FLEXIO2_FLEXIO02,0x10B0u); 
            break;
            
          case 3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_FLEXIO2_FLEXIO03,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_FLEXIO2_FLEXIO03,0x10B0u); 
            break;
            
          case 4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_FLEXIO2_FLEXIO04,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_FLEXIO2_FLEXIO04,0x10B0u); 
            break;
            
          case 5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_FLEXIO2_FLEXIO05,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_FLEXIO2_FLEXIO05,0x10B0u); 
            break;
            
          case 6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_FLEXIO2_FLEXIO06,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_FLEXIO2_FLEXIO06,0x10B0u); 
            break;
            
          case 7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_FLEXIO2_FLEXIO07,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_FLEXIO2_FLEXIO07,0x10B0u); 
            break;
            
          case 8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_FLEXIO2_FLEXIO08,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_FLEXIO2_FLEXIO08,0x10B0u); 
            break;
            
          case 9:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_FLEXIO2_FLEXIO09,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_FLEXIO2_FLEXIO09,0x10B0u); 
            break;
            
          case 10:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_FLEXIO2_FLEXIO10,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_FLEXIO2_FLEXIO10,0x10B0u); 
            break;
            
          case 11:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_FLEXIO2_FLEXIO11,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_FLEXIO2_FLEXIO11,0x10B0u); 
            break;
            
          case 12:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_FLEXIO2_FLEXIO12,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_FLEXIO2_FLEXIO12,0x10B0u); 
            break;
            
          case 13:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_FLEXIO2_FLEXIO13,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_FLEXIO2_FLEXIO13,0x10B0u); 
            break;
            
          case 14:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_FLEXIO2_FLEXIO14,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_FLEXIO2_FLEXIO14,0x10B0u); 
            break;
            
          case 15:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_FLEXIO2_FLEXIO15,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_FLEXIO2_FLEXIO15,0x10B0u); 
            break;
            
          case 16:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_FLEXIO2_FLEXIO16,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_FLEXIO2_FLEXIO16,0x10B0u); 
            break;
            
          case 17:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_FLEXIO2_FLEXIO17,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_FLEXIO2_FLEXIO17,0x10B0u); 
            break;
            
          case 18:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_FLEXIO2_FLEXIO18,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_FLEXIO2_FLEXIO18,0x10B0u); 
            break;
            
          case 19:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_FLEXIO2_FLEXIO19,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_FLEXIO2_FLEXIO19,0x10B0u); 
            break;
            
          case 20:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_FLEXIO2_FLEXIO20,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_FLEXIO2_FLEXIO20,0x10B0u); 
            break;
            
          case 21:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_FLEXIO2_FLEXIO21,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_FLEXIO2_FLEXIO21,0x10B0u); 
            break;
            
          case 22:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_FLEXIO2_FLEXIO22,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_FLEXIO2_FLEXIO22,0x10B0u); 
            break;
            
          case 23:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_FLEXIO2_FLEXIO23,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_FLEXIO2_FLEXIO23,0x10B0u); 
            break;
            
          case 24:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_FLEXIO2_FLEXIO24,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_FLEXIO2_FLEXIO24,0x10B0u); 
            break;
            
          case 25:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_FLEXIO2_FLEXIO25,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_FLEXIO2_FLEXIO25,0x10B0u); 
            break;
            
          case 26:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_FLEXIO2_FLEXIO26,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_FLEXIO2_FLEXIO26,0x10B0u); 
            break;
            
          case 27:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_FLEXIO2_FLEXIO27,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_FLEXIO2_FLEXIO27,0x10B0u); 
            break;
            
          case 28:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_FLEXIO2_FLEXIO28,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_FLEXIO2_FLEXIO28,0x10B0u); 
            break;
            
          case 29:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_FLEXIO2_FLEXIO29,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_13_FLEXIO2_FLEXIO29,0x10B0u); 
            break;
            
          case 30:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_FLEXIO2_FLEXIO30,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_FLEXIO2_FLEXIO30,0x10B0u); 
            break;
            
          case 31:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_FLEXIO2_FLEXIO31,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_FLEXIO2_FLEXIO31,0x10B0u); 
            break;
        }
        break;
    
      case (uint32_t)FLEXIO3:
        assert(pin<32);
        switch(pin)
        {
          case 0:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_FLEXIO3_FLEXIO00,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_FLEXIO3_FLEXIO00,0x10B0u); 
            break;
            
          case 1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_FLEXIO3_FLEXIO01,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_FLEXIO3_FLEXIO01,0x10B0u); 
            break;
            
          case 2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_FLEXIO3_FLEXIO02,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_02_FLEXIO3_FLEXIO02,0x10B0u); 
            break;
            
          case 3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_FLEXIO3_FLEXIO03,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_03_FLEXIO3_FLEXIO03,0x10B0u); 
            break;
            
          case 4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_FLEXIO3_FLEXIO04,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_FLEXIO3_FLEXIO04,0x10B0u); 
            break;
            
          case 5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_FLEXIO3_FLEXIO05,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_05_FLEXIO3_FLEXIO05,0x10B0u); 
            break;
            
          case 6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_FLEXIO3_FLEXIO06,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_FLEXIO3_FLEXIO06,0x10B0u); 
            break;
            
          case 7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_FLEXIO3_FLEXIO07,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_FLEXIO3_FLEXIO07,0x10B0u); 
            break;
            
          case 8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_FLEXIO3_FLEXIO08,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_FLEXIO3_FLEXIO08,0x10B0u); 
            break;
            
          case 9:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_FLEXIO3_FLEXIO09,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_FLEXIO3_FLEXIO09,0x10B0u); 
            break;
            
          case 10:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_FLEXIO3_FLEXIO10,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_FLEXIO3_FLEXIO10,0x10B0u); 
            break;
            
          case 11:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_FLEXIO3_FLEXIO11,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_FLEXIO3_FLEXIO11,0x10B0u); 
            break;
            
          case 12:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_FLEXIO3_FLEXIO12,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_FLEXIO3_FLEXIO12,0x10B0u); 
            break;
            
          case 13:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_FLEXIO3_FLEXIO13,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_FLEXIO3_FLEXIO13,0x10B0u); 
            break;
            
          case 14:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_FLEXIO3_FLEXIO14,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_FLEXIO3_FLEXIO14,0x10B0u); 
            break;
            
          case 15:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_FLEXIO3_FLEXIO15,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_FLEXIO3_FLEXIO15,0x10B0u); 
            break;
            
          case 16:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_FLEXIO3_FLEXIO16,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_FLEXIO3_FLEXIO16,0x10B0u); 
            break;
            
          case 17:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_FLEXIO3_FLEXIO17,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_FLEXIO3_FLEXIO17,0x10B0u); 
            break;
            
          case 18:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_FLEXIO3_FLEXIO18,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_FLEXIO3_FLEXIO18,0x10B0u); 
            break;
            
          case 19:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_FLEXIO3_FLEXIO19,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_FLEXIO3_FLEXIO19,0x10B0u); 
            break;
            
          case 20:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_FLEXIO3_FLEXIO20,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_FLEXIO3_FLEXIO20,0x10B0u); 
            break;
            
          case 21:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_FLEXIO3_FLEXIO21,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_FLEXIO3_FLEXIO21,0x10B0u); 
            break;
            
          case 22:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_FLEXIO3_FLEXIO22,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_FLEXIO3_FLEXIO22,0x10B0u); 
            break;
            
          case 23:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_FLEXIO3_FLEXIO23,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_FLEXIO3_FLEXIO23,0x10B0u); 
            break;
            
          case 24:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_FLEXIO3_FLEXIO24,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_FLEXIO3_FLEXIO24,0x10B0u); 
            break;
            
          case 25:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_FLEXIO3_FLEXIO25,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_FLEXIO3_FLEXIO25,0x10B0u); 
            break;
            
          case 26:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_FLEXIO3_FLEXIO26,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_FLEXIO3_FLEXIO26,0x10B0u); 
            break;
            
          case 27:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_FLEXIO3_FLEXIO27,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_FLEXIO3_FLEXIO27,0x10B0u); 
            break;
            
          case 28:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_FLEXIO3_FLEXIO28,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_FLEXIO3_FLEXIO28,0x10B0u); 
            break;
            
          case 29:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_FLEXIO3_FLEXIO29,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_13_FLEXIO3_FLEXIO29,0x10B0u); 
            break;
            
          case 30:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_FLEXIO3_FLEXIO30,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_FLEXIO3_FLEXIO30,0x10B0u); 
            break;
            
          case 31:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_FLEXIO3_FLEXIO31,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_FLEXIO3_FLEXIO31,0x10B0u); 
            break;
        }
        break;
    }


}
