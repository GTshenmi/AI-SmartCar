/*
 * ui.c
 *
 *  Created on: 2020年12月7日
 *      Author: 936305695
 */
#include "ui.h"

ui_data_pkg_t UIData;
void ShowADCData(float *data,uint16_t len);


/*
 * App Layer 传参:
 * argv: ui_data_pkg_t *
 * argc: sizeof(ui_data_pkg_t)
 * */

void UI_Update(void *argv,uint16_t argc)
{
    ui_data_pkg_t *data = (ui_data_pkg_t *)argv;
    ShowADCData(data->NSADC,5);
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


