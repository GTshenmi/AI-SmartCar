/*
 * key.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "key.h"

uint8_t KEY_Read(struct key *self)
{
    return GPIOx.Read(self->GPIODevice);
}

static void Key_GetLevel(struct key *self)
{
            if(!self->KeyShield)
            {
                if(self->Read(self) == self->DownLevel)
                    self->Level = 1;
                else
                    self->Level = 0;
            }
}

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#pragma warning 549
#endif
static void Key_ReadStatus(struct key *self)
{
    Key_GetLevel(self);

    switch(self->State)
    {
        //״̬0��û�а�������
        case KEY_NULL:
            if(self->Level == 1)//�а�������
            {
                self->State = KEY_SURE;//ת��״̬1
                self->Event = KEY_NULL;//���¼�
            }
            else
            {
                self->Event = KEY_NULL;//���¼�
            }
            break;
        //״̬1����������ȷ��
        case KEY_SURE:
            if(self->Level == 1)//ȷ�Ϻ��ϴ���ͬ
            {
                self->State = KEY_DOWN;//ת��״̬2
        self->Event = KEY_DOWN;//�����¼�
                self->Count = 0;//����������
            }
            else
            {
                self->State = KEY_NULL;//ת��״̬0
                self->Event = KEY_NULL;//���¼�
            }
            break;
        //״̬2����������
        case KEY_DOWN:
            if(self->Level != 1)//�����ͷţ��˿ڸߵ�ƽ
            {
                self->State = KEY_NULL;//ת��״̬0
                self->Event = KEY_UP;//�ɿ��¼�
            }
            else if((self->Level == 1) && (++self->Count >= KEY_LONG_DOWN_DELAY)) //����KEY_LONG_DOWN_DELAYû���ͷ�
            {
                self->State = KEY_LONG;//ת��״̬3
                self->Event = KEY_LONG;//�����¼�
                self->Count = 0;//����������
            }
            else
            {
                self->Event = KEY_NULL;//���¼�
            }
            break;
        //״̬3��������������
        case KEY_LONG:
            if(self->Level != 1)//�����ͷţ��˿ڸߵ�ƽ
            {
                self->State = KEY_NULL;//ת��״̬0
                self->Event = KEY_UP;//�ɿ��¼�
                self->Event = KEY_NULL;
            }
            else if((self->Level == 1)&& (++self->Count >= KEY_LONG_DOWN_DELAY)) //����KEY_LONG_DOWN_DELAYû���ͷ�
            {
                self->Event = KEY_LONG;//�����¼�
                self->Count = 0;//����������
            }
            else
            {
                self->Event = KEY_NULL;//���¼�
            }
            break;
    }
}
#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#pragma warning default
#endif

uint8_t KEY_Scan(struct key *self)
{
    Key_ReadStatus(self);
    if(self->Event == KEY_UP)
        self->PressedCallBack(self);
    else if(self->Event == KEY_LONG)
        self->LongPressedCallBack(self);

    return 0;
}
void KEY_DefaultCallBack(struct key *self)
{

}

uint8_t KEY_Init(struct key *self)
{
    self->Count = 0;
    self->Level = 0;
    self->State = KEY_NULL;
    self->Event = KEY_NULL;
    self->Read = KEY_Read;
    self->Scan = KEY_Scan;
    self->PressedCallBack = KEY_DefaultCallBack;
    self->LongPressedCallBack = KEY_DefaultCallBack;
    return GPIOx.Init(self->GPIODevice);
}

