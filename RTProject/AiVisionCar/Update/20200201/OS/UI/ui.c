/*
 * ui.c
 *
 *  Created on: 2020��12��7��
 *      Author: 936305695
 */
#include "ui.h"

__attribute__((weak)) void Task_UiUpdate()
{
    UI.Update();
}

void UI_Update()
{

}

ui_t UI = {
        .Update = UI_Update,
};




