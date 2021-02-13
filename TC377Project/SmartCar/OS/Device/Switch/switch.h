/*
 * switch.h
 *
 *  Created on: 2021��1��8��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SWITCH_SWITCH_H_
#define OS_DEVICE_SWITCH_SWITCH_H_

#include "driver.h"

typedef enum
{
    Switch_OFF,Switch_ON
}switch_state_t;

typedef struct switchx
{
        void (*Init)(struct switchx *self);
        switch_state_t (*Read)(struct switchx *self);

        uint8_t Level;                    //���⵱ǰIO��ƽ������1��̧��0
        uint8_t DownLevel;                //����ʱIOʵ�ʵĵ�ƽ
        switch_state_t State;                    //����״̬

        gpiox_t *GPIODevice;

        struct switchx *Self;

}switch_t;

void Switch_Init(struct switchx *self);

#endif /* OS_DEVICE_SWITCH_SWITCH_H_ */
