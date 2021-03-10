/*
 * capture.h
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_CAPTURE_CAPTURE_H_
#define OS_DEVICE_CAPTURE_CAPTURE_H_

#include "driver.h"
#include "image.h"

typedef enum
{
    Capture_Stop,Capture_Start,Capture_Busy,Capture_Fin,
}cap_state_t;

typedef struct capture
{
  private

     uint8_t (*__Init__)(void *config);
     void *__InitConfig__;
     uint8_t (*__Start__)(void);
     uint8_t (*__Stop__)(void);
     uint8_t (*__GetState__)(void);
     uint8_t (*__Read__)(void);

     uartx_t *ReportUartn;
     uint32_t Reg;

  public

     uint8_t (*Init)(struct capture *self,uint8_t fps);
     uint8_t (*Start)(struct capture *self);
     uint8_t (*Stop)(struct capture *self);
   /*
    * flags == 0 : ������ȡ��ֱ��ͼƬȫ������ImageCache�Ž�������
    * flags == 1 : ��ѯ��ȡ���ú��������Ͷ�ȡ�������ȴ�ͼƬ��ȡ���,ͨ��Capture.GetState()��ѯ��״̬ΪCapture_Finʱ,��ʾ��ȡ��ɡ�
       *                          ��ȡ��ɺ���Ҫ����Capture.ClearFinFlag()�����ɱ�־���ú������ܼ�����ȡ,�����ȡ��Ч��
    */
     image_t (*Read)(struct capture *self,uint16_t flags);
     void (*ClearFinFlag)(struct capture *self);

     cap_state_t (*GetState)(struct capture *self);

     uint8_t (*Filter)(struct capture *self);

     void (*Show)(struct capture *self,image_t image,uint8_t flags);
     void (*Report)(struct capture *self,image_t image);

     void (*Test)(struct capture *self);

     image_t ImageCache;

     cap_state_t State;

}capture_t;

uint8_t Cap_Init(struct capture *self,uint8_t fps);

#endif /* OS_DEVICE_CAPTURE_CAPTURE_H_ */
