/*
 * capture.h
 *
 *  Created on: 2020年12月14日
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
    * flags == 0 : 阻塞读取，直到图片全部存入ImageCache才结束函数
    * flags == 1 : 查询读取，该函数仅发送读取命令，不会等待图片读取完成,通过Capture.GetState()查询到状态为Capture_Fin时,表示读取完成。
       *                          读取完成后，需要调用Capture.ClearFinFlag()清除完成标志，该函数才能继续读取,否则读取无效。
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
