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
#include "fsl_common.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "LQ_DMA.h"



/*!
  * @brief    DMA������ɱ�־λ
  *
  * @note     
  */ 
bool g_Transfer_Done = false;


/*!
  * @brief    EDMA�û��ص���������DMA�������ʱ������øú���
  *
  * @param    �����û������ϵ
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/26 ����һ
  */
void EDMA_Callback(edma_handle_t *handle, void *param,  bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}


/*!
  * @brief    DMA������
  *
  * @note     
  */ 
edma_handle_t g_EDMA_Handle; 


/*!
  * @brief    DMA�������ýṹ��
  *
  * @note     
  */ 
edma_transfer_config_t transferConfig;





/*!
  * @brief    DMA�ڴ� TO �ڴ��ʼ��
  *
  * @param    DMA_channel:  DMAͨ����  0-31
  * @param    srcAddr    :  Դ��ַ
  * @param    destAddr   :  Ŀ�ĵ�ַ
  * @param    len        :  ���䳤��
  *
  * @return   
  *
  * @note     ʹ��DMA����ʱ���Ҫ�Ѵ�������ݷ���NOCACHE�����У���ȻҪ�ֶ�ˢ��D-cache
  *
  * @see      
  *
  * @date     2019/8/26 ����һ
  */
void DMA_MemoryToMemoryInit(uint8_t DMA_channel, uint32_t srcAddr, uint32_t destAddr, uint32_t len)
{
    /* ��ʼ��DMA��·������ */
    DMAMUX_Init(DMAMUX);
    
    /*���� DMA ͨ��һֱ���ڻ״̬*/
    DMAMUX_EnableAlwaysOn(DMAMUX, DMA_channel, true);

    /* ʹ��DMAͨ�� */
    DMAMUX_EnableChannel(DMAMUX, DMA_channel);
    
    /* ��ʼ�� EDMA ģ�� */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(DMA0, &edma_config);
    
    
    /* ��ʼ���������ýṹ�� */
    transferConfig.srcAddr = (uint32_t)srcAddr;                //Դ��ַ
    transferConfig.srcOffset = 1;                              //Դ��ַƫ��
    transferConfig.srcTransferSize = kEDMA_TransferSize1Bytes; //Դ���ݿ��

    transferConfig.destAddr = (uint32_t)destAddr;              //Ŀ�ĵ�ַ
    transferConfig.destOffset = 1;                             //Ŀ�ĵ�ַƫ��
    transferConfig.destTransferSize = kEDMA_TransferSize1Bytes;//Ŀ�����ݿ��
   
    transferConfig.minorLoopBytes = 1;                         //��ѭ�������ֽ���
    transferConfig.majorLoopCounts = len;                      //��ѭ������ֵ
    
    /* ���� eDMA ��� */
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, DMA_channel);
    
    /* ���ô�����ɻص����� */
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    
    /* �ύ eDAM �������� */
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    
    /* �������� */
    EDMA_StartTransfer(&g_EDMA_Handle);

}


