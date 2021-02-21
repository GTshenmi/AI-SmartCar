/*
 * ui.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
#include "ui.h"

ui_data_pkg_t UIData;
/*
 * App Layer 传参:
 * argv: ui_data_pkg_t *
 * argc: sizeof(ui_data_pkg_t)
 * */

void UI_Update(void *argv,uint16_t argc)
{
    ui_data_pkg_t *data = (ui_data_pkg_t *)argv;

    for(uint16_t i = 0 ; i < 5 ; i++ )
        Screen.WriteXLine(Screen.Self,i,"LADC[%d] = %u",i,data->LADC[i]);
}




