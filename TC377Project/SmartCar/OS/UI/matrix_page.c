/*
 * matrix_page.c
 *
 *  Created on: 2021Äê5ÔÂ24ÈÕ
 *      Author: Chi Zhang
 */

#include "matrix_page.h"
#include "ui_utilities.h"

extern sint32_t FuzzyRule[7][7];

void displaySettingPage (MatrixPageStruct *Self)
{
    Screen.WriteXLine(Screen.Self, 6, "Number = %2d    ", *(Self->numberToChange));
    Screen.WriteXLine(Screen.Self, 5, "DisplaySettingPage");
}

void displayChoosePage (MatrixPageStruct *Self)
{
    char stringBuffer[20];
    Screen.WriteXLine(Screen.Self, 1, "DisplayChoosePage");
    for (uint8_t i = 0; i < 7; i++)
    {
        for (uint8_t j = 0; j < 7; j++)
        {
            sprintf(stringBuffer, "%2ld ", FuzzyRule[i][j]);
            Screen.ShowString(Screen.Self, (1 + j * 2) * Screen.Font.Width, (2 + i) * Screen.Font.Hight,
                    (uint8_t*) stringBuffer, (Self->Cursor[i][j] ? BLUE : BLACK));
        }
    }
}

void MatrixPage_Open (MatrixPageStruct *Self, UIPageStruct *UIPage)
{
    ClearScreen();
    Self->choosePageOpened = 1;
    Self->Cursor[0][0] = 1;
}

void MatrixPage_Close (MatrixPageStruct *Self)
{
    ClearScreen();
    if (Self->choosePageOpened)
    {
        Self->choosePageOpened = 0;
        Self->numberToChange = NULL;
        for (uint8_t i = 0; i < 7; i++)
        {
            for (uint8_t j = 0; j < 7; j++)
            {
                Self->Cursor[i][j] = 0;
            }
        }
    }
    else
    {
        Self->choosePageOpened = 1;
        Self->numberToChange = NULL;
    }
}

uint8_t MatrixPage_GetStatus (MatrixPageStruct *Self)
{
    return (Self->numberToChange || Self->choosePageOpened) ? 1 : 0;
}

void MatrixPage_Display (MatrixPageStruct *Self)
{
    if (Self->numberToChange)
    {
        displaySettingPage(Self);
    }
    else
    {
        displayChoosePage(Self);
    }
}

void MatrixPage_UpPressed (MatrixPageStruct *Self)
{
    if (Self->numberToChange)
    {
        (*Self->numberToChange)++;
    }
    else
    {
        for (uint8_t i = 1; i < 7; i++)
        {
            for (uint8_t j = 0; j < 7; j++)
            {
                if (Self->Cursor[i][j])
                {
                    Self->Cursor[i][j] = 0;
                    Self->Cursor[i - 1][j] = 1;
                    return;
                }
            }
        }
    }
}

void MatrixPage_DownPressed (MatrixPageStruct *Self)
{
    if (Self->numberToChange)
    {
        (*Self->numberToChange)--;
    }
    else
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            for (uint8_t j = 0; j < 7; j++)
            {
                if (Self->Cursor[i][j])
                {
                    Self->Cursor[i][j] = 0;
                    Self->Cursor[i + 1][j] = 1;
                    return;
                }
            }
        }
    }
}

void MatrixPage_LeftPressed (MatrixPageStruct *Self)
{
    if (Self->numberToChange)
    {

    }
    else
    {
        if (Self->Cursor[0][0])
            return;

        for (uint8_t i = 0; i < 7; i++)
        {
            for (uint8_t j = 0; j < 7; j++)
            {
                if (Self->Cursor[i][j])
                {
                    Self->Cursor[i][j] = 0;
                    Self->Cursor[i][j - 1] = 1;
                    return;
                }

            }
        }
    }
}

void MatrixPage_RightPressed (MatrixPageStruct *Self)
{
    if (Self->numberToChange)
    {

    }
    else
    {
        if (Self->Cursor[6][6])
            return;

        for (uint8_t i = 0; i < 7 * 7; i++)
        {
            for (uint8_t j = 0; j < 7; j++)
            {
                if (Self->Cursor[i][j])
                {
                    Self->Cursor[i][j] = 0;
                    Self->Cursor[i][j + 1] = 1;
                    return;
                }

            }
        }
    }
}

void MatrixPage_ConfirmPressed (MatrixPageStruct *Self)
{
    if (!Self->choosePageOpened)
    {
        MatrixPage_Close(Self);
    }
    else
    {
        Self->choosePageOpened = 1;
        for (uint8_t i = 0; i < 7; i++)
        {
            for (uint8_t j = 0; i < 7; j++)
            {
                if (Self->Cursor[i][j])
                {
                    Self->numberToChange = &(FuzzyRule[i][j]);
                    Self->choosePageOpened = 0;
                    ClearScreen();
                    return;
                }
            }
        }
    }
}

void MatrixPage_CancelPressed (MatrixPageStruct *Self)
{
    MatrixPage_Close(Self);
}

MatrixPageStruct MatrixPage =
{
        .numberToChange = NULL,
        .choosePageOpened = 0,
        .Cursor = {{0}},
        .UIPage = NULL,

        .Open = MatrixPage_Open,
        .Close = MatrixPage_Close,

        .GetStatus = MatrixPage_GetStatus,
        .Display = MatrixPage_Display,

        .UpPressed = MatrixPage_UpPressed,
        .DownPressed = MatrixPage_DownPressed,
        .LeftPressed = MatrixPage_LeftPressed,
        .RightPressed = MatrixPage_RightPressed,

        .ConfirmPressed = MatrixPage_ConfirmPressed,
        .CancelPressed = MatrixPage_CancelPressed,

        .Self = &MatrixPage,
};
