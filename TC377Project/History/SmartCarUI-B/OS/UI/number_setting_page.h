/*
 * number_setting_page.h
 *
 *  Created on: 2021Äê3ÔÂ12ÈÕ
 *      Author:
 */

#ifndef OS_DEVICE_KEY_NUMBER_SETTING_PAGE_H_
#define OS_DEVICE_KEY_NUMBER_SETTING_PAGE_H_

#include "ui.h"
#include "template_page.h"

#define INT_PRECISION 4
#define FLOAT_PRECISION 3

typedef struct sSettingPageStruct_t{
        UIPageStruct *targetUIPage;
        uint8_t digitToOperate;

        void (*Open)(struct sSettingPageStruct_t *Self,UIPageStruct *targetUIPage);
        void (*Display)(struct sSettingPageStruct_t *Self);
        void (*Close)(struct sSettingPageStruct_t *Self);
        uint8_t (*GetStatus)(struct sSettingPageStruct_t *Self);

        void (*Add)(struct sSettingPageStruct_t *Self);
        void (*Minus)(struct sSettingPageStruct_t *Self);
        void (*Confirm)(struct sSettingPageStruct_t *Self);
        void (*Cancel)(struct sSettingPageStruct_t *Self);

        struct sSettingPageStruct_t *Self;
}SettingPageStruct;

extern SettingPageStruct SettingPage;

#endif /* OS_DEVICE_KEY_NUMBER_SETTING_PAGE_H_ */
