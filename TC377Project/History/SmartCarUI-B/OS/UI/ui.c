/*
 * ui.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
#include "ui.h"
#include "template_page.h"
#include "number_setting_page.h"

ui_data_pkg_t UIData;
void ShowADCData (uint16_t *data, uint16_t len);
void Key0PressedCallBack (struct key *self, void *argv, uint16_t argc);
void Key1PressedCallBack (struct key *self, void *argv, uint16_t argc);
void Key2PressedCallBack (struct key *self, void *argv, uint16_t argc);
void Key3PressedCallBack (struct key *self, void *argv, uint16_t argc);
void Key4PressedCallBack (struct key *self, void *argv, uint16_t argc);
void Key5PressedCallBack (struct key *self, void *argv, uint16_t argc);

/*
 * App Layer 传参:
 * argv: ui_data_pkg_t *
 * argc: sizeof(ui_data_pkg_t)
 * */
void UI_Init ()
{
    Screen.Init(Screen.Self);
    for (int i = 0; i < 6; i++)
    {
        KEY[i].Init(KEY[i].Self);
    }

    KEY[0].PressedCallBack = Key0PressedCallBack;
    KEY[1].PressedCallBack = Key1PressedCallBack;
    KEY[2].PressedCallBack = Key2PressedCallBack;
    KEY[3].PressedCallBack = Key3PressedCallBack;
    KEY[4].PressedCallBack = Key4PressedCallBack;
    KEY[5].PressedCallBack = Key5PressedCallBack;

    UIPagesInit();
}

void UI_Update (void *argv, uint16_t argc)
{
    //ui_data_pkg_t *data = (ui_data_pkg_t *)argv;

    for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
    {
        UIPages[i].displayData(UIPages[i].Self);
    }

    if(SettingPage.GetStatus(SettingPage.Self)){
        SettingPage.Display(SettingPage.Self);
    }
}

void Key0PressedCallBack (struct key *self, void *argv, uint16_t argc) // 按键是坏的 大down
{
    Screen.WriteXLine(Screen.Self, 0, "key0PressedCallBack");
}

void Key1PressedCallBack (struct key *self, void *argv, uint16_t argc) // TODO test: 更改光标指向 下降
{
    if(SettingPage.GetStatus(SettingPage.Self)){

    }
    else
    {
        uint8_t i = 0;
        while (!UIPages[i].cursorSelected)
        {
            i++;
        }
        if (i == TOTAL_PAGE_NUMBER - 1)
        {
            UIPages[i].cursorSelected = 0;
            UIPages[0].cursorSelected = 1;
        }
        else
        {
            UIPages[i++].cursorSelected = 0;
            UIPages[i].cursorSelected = 1;
        }
    }
    Screen.WriteXLine(Screen.Self, 0, "key1PressedCallBack");
}

void Key2PressedCallBack (struct key *self, void *argv, uint16_t argc) // 确定按键
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Confirm(SettingPage.Self);
    }
    else
    {
        uint8_t i = 0;
        while (!UIPages[i].cursorSelected)
        {
            i++;
        }
        UIPages[i].confirmAction(UIPages[i].Self);
    }
    Screen.WriteXLine(Screen.Self, 0, "key2PressedCallBack");
}

void Key3PressedCallBack (struct key *self, void *argv, uint16_t argc) // 小up
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Add(SettingPage.Self);
    }
    else
    {
        for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
        {
            UIPages[i].beginLine += 5;
        }

    }
    UI_Update(NULL, 0);

    Screen.WriteXLine(Screen.Self, 0, "key3PressedCallBack");
}

void Key4PressedCallBack (struct key *self, void *argv, uint16_t argc) // 小down
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Minus(SettingPage.Self);
    }
    else
    {
        for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
        {
            UIPages[i].beginLine -= 5;
        }
    }
    Screen.WriteXLine(Screen.Self, 0, "key4PressedCallBack");
}

void Key5PressedCallBack (struct key *self, void *argv, uint16_t argc) // // 取消按键
{
    if(SettingPage.GetStatus(SettingPage.Self)){
        SettingPage.Cancel(SettingPage.Self);
    }else{

    }
    Screen.WriteXLine(Screen.Self, 0, "key5PressedCallBack");
}

/*
 *  示例:
 *
 * */
void ShowADCData (uint16_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
        Screen.WriteXLine(Screen.Self, i + 1, "ADC[%d] = %5d", i, data[i]);
}
