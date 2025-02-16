/*
 * key.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "key.h"
#include "driver.h"

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
            //状态0：没有按键按下
            case KEY_NULL:
                if(self->Level == 1)//有按键按下
                {
                    self->State = KEY_SURE;//转入状态1
                    self->Event = KEY_NULL;//空事件
                }
                else
                {
                    self->Event = KEY_NULL;//空事件
                }
                break;
            //状态1：按键按下确认
            case KEY_SURE:
                if(self->Level == 1)//确认和上次相同
                {
                    self->State = KEY_DOWN;//转入状态2
                    self->Event = KEY_DOWN;//按下事件
                    self->Count = 0;//计数器清零
                }
                else
                {
                    self->State = KEY_NULL;//转入状态0
                    self->Event = KEY_NULL;//空事件
                }
                break;
            //状态2：按键按下
            case KEY_DOWN:
                if(self->Level != 1)//按键释放，端口高电平
                {
                    self->State = KEY_NULL;//转入状态0
                    self->Event = KEY_UP;//松开事件
                }
                else if((self->Level == 1) && (++self->Count >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
                {
                    self->State = KEY_LONG;//转入状态3
                    self->Event = KEY_LONG;//长按事件
                    self->Count = 0;//计数器清零
                }
                else
                {
                    self->Event = KEY_NULL;//空事件
                }
                break;
            //状态3：按键连续按下
            case KEY_LONG:
                if(self->Level != 1)//按键释放，端口高电平
                {
                    self->State = KEY_NULL;//转入状态0
                    self->Event = KEY_UP;//松开事件
                    self->Event = KEY_NULL;


                    //self->Count = 0;//计数器清零
                }
                else if((self->Level == 1)&&(++self->Count >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
                {
                    self->Event = KEY_LONG;//长按事件

                    self->Count = KEY_LONG_DOWN_DELAY + 1;

                    self->Count = 0;//计数器清零
                }
                else
                {
                    self->Event = KEY_NULL;//空事件
                    //self->Count = 0;//计数器清零
                }
                break;
        }

        self->CallBack(self,self->Argv,self->Argc);
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

uint8_t KEY_ChangeGroup(struct key *self,group_t group,char *psw)
{
    if(group == Sys)
    {
        if(!strcmp(psw,"Sys Enable:123456"))
        {
            self->Group = Sys;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        self->Group = group;
    }

    return 0;
}

void KEY_CallBack(struct key *self,void *argv,uint16_t argc)
{
    if(self->Group == Sys)
    {
        if((self->Event == KEY_DOWN || self->Event == KEY_LONG) && self->SysCallBack != NULL)
            self->SysCallBack(self,self->Argv,self->Argc);
    }
    else
    {
        if(self->Event == KEY_DOWN && (self->PressedCallBack != NULL))
             self->PressedCallBack(self,self->Argv,self->Argc);
        else if(self->Event == KEY_LONG && (self->LongPressedCallBack != NULL))
              self->LongPressedCallBack(self,self->Argv,self->Argc);
    }
}

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

void KEY_Connect(struct key *self,key_eventcallback pressedcallback,key_eventcallback longpressedcallback,void *argv,uint16_t argc)
{
    self->PressedCallBack = pressedcallback;
    self->LongPressedCallBack = longpressedcallback;
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

    self->Argv = NULL;
    self->Argc = 0;
    self->PressedCallBack = KEY_DefaultCallBack;
    self->LongPressedCallBack = KEY_DefaultCallBack;
    self->Connect = KEY_Connect;

    self->SetDownLevel(self,0);
    self->SetShield(self,false);
    self->Connect(self,KEY_DefaultCallBack,KEY_DefaultCallBack,NULL,0);

    self->Group = Usr;

    self->CallBack = KEY_CallBack;
    self->SysCallBack = NULL;
    self->ChangeGroup = KEY_ChangeGroup;

    return GPIOx.Init(self->GPIOn);
}

