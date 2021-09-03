/*!
  * @file     LQ_FlexCamera.h
  *
  * @brief    摄像头FLEXIO驱动
  *
  * @author   LQ-005
  *
  * @note     
  *
  * @version  
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     
  */ 
#ifndef __LQFLECAMERA_H__
#define __LQFLECAMERA_H__

/**  @brief    FLEXIO 通道  */
typedef enum
{
    FLEXIO1_D00_E3 = 0,    /*!< GPIO_EMC_00 */
    FLEXIO1_D01_F3    ,    /*!< GPIO_EMC_01 */
    FLEXIO1_D02_F4    ,    /*!< GPIO_EMC_02 */
    FLEXIO1_D03_G4    ,    /*!< GPIO_EMC_03 */
    FLEXIO1_D04_F2    ,    /*!< GPIO_EMC_04 */
    FLEXIO1_D05_G5    ,    /*!< GPIO_EMC_05 */
    FLEXIO1_D06_H5    ,    /*!< GPIO_EMC_06 */
    FLEXIO1_D07_H4    ,    /*!< GPIO_EMC_07 */
    FLEXIO1_D08_H3    ,    /*!< GPIO_EMC_08 */
    FLEXIO1_D09_C2    ,    /*!< GPIO_EMC_09 */
    FLEXIO1_D10_G1    ,    /*!< GPIO_EMC_10 */
    FLEXIO1_D11_G3    ,    /*!< GPIO_EMC_11 */
    FLEXIO1_D12_B3    ,    /*!< GPIO_EMC_26 */
    FLEXIO1_D13_A2    ,    /*!< GPIO_EMC_27 */
    FLEXIO1_D14_D1    ,    /*!< GPIO_EMC_28 */
    FLEXIO1_D15_E1    ,    /*!< GPIO_EMC_29 */
    
    
    FLEXIO2_D00_D7 = 0,    /*!< GPIO_B0_00 */
    FLEXIO2_D01_E7    ,    /*!< GPIO_B0_01 */
    FLEXIO2_D02_E8    ,    /*!< GPIO_B0_02 */
    FLEXIO2_D03_D8    ,    /*!< GPIO_B0_03 */
    FLEXIO2_D04_C8    ,    /*!< GPIO_B0_04 */
    FLEXIO2_D05_B8    ,    /*!< GPIO_B0_05 */
    FLEXIO2_D06_A8    ,    /*!< GPIO_B0_06 */
    FLEXIO2_D07_A9    ,    /*!< GPIO_B0_07 */
    FLEXIO2_D08_B9    ,    /*!< GPIO_B0_08 */
    FLEXIO2_D09_C9    ,    /*!< GPIO_B0_09 */
    FLEXIO2_D10_D9    ,    /*!< GPIO_B0_10 */
    FLEXIO2_D11_A10    ,   /*!< GPIO_B0_11 */
    FLEXIO2_D12_C10    ,   /*!< GPIO_B0_12 */
    FLEXIO2_D13_D10    ,   /*!< GPIO_B0_13 */
    FLEXIO2_D14_E10    ,   /*!< GPIO_B0_14 */
    FLEXIO2_D15_E11    ,   /*!< GPIO_B0_15 */
    FLEXIO2_D16_A11    ,   /*!< GPIO_B1_00 */
    FLEXIO2_D17_B11   ,    /*!< GPIO_B1_01 */
    FLEXIO2_D18_C11   ,    /*!< GPIO_B1_02 */
    FLEXIO2_D19_D11   ,    /*!< GPIO_B1_03 */
    FLEXIO2_D20_E12   ,    /*!< GPIO_B1_04 */
    FLEXIO2_D21_D12   ,    /*!< GPIO_B1_05 */
    FLEXIO2_D22_C12   ,    /*!< GPIO_B1_06 */
    FLEXIO2_D23_B12   ,    /*!< GPIO_B1_07 */
    FLEXIO2_D24_A12   ,    /*!< GPIO_B1_08 */
    FLEXIO2_D25_A13   ,    /*!< GPIO_B1_09 */
    FLEXIO2_D26_B13   ,    /*!< GPIO_B1_10 */
    FLEXIO2_D27_C13    ,   /*!< GPIO_B1_11 */
    FLEXIO2_D28_D13    ,   /*!< GPIO_B1_12 */
    FLEXIO2_D29_D14    ,   /*!< GPIO_B1_13 */
    FLEXIO2_D30_C14    ,   /*!< GPIO_B1_14 */
    FLEXIO2_D31_B14    ,   /*!< GPIO_B1_15 */
    
    FLEXIO3_D00_J11 = 0,   /*!< GPIO_AD_B1_00 */
    FLEXIO3_D01_K11    ,   /*!< GPIO_AD_B1_01 */
    FLEXIO3_D02_L11   ,    /*!< GPIO_AD_B1_02 */
    FLEXIO3_D03_M12   ,    /*!< GPIO_AD_B1_03 */
    FLEXIO3_D04_L12   ,    /*!< GPIO_AD_B1_04 */
    FLEXIO3_D05_K12   ,    /*!< GPIO_AD_B1_05 */
    FLEXIO3_D06_J12   ,    /*!< GPIO_AD_B1_06 */
    FLEXIO3_D07_K10   ,    /*!< GPIO_AD_B1_07 */
    FLEXIO3_D08_H13   ,    /*!< GPIO_AD_B1_08 */
    FLEXIO3_D09_M13   ,    /*!< GPIO_AD_B1_09 */
    FLEXIO3_D10_L13   ,    /*!< GPIO_AD_B1_10 */
    FLEXIO3_D11_J13    ,   /*!< GPIO_AD_B1_11 */
    FLEXIO3_D12_H12    ,   /*!< GPIO_AD_B1_12 */
    FLEXIO3_D13_H11    ,   /*!< GPIO_AD_B1_13 */
    FLEXIO3_D14_G12    ,   /*!< GPIO_AD_B1_14 */
    FLEXIO3_D15_J14    ,   /*!< GPIO_AD_B1_15 */
    FLEXIO3_D16_A11    ,   /*!< GPIO_B1_00 */
    FLEXIO3_D17_B11   ,    /*!< GPIO_B1_01 */
    FLEXIO3_D18_C11   ,    /*!< GPIO_B1_02 */
    FLEXIO3_D19_D11   ,    /*!< GPIO_B1_03 */
    FLEXIO3_D20_E12   ,    /*!< GPIO_B1_04 */
    FLEXIO3_D21_D12   ,    /*!< GPIO_B1_05 */
    FLEXIO3_D22_C12   ,    /*!< GPIO_B1_06 */
    FLEXIO3_D23_B12   ,    /*!< GPIO_B1_07 */
    FLEXIO3_D24_A12   ,    /*!< GPIO_B1_08 */
    FLEXIO3_D25_A13   ,    /*!< GPIO_B1_09 */
    FLEXIO3_D26_B13   ,    /*!< GPIO_B1_10 */
    FLEXIO3_D27_C13    ,   /*!< GPIO_B1_11 */
    FLEXIO3_D28_D13    ,   /*!< GPIO_B1_12 */
    FLEXIO3_D29_D14    ,   /*!< GPIO_B1_13 */
    FLEXIO3_D30_C14    ,   /*!< GPIO_B1_14 */
    FLEXIO3_D31_B14    ,   /*!< GPIO_B1_15 */

}FLEXIO_DN;

#include "LQ_FlexioMT9V034.h"
#include "include.h"
#include "fsl_flexio.h"
#include "fsl_flexio_camera.h"
#include "fsl_flexio_camera_edma.h"
#include "fsl_edma.h"

/*!< flexio 原始数组*/
extern uint8_t FlexioFrameBuf[APP_FLEXIO_CAMERA_HEIGHT][APP_FLEXIO_CAMERA_WIDTH];

/* 用于存放需要使用数据 */
extern uint8_t FlexioImage_Use[Use_FLEXIO_ROWS][Use_FLEXIO_Line]; 

/* 存放二值化后的数据 */
extern uint8_t FlexioPixle[Use_FLEXIO_ROWS][Use_FLEXIO_Line];  



/**  @brief    摄像头 DMA传输配置结构体 目的地址和传输长度  */
extern flexio_camera_transfer_t flexio_rxfer;
extern flexio_camera_edma_handle_t flexio_camera_edma_handle;
extern FLEXIO_CAMERA_Type FlexioCamera;

/**  @brief    FLEXIO CAMERA传输完成标志位  */
extern volatile bool flexio_camera_flag;



/*!
  * @brief    FLEXIO 驱动摄像头初始化
  *
  * @param    dma_ch      :flexio camera 使用的DMA通道
  * @param    dataAddress :flexio camera 存储buf的地址
  * @param    len         :flexio camera 传输长度
  *
  * @return   无
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/23 星期五
  */
void FLEXIO_CameraInit(uint8_t dma_ch, uint32_t dataAddress, uint32_t len);

/**
  * @brief    获取需要使用的图像数组
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void FLEXIO_Get_Use_Image(void);


/**
  * @brief    二值化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  FlexIO_Get_01_Value(); //使用平均阈值二值化
  *
  * @date     2019/6/25 星期二
  */
void FlexIO_Get_01_Value(void);


/**
  * @brief    OLED 显示图像
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void FlexIO_Draw_Road(void);



#endif 



