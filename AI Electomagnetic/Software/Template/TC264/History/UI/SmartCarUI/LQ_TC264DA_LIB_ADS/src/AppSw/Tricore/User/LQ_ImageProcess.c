/*!
 * @file     LQ_ImageProcess.c
 *
 * @brief
 *
 * @company  �����������ܿƼ�
 *
 * @author   LQ-005
 *
 * @note     Tab�� 4���ո�
 *
 * @version  V1.0
 *
 * @par URL  http://shop36265907.taobao.com
 *           http://www.lqist.cn
 *
 * @date     2020��6��9��
 */

#include <LQ_CAMERA.h>
#include <LQ_DMA.h>
#include <LQ_GPIO_LED.h>
//#include <LQ_TFT18.h>

/*!
 * @brief    ͼ�����ݴ���
 *
 * @param    ��
 *
 * @return   ������λ��
 *
 * @note     ��
 *
 * @see
 *
 * @date     2020/4/28
 */
void ImageProcess (void)
{
    if (Camera_Flag == 2)
    {

        /* ��ȡ����ʹ�õ����� */
        Get_Use_Image();

        /* �������ͷ�ɼ���ɱ�־λ  �����������򲻻��ٴβɼ����� */
        Camera_Flag = 0;

        /* ��ֵ�� */
        Get_01_Value(2);

//		//TFT1.8��̬��ʾ����ͷԭʼͼ��
//		TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
//
//		// ��ʾ��ֵ��ͼ��
//		TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Pixle);

    }
}
