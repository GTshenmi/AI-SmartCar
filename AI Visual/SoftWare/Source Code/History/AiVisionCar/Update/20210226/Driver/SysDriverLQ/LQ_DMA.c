/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC3xxxA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
�������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC3xxxA
��Crystal�� 20.000Mhz
��SYS PLL�� 300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include "LQ_DMA.h"

#include <IfxPort_reg.h>


#include "LQ_GPIO.h"

//��װ�жϷ������������ź����ȼ�
IFX_INTERRUPT(DMA_IRQHandler, DMA_VECTABNUM, DMA_PRIORITY);

//��װ�жϷ�����
void DMA_IRQHandler(void)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
//	IfxCpu_enableInterrupts();
	DMA_CameraIRQ(PIN_INT2_PRIORITY);
}


/**
 * ����ͷ DMA ���ýṹ��
 */
Dma_Camera_t g_DmaCameraLinkedList;

/** ����ͷ ���ݲɼ���ɱ�־λ */
volatile unsigned char Camera_Flag = 0;




/*!
  * @brief    ����ͷDMA�����ʼ��
  *
  * @param    srcStartAddr   ��Դ��ַ IO��ַ
  * @param    dstStartAddr   ��Ŀ�ĵ�ַ ����ͷ���ݻ�����
  * @param    channel        ��DMA����ͨ�� 0-47      ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��
  *
  * @return   ��
  *
  * @note     ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��    һ��ͼ��Ҫ��������DMA���䣬��˻ᴥ������DMA�ж�
  *
  * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
  *
  * @date     2020/4/8
  */
void DMA_CameraInitConfig(unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel)
{
	/* �ر��ж� */
	boolean        interruptState = IfxCpu_disableInterrupts();

	/* create module config */
	IfxDma_Dma_Config        dmaConfig;
	IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);

	/* initialize module */
	IfxDma_Dma               dma;
	IfxDma_Dma_initModule(&dma, &dmaConfig);

	/* initial configuration for all channels */
	IfxDma_Dma_ChannelConfig cfg;
	IfxDma_Dma_initChannelConfig(&cfg, &dma);

//	cfg.busPriority   = IfxDma_ChannelBusPriority_high;                    //���ȼ����
	cfg.requestMode   = IfxDma_ChannelRequestMode_oneTransferPerRequest;   //��������һ�δ���
	cfg.moveSize      = IfxDma_ChannelMoveSize_8bit;                       //һ��move 8bit
	cfg.shadowControl = IfxDma_ChannelShadow_none;                         //����ģʽ
	cfg.operationMode = IfxDma_ChannelOperationMode_continuous;            //������ɼ�������
	cfg.hardwareRequestEnabled = TRUE;                                     //ʹ��Ӳ����������
	cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), srcStartAddr);  //����Դ��ַ
	cfg.sourceCircularBufferEnabled = TRUE;                                      //����Դ��ַ����
	cfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_none;
	cfg.channelId     = channel;                                                 //����DMAͨ��
	cfg.channelInterruptEnabled = TRUE;                                          //ʹ��ͨ���ж�
	cfg.channelInterruptPriority = DMA_PRIORITY;                                 //DMA�ж����ȼ�
	cfg.channelInterruptTypeOfService = DMA_VECTABNUM;


	/* ���ô�������� */
	cfg.destinationAddress =  IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), dstStartAddr);

	/* address to next transaction set */
	cfg.shadowAddress = 0;

	cfg.transferCount = 11280;

	/* transfer first transaction set into DMA channel */
	IfxDma_Dma_initChannel(&g_DmaCameraLinkedList.chn, &cfg);

	/* transfer into linked list storage */
	IfxDma_Dma_initLinkedListEntry((void *)&g_DmaCameraLinkedList.linkedList[0], &cfg);

	IfxCpu_Irq_installInterruptHandler((void*)DMA_IRQHandler, DMA_PRIORITY);

	/* clear service request flag */
	(IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

	IfxDma_clearChannelInterrupt(&MODULE_DMA, channel);

	IfxDma_disableChannelTransaction(&MODULE_DMA, channel);

	/* �����ж� */
	IfxCpu_restoreInterrupts(interruptState);
}




