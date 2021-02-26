/*
 * key.h
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_KEY_KEY_H_
#define OS_DEVICE_KEY_KEY_H_


#include "platform.h"

#define KEY_LONG_DOWN_DELAY (uint8_t)10

#define KEY_BITS_DOWNLEVEL_MASK 0x00000100
#define KEY_BITS_LEVEL_MASK     0x00000200
#define KEY_BITS_COUNT_MASK     0x000000ff
#define KEY_BITS_SHIELD_MASK    0x00000400
#define KeySetLevel(state)

typedef enum _KEY_STATUS_LIST
{
    KEY_NULL = 0x00,
    KEY_SURE = 0x01,
    KEY_UP   = 0x02,
    KEY_DOWN = 0x04,
    KEY_LONG = 0x08,
}KEY_STATUS_LIST;

typedef struct key
{
    private

        void *GPIOn;

        uint8_t KeyShield;                //按键屏蔽1:屏蔽，0:不屏蔽
        uint8_t Count;                    //按键长按计数
        uint8_t Level;                    //虚拟当前IO电平，按下1，抬起0
        uint8_t DownLevel;                //按下时IO实际的电平

        uint8_t State;                    //按键状态
        uint8_t Event;                    //按键事件

        void *Argv;
        uint16_t Argc;

        uint8_t (*ReadIOLevel)(struct key *self);/*读IO电平函数*/
        uint8_t (*Read)(struct key *self);

    public

        uint8_t (*Init)(struct key *self);
        uint8_t (*Scan)(struct key *self);

        uint8_t (*GetState)(struct key *self);                   /*KEY_NULL:无按键按下  KEY_DOWN:按键按下一次  KEY_LONG：长按 KEY_SURE:按键按下确认（消抖过程）*/
        uint8_t (*GetEvent)(struct key *self);                   /*KEY_NULL:无按键按下  KEY_DOWN:按键按下一次  KEY_LONG：长按*/
        uint8_t (*Is_Shield)(struct key *self);

        void (*SetShield)(struct key *self,bool is_shield);      /*按键屏蔽使能 */
        void (*SetDownLevel)(struct key *self,uint8_t downlevel);/*设置按键按下时IO口的实际电平 */

        void (*PressedCallBack)(struct key *self,void *argv,uint16_t argc);    /*回调函数(按下一次)*/
        void (*LongPressedCallBack)(struct key *self,void *argv,uint16_t argc);/*回调函数(长按)*/

        void (*BindUsrData)(struct key *self,void *argv,uint16_t argc);

        void (*Test)(struct key *self);

        struct key *Self;
}key_t;

uint8_t KEY_Init(struct key *self);

#endif /* OS_DEVICE_KEY_KEY_H_ */
