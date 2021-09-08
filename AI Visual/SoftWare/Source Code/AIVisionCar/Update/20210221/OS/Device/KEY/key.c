/*
 * key.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "key.h"

uint8_t KEY_Read(struct key *self)
{
    if(self->ReadIOLevel(self) == self->DownLevel)
        self->Level = 1;
    else
        self->Level = 0;

    return self->Level;
}

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#pragma warning 549
#endif
uint8_t KEY_Scan(struct key *self)
{
    if(!self->KeyShield)
    {
        self->Read(self);

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

        if(self->Event == KEY_DOWN && (self->PressedCallBack != NULL))
             self->PressedCallBack(self,self->Argv,self->Argc);
        else if(self->Event == KEY_LONG && (self->LongPressedCallBack != NULL))
              self->LongPressedCallBack(self,self->Argv,self->Argc);
    }
    else
    {
        self->State = KEY_NULL;
        self->Event = KEY_NULL;
    }
    return 0;
}
#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#pragma warning default
#endif

void KEY_SetShield(struct key *self,bool is_shield)
{
    if(is_shield)
        self->KeyShield = 1;
    else
        self->KeyShield = 0;
}

uint8_t KEY_GetEvent(struct key *self)
{
    return self->Event;
}

uint8_t KEY_GetState(struct key *self)
{
    return self->State;
}

uint8_t KEY_IsShield(struct key *self)
{
    return self->KeyShield;
}

void KEY_SetDownLevel(struct key *self,uint8_t downlevel)
{
    self->DownLevel = downlevel;
}

void KEY_BindUsrData(struct key *self,void *argv,uint16_t argc)
{
    self->Argv = argv;
    self->Argc = argc;
}

void KEY_DefaultCallBack(struct key *self,void *argv,uint16_t argc)
{

}

uint8_t KEY_ReadIOLevel(struct key *self)
{
    return GPIOx.Read(self->GPIOn);
}


uint8_t KEY_Init(struct key *self)
{
    self->Count = 0;
    self->Level = 0;
    self->State = KEY_NULL;
    self->Event = KEY_NULL;
    self->Argv = NULL;
    self->Argc = 0;
    self->KeyShield = false;
    self->DownLevel = 0;


    self->ReadIOLevel = KEY_ReadIOLevel;
    self->Read = KEY_Read;

    self->Scan = KEY_Scan;

    self->Is_Shield = KEY_IsShield;
    self->GetEvent = KEY_GetEvent;
    self->GetState = KEY_GetState;

    self->SetDownLevel = KEY_SetDownLevel;
    self->SetShield = KEY_SetShield;
    self->BindUsrData = KEY_BindUsrData;

    self->PressedCallBack = KEY_DefaultCallBack;
    self->LongPressedCallBack = KEY_DefaultCallBack;

    self->SetDownLevel(self,0);
    self->SetShield(self,false);

    return GPIOx.Init(self->GPIOn);
}

