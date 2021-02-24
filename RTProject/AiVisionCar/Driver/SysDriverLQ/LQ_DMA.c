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
#include "fsl_common.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "LQ_DMA.h"



/*!
  * @brief    DMA传输完成标志位
  *
  * @note     
  */ 
bool g_Transfer_Done = false;


/*!
  * @brief    EDMA用户回调函数，当DMA传输结束时，会调用该函数
  *
  * @param    参数用户无需关系
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/26 星期一
  */
void EDMA_Callback(edma_handle_t *handle, void *param,  bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}


/*!
  * @brief    DMA传输句柄
  *
  * @note     
  */ 
edma_handle_t g_EDMA_Handle; 


/*!
  * @brief    DMA传输配置结构体
  *
  * @note     
  */ 
edma_transfer_config_t transferConfig;





/*!
  * @brief    DMA内存 TO 内存初始化
  *
  * @param    DMA_channel:  DMA通道号  0-31
  * @param    srcAddr    :  源地址
  * @param    destAddr   :  目的地址
  * @param    len        :  传输长度
  *
  * @return   
  *
  * @note     使用DMA传输时最好要把传输的数据放在NOCACHE区域中，不然要手动刷新D-cache
  *
  * @see      
  *
  * @date     2019/8/26 星期一
  */
void DMA_MemoryToMemoryInit(uint8_t DMA_channel, uint32_t srcAddr, uint32_t destAddr, uint32_t len)
{
    /* 初始化DMA多路复用器 */
    DMAMUX_Init(DMAMUX);
    
    /*设置 DMA 通道一直处于活动状态*/
    DMAMUX_EnableAlwaysOn(DMAMUX, DMA_channel, true);

    /* 使能DMA通道 */
    DMAMUX_EnableChannel(DMAMUX, DMA_channel);
    
    /* 初始化 EDMA 模块 */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(DMA0, &edma_config);
    
    
    /* 初始化传输配置结构体 */
    transferConfig.srcAddr = (uint32_t)srcAddr;                //源地址
    transferConfig.srcOffset = 1;                              //源地址偏移
    transferConfig.srcTransferSize = kEDMA_TransferSize1Bytes; //源数据宽度

    transferConfig.destAddr = (uint32_t)destAddr;              //目的地址
    transferConfig.destOffset = 1;                             //目的地址偏移
    transferConfig.destTransferSize = kEDMA_TransferSize1Bytes;//目的数据宽度
   
    transferConfig.minorLoopBytes = 1;                         //次循环传输字节数
    transferConfig.majorLoopCounts = len;                      //主循环计数值
    
    /* 创建 eDMA 句柄 */
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, DMA_channel);
    
    /* 设置传输完成回调函数 */
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    
    /* 提交 eDAM 传输请求 */
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    
    /* 启动传输 */
    EDMA_StartTransfer(&g_EDMA_Handle);

}


