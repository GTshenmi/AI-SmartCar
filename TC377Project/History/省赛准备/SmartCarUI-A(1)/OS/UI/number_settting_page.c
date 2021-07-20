/*
 * number_setting_page.h
 *
 *  Created on: 2021年3月12日
 *      Author:
 */

#include "number_setting_page.h"

void SettingPageOpen (SettingPageStruct *Self, UIPageStruct *targetUIPage)
{
    Self->targetUIPage = targetUIPage;
    if (targetUIPage->targetDataType == INTEGER)
    {
        Self->digitToOperate = INT_PRECISION - 1;
    }
    else if (targetUIPage->targetDataType == FLOAT)
    {
        Self->digitToOperate = FLOAT_PRECISION - 1;
    }

}

void SettingPageDisplay (SettingPageStruct *Self) // TODO 显示数字 加个方框
{
    uint8_t displayLine = Screen.Hight / Screen.Font.Hight - 1;
    Screen.Fill(Screen.Self, 0, displayLine * Screen.Font.Hight, Screen.Width, (displayLine + 1) * Screen.Font.Hight,
            WHITE);
    if (Self->targetUIPage->targetDataType == INTEGER)
    {
        uint16_t targetData = *((uint16_t*) Self->targetUIPage->primaryTargetData);
        uint8_t charNumber = 0;

        char *displayString = "SetTargetInt:";
        Screen.ShowString(Screen.Self, charNumber * Screen.Font.Width, displayLine * Screen.Font.Hight,
                (uint8_t*) displayString, BLACK);
        charNumber += strlen(displayString);

#if 0
        char stringBuffer[10];
        char* stringPointer = stringBuffer;
        sprintf(stringPointer,"%04d",targetData);
        while(strlen(stringPointer) >= 5){
            Screen.ShowChar(Screen.Self,++charNumber * Screen.Font.Width, displayLine*Screen.Font.Hight, stringPointer[0], BLACK);
            stringPointer++;
        }
        for(uint8_t i = 0;stringPointer[i] != '\0';i++){
            Screen.ShowChar(Screen.Self,++charNumber * Screen.Font.Width,displayLine * Screen.Font.Hight,stringPointer[i],((3-i) == Self->digitToOperate ? RED : BLACK));
        }
#endif
        char stringBuffer[10];
        sprintf(stringBuffer, "%04d", targetData);
        Screen.ShowString(Screen.Self, ++charNumber * Screen.Font.Width, displayLine * Screen.Font.Hight,
                (uint8_t*) stringBuffer, BLACK);
        charNumber += strlen(stringBuffer);
        switch (Self->digitToOperate)
        {
            case 0 :
                Screen.ShowChar(Screen.Self, (charNumber - 1) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 1], RED);
                break;
            case 1 :
                Screen.ShowChar(Screen.Self, (charNumber - 2) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 2], RED);
                break;
            case 2 :
                Screen.ShowChar(Screen.Self, (charNumber - 3) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 3], RED);
                break;
            case 3 :
                Screen.ShowChar(Screen.Self, (charNumber - 4) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 4], RED);
                break;
        }
    }
    else if (Self->targetUIPage->targetDataType == FLOAT)
    {
        float targetData = *((float*) Self->targetUIPage->secondaryTargetData);
        uint8_t charNumber = 0;

        char *displayString = "SetTarFloat:";
        Screen.ShowString(Screen.Self, charNumber * Screen.Font.Width, displayLine * Screen.Font.Hight,
                (uint8_t*) displayString, BLACK);
        charNumber += strlen(displayString);

        char stringBuffer[20];
        sprintf(stringBuffer, "%.3f", targetData);
        Screen.ShowString(Screen.Self, ++charNumber * Screen.Font.Width, displayLine * Screen.Font.Hight,
                (uint8_t*) stringBuffer, BLACK);
        charNumber += strlen(stringBuffer);
        switch (Self->digitToOperate)
        {
            case 0 :
                Screen.ShowChar(Screen.Self, (charNumber - 2) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 2], RED);
                break;
            case 1 :
                Screen.ShowChar(Screen.Self, (charNumber - 3) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 3], RED);
                break;
            case 2 :
                Screen.ShowChar(Screen.Self, (charNumber - 5) * Screen.Font.Width, displayLine * Screen.Font.Hight,
                        stringBuffer[strlen(stringBuffer) - 5], RED);
                break;

        }

#if 0
        while(*stringPointer++ != '.'){
            Screen.ShowChar(Screen.Self,++charNumber * Screen.Font.Width,displayLine * Screen.Font.Hight,*stringPointer,BLACK);
        }
        Screen.ShowChar(Screen.Self,charNumber * Screen.Font.Width,displayLine * Screen.Font.Hight,*stringPointer,(Self->digitToOperate == 4 ? RED : BLACK));
        Screen.ShowChar(Screen.Self,charNumber * Screen.Font.Width,displayLine * Screen.Font.Hight,'.',BLACK);

        for(uint8_t i = 0;stringPointer[i] != '\0';i++){
            Screen.ShowChar(Screen.Self,++charNumber * Screen.Font.Width,displayLine * Screen.Font.Hight,stringPointer[i],((2-i) == Self->digitToOperate ? RED : BLACK));
        }
#endif
    }
}

void SettingPageClose (SettingPageStruct *Self)
{
    Self->targetUIPage = NULL;
}

uint8_t SettingPageGetStatus (SettingPageStruct *Self)
{
    if (Self->targetUIPage)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
float numberCoefficient(uint8_t coefficient){
    switch(coefficient){
        case 2:
            return 1.0f;
        case 1:
            return 0.1f;
        case 0:
            return 0.01f;
    }
    return 0.0f;
}

void SettingPageAdd (SettingPageStruct *Self)
{
    if (Self->targetUIPage->targetDataType == INTEGER)
    {
        uint16_t coefficient = 1;
        for(uint8_t i = 0;i < Self->digitToOperate; i++){
            coefficient *= 10;
        }
        *(int*) Self->targetUIPage->primaryTargetData += coefficient;
    }
    else if (Self->targetUIPage->targetDataType == FLOAT)
        *(float*) Self->targetUIPage->secondaryTargetData += numberCoefficient(Self->digitToOperate);
}

void SettingPageMinus (SettingPageStruct *Self)
{
    if (Self->targetUIPage->targetDataType == INTEGER)
    {
        uint16_t coefficient = 1;
        for(uint8_t i = 0;i < Self->digitToOperate; i++){
            coefficient *= 10;
        }
        *(int*) Self->targetUIPage->primaryTargetData -= coefficient;
    }
    else if (Self->targetUIPage->targetDataType == FLOAT)
        *(float*) Self->targetUIPage->primaryTargetData -= numberCoefficient(Self->digitToOperate);
}

void SettingPageConfirm (SettingPageStruct *Self)
{

    if (Self->digitToOperate == 0)
    {
        Self->Close(Self);
    }
    else
    {
        Self->digitToOperate--;
    }

}

void SettingPageCancel (SettingPageStruct *Self)
{
    if (Self->targetUIPage->targetDataType == INTEGER)
    {
        if (Self->digitToOperate == INT_PRECISION - 1)
        {
            Self->Close(Self);
        }
        else
        {
            Self->digitToOperate++;
        }
    }
    else if (Self->targetUIPage->targetDataType == FLOAT)
    {
        if (Self->digitToOperate == FLOAT_PRECISION - 1)
        {
            Self->Close(Self);
        }
        else
        {
            Self->digitToOperate++;
        }

    }
}

SettingPageStruct SettingPage = {
    .targetUIPage = NULL,

    .Open = SettingPageOpen,
    .Display = SettingPageDisplay,
    .Close = SettingPageClose,
    .GetStatus = SettingPageGetStatus,

    .Add = SettingPageAdd,
    .Minus = SettingPageMinus,
    .Confirm = SettingPageConfirm,
    .Cancel = SettingPageCancel,

    .Self = &SettingPage,
};
