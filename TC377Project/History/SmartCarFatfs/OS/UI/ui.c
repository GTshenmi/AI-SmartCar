/*
 * ui.c
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */
#include <setting_page.h>
#include "ui.h"
#include "info_page.h"
#include "ui_utilities.h"

ui_data_pkg_t UIData;

void cursorUpPressedCallBack (key_t *self, void *argv, uint16_t argc);
void cursorDownPressedCallBack (key_t *self, void *argv, uint16_t argc);
void pageUpPressedCallBack (key_t *self, void *argv, uint16_t argc);
void pageDownPressedCallBack (key_t *self, void *argv, uint16_t argc);
void pageUpLongPressedCallBack (key_t *self, void *argv, uint16_t argc);
void confirmPressedCallBack (key_t *self, void *argv, uint16_t argc);
void pageDownLongPressedCallBack (key_t *self, void *argv, uint16_t argc);
void cancelPressedCallBack (key_t *self, void *argv, uint16_t argc);
void (*LoadParameterFromSD) (void) = NULL;
void (*SaveParameterToSD) (void) = NULL;

/*
 * App Layer ����:
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
    DIPSwitch.Init(DIPSwitch.Self);



    KEY[0].Connect(KEY[0].Self,cursorUpPressedCallBack,NULL,NULL,0);
    KEY[1].Connect(KEY[1].Self,cursorDownPressedCallBack,NULL,NULL,0);
    KEY[2].Connect(KEY[2].Self,confirmPressedCallBack,NULL,NULL,0);
    KEY[3].Connect(KEY[3].Self,cancelPressedCallBack,NULL,NULL,0);
    KEY[4].Connect(KEY[4].Self,pageDownPressedCallBack,pageDownLongPressedCallBack,NULL,0);
    KEY[5].Connect(KEY[5].Self,pageUpPressedCallBack,pageUpLongPressedCallBack,NULL,0);

    UIPagesInit();
}

void UI_Update (void *argv, uint16_t argc)
{
#if defined(Chip) && Chip == TC377 || Chip == TC264
    if (Cpu_AcquireMutex(&UIMutexLock))
    {
#endif
        //ui_data_pkg_t *data = (ui_data_pkg_t *)argv;
        if (SettingPage.GetStatus(SettingPage.Self))
        {
            SettingPage.Display(SettingPage.Self);
        }
        else
        {
            for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
            {
                UIPageDisplay(UIPages[i].Self);
            }

        }

#if defined(Chip) && Chip == TC377 || Chip == TC264
        Cpu_ReleaseMutex(&UIMutexLock);
    }

#endif
}

void cursorUpPressedCallBack (key_t *self, void *argv, uint16_t argc) // �������
{
    if(!SettingPage.GetStatus(SettingPage.Self)){
        uint8_t i = 0;
        while (!UIPages[i].cursorSelected && i <= TOTAL_PAGE_NUMBER)
        {
            i++;
        }
        if (i == 0)
        {
            UIPages[0].cursorSelected = 0;
            UIPages[TOTAL_PAGE_NUMBER - 1].cursorSelected = 1;
        }
        else if (i >= TOTAL_PAGE_NUMBER)
        {
            return;
        }
        else
        {
            UIPages[i--].cursorSelected = 0;
            UIPages[i].cursorSelected = 1;
        }
    }
    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,9,BLACK);
}

void cursorDownPressedCallBack (key_t *self, void *argv, uint16_t argc) // TODO test: ���Ĺ��ָ�� �½�
{
    if(!SettingPage.GetStatus(SettingPage.Self)){
        uint8_t i = 0;
        while (!UIPages[i].cursorSelected && i <= TOTAL_PAGE_NUMBER)
        {
            i++;
        }
        if (i == TOTAL_PAGE_NUMBER - 1)
        {
            UIPages[i].cursorSelected = 0;
            UIPages[0].cursorSelected = 1;
        }
        else if (i >= TOTAL_PAGE_NUMBER)
        {
            return;
        }
        else
        {
            UIPages[i++].cursorSelected = 0;
            UIPages[i].cursorSelected = 1;
        }
    }
    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,1,BLACK);
}

void confirmPressedCallBack (key_t *self, void *argv, uint16_t argc) // ȷ������
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
    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,2,BLACK);
}

void pageUpPressedCallBack (key_t *self, void *argv, uint16_t argc) // Сup
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Add(SettingPage.Self);
    }
    else
    {
        ClearScreen();
        for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
        {
            UIPages[i].beginLine += 5;
        }
        changeCursorLocation();

    }
    UI_Update(NULL, 0);

    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,3,BLACK);
}

void pageDownPressedCallBack (key_t *self, void *argv, uint16_t argc) // Сdown
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Minus(SettingPage.Self);
    }
    else
    {
        ClearScreen();
        for (uint8_t i = 0; i < TOTAL_PAGE_NUMBER; i++)
        {
            UIPages[i].beginLine -= 5;
        }
        changeCursorLocation();
    }
    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,4,BLACK);
}

void pageDownLongPressedCallBack(key_t *self, void *argv, uint16_t argc){
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Minus(SettingPage.Self);
    }
}
void pageUpLongPressedCallBack (key_t *self, void *argv, uint16_t argc){
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Add(SettingPage.Self);
    }
}

void cancelPressedCallBack (key_t *self, void *argv, uint16_t argc) // // ȡ������
{
    if (SettingPage.GetStatus(SettingPage.Self))
    {
        SettingPage.Cancel(SettingPage.Self);
        ClearScreen();
    }

    Screen.ShowNum(Screen.Self,Screen.Width - Screen.Font.Width,0,5,BLACK);
}

