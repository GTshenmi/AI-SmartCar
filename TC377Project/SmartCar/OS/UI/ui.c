/*
 * ui.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
#include "ui.h"

#define UI_MAX_KEY_NUM 6

ui_data_pkg_t UIData;
void ShowADCData(float *data,uint16_t len);
void Key_PressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0 ; i < UI_MAX_KEY_NUM ; i++)
    {
        if(self == KEY[i].Self)
        {
            Screen.WriteXLine(Screen.Self,0,"KEY[%d] Pressed.",i);
        }
    }
}

/*
 * App Layer 传参:
 * argv: ui_data_pkg_t *
 * argc: sizeof(ui_data_pkg_t)
 * */

void UI_Init()
{
    Screen.Init(Screen.Self);
    for(int i = 0;i < UI_MAX_KEY_NUM ;i++)
    {
        KEY[i].Init(KEY[i].Self);
        KEY[i].PressedCallBack = Key_PressedCallBack;
    }

}

void UI_Update(void *argv,uint16_t argc)
{
    ui_data_pkg_t *data = (ui_data_pkg_t *)argv;

    for(uint16_t i = 0 ; i < 5 ; i++ )
        Screen.WriteXLine(Screen.Self,i + 4,"LADC[%d] = %u",i,data->LADC[i]);

    for(uint16_t i = 0 ; i < 7 ; i++ )
        Screen.WriteXLine(Screen.Self,i + 4 + 5,"SADC[%d] = %u",i,data->SADC[i]);
}

/*
  *  示例:
 *
 * */
void ShowADCData(float *data,uint16_t len)
{
    for(uint16_t i = 0 ; i < len ; i++ )
        Screen.WriteXLine(Screen.Self,i,"SADC[%d] = %.3f",i,data[i]);
}


