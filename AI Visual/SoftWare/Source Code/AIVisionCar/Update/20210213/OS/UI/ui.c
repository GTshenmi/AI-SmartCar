/*
 * ui.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
#include "ui.h"

/*
 * App Layer 传参:
 * argv: data_t *
 * argc: sizeof(data_t)
 * */
__attribute__((weak)) void Task_UiUpdate(void *argv,uint16_t argc)
{
    UI.Update(argv,argc);
}

void UI_Update(void *argv,uint16_t argc)
{
    ui_data_pkg_t *usrdata = (ui_data_pkg_t *)argv;

    for(uint16_t i = 0 ; i < 5 ; i++ )
        Screen.WriteXLine(Screen.Self,i,"LADC[%d] = %u",i,usrdata->LADC_Value[i]);
}

ui_t UI = {
        .Update = UI_Update,
};




