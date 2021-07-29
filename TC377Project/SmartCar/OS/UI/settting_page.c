/*
 * number_setting_page.h
 *
 *  Created on: 2021Äê3ÔÂ12ÈÕ
 *      Author:
 */

#include <setting_page.h>
#include <foo.h>

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

    ClearScreen();
    Self->Display(Self);

}

void SettingPageDisplay (SettingPageStruct *Self)
{
    const uint8_t displayLine = Screen.Hight / Screen.Font.Hight / 2 - 1;
    if (Self->targetUIPage->targetDataType == INTEGER)
    {
        uint16_t targetData = *((uint16_t*) Self->targetUIPage->primaryTargetData);
        uint8_t charNumber = 0;
        char *displayString = "SetTargetInt:";
        Screen.ShowString(Screen.Self, charNumber * Screen.Font.Width, displayLine * Screen.Font.Hight,
                (uint8_t*) displayString, BLACK);
        charNumber += strlen(displayString);

        char stringBuffer[10];
        sprintf(stringBuffer, "%05d", targetData);
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
        sprintf(stringBuffer, "%.6f", targetData);
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

    }
}

void SettingPageClose (SettingPageStruct *Self)
{
    Self->targetUIPage = NULL;
    ClearScreen();
    SaveDataToEeprom();
}

uint8_t SettingPageGetStatus (SettingPageStruct *Self)
{
    if(Self->targetUIPage){
        return 1;
    }else{
        return 0;
    }
}

float numberCoefficient(uint8_t coefficient){
    switch(coefficient){
        case 5:
            return 100.0f;
        case 4:
            return 10.0f;
        case 3:
            return 1.0f;
        case 2:
            return 0.1f;
        case 1:
            return 0.01f;
        case 0:
            return 0.001f;
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

    Self->Display(Self);
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
        *(float*) Self->targetUIPage->secondaryTargetData -= numberCoefficient(Self->digitToOperate);

    Self->Display(Self);
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

    if(Self->GetStatus(Self))
        Self->Display(Self);
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

    if(Self->GetStatus(Self))
        Self->Display(Self);
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
