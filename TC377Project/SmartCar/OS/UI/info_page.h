/*
 * info_page.h
 *
 *  Created on: 2021Äê3ÔÂ15ÈÕ
 *      Author:
 */

#ifndef OS_UI_INFO_PAGE_H_
#define OS_UI_INFO_PAGE_H_

#include "ui.h"
#include "template_page.h"
#include "ui_utilities.h"

typedef enum{
    NOSELECTED,
    CARSTATE,
    MOTOR1,
    MOTOR2,
    MOTOR3,
    SERVO1,
    SERVO2,
    SERVO3,
}SectionEnum;

typedef struct sInfoPageStruct_t{
    _Bool Selected;
    SectionEnum SelectedSection;
    UIPageStruct* UIPageStructCursor;

    void (*Display)(struct sInfoPageStruct_t *Self);
    uint8_t (*GetStatus)(struct sInfoPageStruct_t *Self);

    void (*AddOrMinus)(struct sInfoPageStruct_t *Self);
    void (*Confirm)(struct sInfoPageStruct_t *Self);
    void (*Cancel)(struct sInfoPageStruct_t *Self);

    struct sInfoPageStruct_t *Self;
}InfoPageStruct;

extern InfoPageStruct InfoPage;

#endif
