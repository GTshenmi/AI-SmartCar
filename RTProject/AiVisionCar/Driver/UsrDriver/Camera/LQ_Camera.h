#ifndef __LQ_SGP18TEST_H
#define __LQ_SGP18TEST_H

#include "LQ_CSI.h"
/* ���ڴ����Ҫʹ������ */
extern volatile pixel_t Image_Use[IMAGEH][IMAGEW]; 

/* ���ڴ��浱ǰ֡�ĵ�ַ */
extern uint32_t fullCameraBufferAddr;

/* ��Ŷ�ֵ��������� */
extern volatile pixel_t Pixle[IMAGEH][IMAGEW];

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
void Get_Use_Image(void);




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
void Draw_Road(void);


/**
  * @brief    ��ֵ��
  *
  * @param    mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
  *
  * @return   
  *
  * @note     
  *
  * @example  Get_01_Value(0); //ʹ�ô�򷨶�ֵ��
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_01_Value(uint8_t mode);

/*************************************************************** 
* 
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����	1
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7) �����g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
������ղ�����  https://blog.csdn.net/kk55guang2/article/details/78475414
                https://blog.csdn.net/kk55guang2/article/details/78490069
				https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
***************************************************************/ 
int GetOSTU(pixel_t tmImage[Use_ROWS][Use_Line]);

#endif

