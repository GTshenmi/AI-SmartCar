/*
 * matrix_page.h
 *
 *  Created on: 2021Äê5ÔÂ24ÈÕ
 *      Author: Chi Zhang
 */

#ifndef OS_UI_MATRIX_PAGE_H_
#define OS_UI_MATRIX_PAGE_H_

#include "ui.h"

typedef struct sMatrixPage_t{
        sint32_t *numberToChange;
        _Bool choosePageOpened;
        uint8_t Cursor[7][7];
        UIPageStruct * UIPage;

        void (*Open)(struct sMatrixPage_t *Self,UIPageStruct *UIPage);
        void (*Close)(struct sMatrixPage_t *Self);

        uint8_t (*GetStatus)(struct sMatrixPage_t *Self);
        void (*Display)(struct sMatrixPage_t *Self);

        void (*UpPressed)(struct sMatrixPage_t *Self);
        void (*DownPressed)(struct sMatrixPage_t *Self);
        void (*LeftPressed)(struct sMatrixPage_t *Self);
        void (*RightPressed)(struct sMatrixPage_t *Self);

        void (*ConfirmPressed)(struct sMatrixPage_t *Self);
        void (*CancelPressed)(struct sMatrixPage_t *Self);

        struct sMatrixPage_t * Self;
}MatrixPageStruct;

extern MatrixPageStruct MatrixPage;


#endif /* OS_UI_MATRIX_PAGE_H_ */
