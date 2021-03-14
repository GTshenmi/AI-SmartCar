/*
 * template_page.c
 *
 *  Created on: 2021年3月8日
 *      Author:
 */

#include "template_page.h"
#include "number_setting_page.h"

void displayCursor (UIPageStruct *Self, int16_t showLineNumber);

void displayDataAlone (UIPageStruct *Self)
{
    uint8_t dataLineTmp = 0;
    int16_t beginLineTmp = Self->beginLine;
    displayCursor(Self, beginLineTmp);

    while (1)
    {
        if (beginLineTmp < SCREEN_MIN_LINE)
        {
            beginLineTmp++;
            dataLineTmp++;
            continue;
        }
        if (beginLineTmp >= SCREEN_MAX_LINE || dataLineTmp >= Self->lineLength)
        {
            break;
        }

        if (Self->targetDataType == INTEGER)
        {
            Screen.WriteXLine(Screen.Self, beginLineTmp, "%s = %04d", Self->description,
                    (((uint16_t*) (Self->primaryTargetData))[dataLineTmp]));
        }
        else if(Self->targetDataType == FLOAT)
        {
            Screen.WriteXLine(Screen.Self, beginLineTmp, "%s = %.3f", Self->description,
                    (((float*) (Self->secondaryTargetData))[dataLineTmp]));
        }

        beginLineTmp++;
        dataLineTmp++;
    }
}

void showDataWithGraph(UIPageStruct *Self)
{

}

void displayPIDData(UIPageStruct *Self){
    uint8_t dataLineTmp = 0;
    int16_t beginLineTmp = Self->beginLine;
    float* targetData = (float*)Self->primaryTargetData;
    displayCursor(Self, beginLineTmp);

    while (1)
    {
        if (beginLineTmp < SCREEN_MIN_LINE)
        {
            beginLineTmp++;
            dataLineTmp++;
            continue;
        }
        if (beginLineTmp >= SCREEN_MAX_LINE || dataLineTmp >= Self->lineLength)
        {
            break;
        }

        Screen.WriteXLine(Screen.Self, beginLineTmp, "%s: P=%.3f I=%.3f D=%.3f", Self->description, targetData, targetData+1, targetData+2);

        beginLineTmp++;
        dataLineTmp++;
    }
}

void displayCursor (UIPageStruct *Self, int16_t showLineNumber)
{
    if ((showLineNumber >= SCREEN_MIN_LINE || showLineNumber <= SCREEN_MAX_LINE) & Self->cursorSelected)
        //Screen.ShowChar(Screen.Self, Screen.Width - Screen.Font.Width, Screen.Font.Hight * showLineNumber, 'Z', RED);
        Screen.DrawSqr(Screen.Self,0,Self->beginLine * Screen.Font.Hight,Screen.Width,(Self->beginLine+Self->lineLength) * Screen.Font.Hight,LIGHTBLUE);
}

void changeDisplayType (UIPageStruct *Self) // 是否归一化
{
    if(Self->targetDataType == INTEGER)
    {
        Self->targetDataType = FLOAT;
    }else
    {
        Self->targetDataType = INTEGER;
    }
}

void emptyConfirmAction (UIPageStruct *Self)
{
}

void openSettingPage(UIPageStruct *Self){
    SettingPage.Open(SettingPage.Self,Self);
}

void UIPagesInit (void)
{
    UIPages[0].primaryTargetData = UIData.LADC;
    UIPages[0].secondaryTargetData = UIData.NLADC;

    UIPages[1].primaryTargetData = UIData.SADC;
    UIPages[1].secondaryTargetData = UIData.NSADC;

    UIPages[2].primaryTargetData = UIData.Speed;
    UIPages[2].secondaryTargetData = UIData.NSpeed;

    UIPages[3].primaryTargetData = UIData.Actual_Speed;
    UIPages[3].secondaryTargetData = UIData.NActual_Speed;
    UIPages[4].secondaryTargetData = UIData.MPID_Kp;
    UIPages[5].secondaryTargetData = UIData.MPID_Ki;
    UIPages[6].secondaryTargetData = UIData.MPID_Kd;
    UIPages[7].secondaryTargetData = UIData.MPID_Result;
    UIPages[8].primaryTargetData = UIData.M_PwmDuty;
    UIPages[9].primaryTargetData = UIData.Angle;
    UIPages[9].secondaryTargetData = UIData.NAngle;
    UIPages[10].secondaryTargetData = UIData.Bias;
    UIPages[11].secondaryTargetData = UIData.SPID_Kp;

    UIPages[12].secondaryTargetData = UIData.SPID_Ki;
    UIPages[13].secondaryTargetData = UIData.SPID_Kd;
    UIPages[14].secondaryTargetData = UIData.SPID_Result;
    UIPages[15].secondaryTargetData = UIData.S_PwmDuty;

    UIPages[0].beginLine = 0;
    for (uint8_t i = 1; i < TOTAL_PAGE_NUMBER; i++)
    {
        UIPages[i].beginLine = UIPages[i - 1].beginLine + UIPages[i - 1].lineLength;
    }

    UIPages[0].cursorSelected = 1;
    for (uint8_t i = 1; i < TOTAL_PAGE_NUMBER; i++)
    {
        UIPages[i].cursorSelected = 0;
    }

}

UIPageStruct UIPages[TOTAL_PAGE_NUMBER] = {
    [0] = {
            .description = "LADC",
            .displayData = displayDataAlone,
            .Self = &UIPages[0],
            .lineLength = 5,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [1] = {
            .description = "SADC",
            .displayData = displayDataAlone,
            .Self = &UIPages[1],
            .lineLength = 7,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [2] = {
            .description = "MotorSpeedSet", //要设置的电机转速(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[2],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [3] = {
            .description = "ActualMotorSped", //电机的实际速度(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[3],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [4] = {
            .description = "MotorSpeed P", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[4],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [5] = {
            .description = "MotorSpeed I", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[5],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [6] = {
            .description = "MotorSpeed D", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[6],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [7] = {
            .description = "MotorSpeedOutPut", //电机PID输出(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[7],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [8] = {
            .description = "MotorSpeedOutPut", //电机最终的占空比(sint16_t)
            .displayData = displayDataAlone,
            .Self = &UIPages[8],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = emptyConfirmAction,
    },
    [9] = {
            .description = "AngleToSet", //要设置的舵机角度(sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[9],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [10] = {
            .description = "Bias", //中线偏差(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[10],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [11] = {
            .description = "Angle P", //舵机PID的三个参数(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[11],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [12] = {
            .description = "Angle I", //舵机PID的三个参数(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[12],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [13] = {
            .description = "Angle D", //舵机PID的三个参数(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[13],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [14] = {
            .description = "AngleOutput", //舵机PID的输出(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[14],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [15] = {
            .description = "AnglePWM", //舵机最终的占空比(uint16_t)
            .displayData = displayDataAlone,
            .Self = &UIPages[15],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
};

