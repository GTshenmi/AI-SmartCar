/*
 * key.h
 *
 *  Created on: 2020��12��14��
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

        uint8_t KeyShield;                //��������1:���Σ�0:������
        uint8_t Count;                    //������������
        uint8_t Level;                    //���⵱ǰIO��ƽ������1��̧��0
        uint8_t DownLevel;                //����ʱIOʵ�ʵĵ�ƽ

        uint8_t State;                    //����״̬
        uint8_t Event;                    //�����¼�

        void *Argv;
        uint16_t Argc;

        uint8_t (*ReadIOLevel)(struct key *self);/*��IO��ƽ����*/
        uint8_t (*Read)(struct key *self);

    public

        uint8_t (*Init)(struct key *self);
        uint8_t (*Scan)(struct key *self);

        uint8_t (*GetState)(struct key *self);                   /*KEY_NULL:�ް�������  KEY_DOWN:��������һ��  KEY_LONG������ KEY_SURE:��������ȷ�ϣ��������̣�*/
        uint8_t (*GetEvent)(struct key *self);                   /*KEY_NULL:�ް�������  KEY_DOWN:��������һ��  KEY_LONG������*/
        uint8_t (*Is_Shield)(struct key *self);

        void (*SetShield)(struct key *self,bool is_shield);      /*��������ʹ�� */
        void (*SetDownLevel)(struct key *self,uint8_t downlevel);/*���ð�������ʱIO�ڵ�ʵ�ʵ�ƽ */

        void (*PressedCallBack)(struct key *self,void *argv,uint16_t argc);    /*�ص�����(����һ��)*/
        void (*LongPressedCallBack)(struct key *self,void *argv,uint16_t argc);/*�ص�����(����)*/

        void (*BindUsrData)(struct key *self,void *argv,uint16_t argc);

        void (*Test)(struct key *self);

        struct key *Self;
}key_t;

uint8_t KEY_Init(struct key *self);

#endif /* OS_DEVICE_KEY_KEY_H_ */
