/*
 * capture.h
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_CAPTURE_CAPTURE_H_
#define OS_DEVICE_CAPTURE_CAPTURE_H_

#include "driver.h"

typedef enum
{
    Cap_Busy,Cap_Free,
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

  public

     uint8_t (*Init)(struct capture *self,uint8_t fps);
     uint8_t (*Read)(struct capture *self);
     uint8_t (*GetState)(struct capture *self);

     uint8_t (*Filter)(struct capture *self);

     void (*Show)(struct capture *self);
     void (*Report)(struct capture *self);

     void (*Test)(struct capture *self);

     uint8_t IMG_Hight;
     uint8_t IMG_Weight;

     uint8_t **IMG_Data;

     cap_state_t State;
     
}capture_t;

uint8_t Cap_Init(struct capture *self,uint8_t fps);

#endif /* OS_DEVICE_CAPTURE_CAPTURE_H_ */
