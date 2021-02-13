/*
 * key.c
 *
 *  Created on: 2020年12月14日
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
            }
            else if((self->Level == 1)&& (++self->Count >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
            {
                self->Event = KEY_LONG;//长按事件
                self->Count = 0;//计数器清零
            }
            else
            {
                self->Event = KEY_NULL;//空事件
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

