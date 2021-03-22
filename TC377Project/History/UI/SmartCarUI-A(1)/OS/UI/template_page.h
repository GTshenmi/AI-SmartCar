/*
 * template_page.h
 *
 *  Created on: 2021年3月8日
 *      Author:
 */

#ifndef OS_DEVICE_KEY_TEMPLATE_PAGE_H_
#define OS_DEVICE_KEY_TEMPLATE_PAGE_H_

#include "ui.h"
#include "ui_utilities.h"

#define SCREEN_MAX_LINE 8
#define SCREEN_MIN_LINE 0
#define TOTAL_PAGE_NUMBER 19
#define ADC_MAX_NUMBER 4096.0

typedef enum{
    INTEGER,
    FLOAT,
}DataType;

typedef struct sUIPageStruct_t{
    _Bool cursorSelected;
    int16_t beginLine;
    uint8_t lineLength; // 要显示的行数
    DataType targetDataType;
    char* description;
    void* primaryTargetData;
    void* secondaryTargetData;

    void (*displayData)(struct sUIPageStruct_t *Self);
    void (*confirmAction)(struct sUIPageStruct_t *Self);

    struct sUIPageStruct_t *Self;
}UIPageStruct;

extern UIPageStruct UIPages[];

void UIPagesInit(void);

#endif /* OS_DEVICE_KEY_TEMPLATE_PAGE_H_ */
