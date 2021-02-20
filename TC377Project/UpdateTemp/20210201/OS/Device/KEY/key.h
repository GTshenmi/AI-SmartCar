/*
 * key.h
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_KEY_KEY_H_
#define OS_DEVICE_KEY_KEY_H_


#include "driver.h"

#define KEY_LONG_DOWN_DELAY (uint8_t)10

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
    uint8_t (*Init)(struct key *self);

    uint8_t KeyShield;                //��������1:���Σ�0:������
    uint8_t Count;                    //������������
    uint8_t Level;                    //���⵱ǰIO��ƽ������1��̧��0
    uint8_t DownLevel;                //����ʱIOʵ�ʵĵ�ƽ
    uint8_t State;                    //����״̬
    uint8_t Event;                    //�����¼�
    uint8_t (*Read)(struct key *self);//��IO��ƽ����
    uint8_t (*Scan)(struct key *self);
    void (*PressedCallBack)(struct key *self);
    void (*LongPressedCallBack)(struct key *self);

    void (*Test)(struct key *self);

    gpiox_t *GPIODevice;

    struct key *Self;
}key_t;

uint8_t KEY_Init(struct key *self);

#endif /* OS_DEVICE_KEY_KEY_H_ */
