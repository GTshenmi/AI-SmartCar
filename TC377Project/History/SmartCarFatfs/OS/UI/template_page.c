/*
 * template_page.c
 *
 *  Created on: 2021年3月8日
 *      Author:
 */

#include "template_page.h"
#include "info_page.h"
#include "setting_page.h"
#include "ui_utilities.h"



void displayDataAlone (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{

    if (Self->targetDataType == INTEGER)
    {
        char stringBuffer[30];
        sprintf(stringBuffer, "%s: %04d ", Self->description, (((int16_t*) (Self->primaryTargetData))[dataLineTmp]));
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));
    }
    else if (Self->targetDataType == FLOAT)
    {
        char stringBuffer[30];
        sprintf(stringBuffer, "%s: %.3f", Self->description, (((float*) (Self->secondaryTargetData))[dataLineTmp]));
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));
    }

}

void displayDesciption (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{
    Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) Self->description,
            cursorSelected(Self));
}

void displayDataWithGraph (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{

    if (Self->targetDataType == INTEGER)
    {
        char stringBuffer[30];
        sprintf(stringBuffer, "%s: %04d ", Self->description, (((int16_t*) (Self->primaryTargetData))[dataLineTmp]));
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));

        Screen.DrawSqr(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2, Screen.Width - 1,
                (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
        uint16_t lengthToFill = (uint16_t) Screen.Width / 2.0
                * (((int16_t*) (Self->primaryTargetData))[dataLineTmp] / ADC_MAX_NUMBER);
        Screen.Fill(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2,
                Screen.Width / 2 + lengthToFill, (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
    }
    else if (Self->targetDataType == FLOAT)
    {
        char stringBuffer[30];
        sprintf(stringBuffer, "%s: %.3f", Self->description, (((float*) (Self->secondaryTargetData))[dataLineTmp]));
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));

        Screen.DrawSqr(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2, Screen.Width - 1,
                (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
        uint16_t lengthToFill = (uint16_t) Screen.Width / 2.0
                * (((float*) (Self->secondaryTargetData))[dataLineTmp] / ADC_MAX_NUMBER);
        Screen.Fill(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2,
                Screen.Width / 2 + lengthToFill, (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);

    }

}




void loadFromSD(UIPageStruct *Self){
    LoadParameterFromSD();
}

void saveToSD(UIPageStruct *Self){
    SaveParameterToSD();
}

void changeDisplayType (UIPageStruct *Self) // 是否归一化
{
    if (Self->targetDataType == INTEGER &&  Self->secondaryTargetData != NULL)
    {
        Self->targetDataType = FLOAT;
    }
    else if(Self->targetDataType == FLOAT && Self->secondaryTargetData != NULL)
    {
        Self->targetDataType = INTEGER;
    }

    UIPageDisplay(Self);
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

    UIPages[4].primaryTargetData = UIData.Actual_Speed;
    UIPages[4].secondaryTargetData = UIData.NActual_Speed;

    UIPages[5].secondaryTargetData = UIData.MPID_Kp;
    UIPages[6].secondaryTargetData = UIData.MPID_Ki;
    UIPages[7].secondaryTargetData = UIData.MPID_Kd;
    UIPages[8].secondaryTargetData = UIData.MPID_Result;
    UIPages[9].primaryTargetData = UIData.M_PwmDuty;
    UIPages[10].primaryTargetData = UIData.Angle;

    UIPages[10].secondaryTargetData = UIData.NAngle;
    UIPages[11].secondaryTargetData = UIData.Bias;
    UIPages[12].secondaryTargetData = UIData.SPID_Kp;

    UIPages[13].secondaryTargetData = UIData.SPID_Ki;
    UIPages[14].secondaryTargetData = UIData.SPID_Kd;
    UIPages[15].secondaryTargetData = UIData.SPID_Result;
    UIPages[16].primaryTargetData = UIData.S_PwmDuty;
    UIPages[21].primaryTargetData = UIData.MotorSysState[0];
    UIPages[22].primaryTargetData = UIData.MotorSysState[1];
    UIPages[23].primaryTargetData = UIData.MotorSysState[2];
    UIPages[24].primaryTargetData = UIData.ServoSysState[0];
    UIPages[25].primaryTargetData = UIData.ServoSysState[1];
    UIPages[26].primaryTargetData = UIData.ServoSysState[2];

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
             .description = "CarState",
             .displayData = displayCarInfo,
             .Self = &UIPages[0],
             .lineLength = 1,
             .confirmAction = changeCarStatus,
    },
    [1] = {
            .description = "LADC",
            .displayData = displayDataWithGraph,
            .Self = &UIPages[1],
            .lineLength = MAX_LESENSOR_NUM,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [2] = {
            .description = "SADC",
            .displayData = displayDataWithGraph,
            .Self = &UIPages[2],
            .lineLength = MAX_SESENSOR_NUM,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [3] = {
            .description = "Bias", //中线偏差(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[3],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [4] = {
             .description = "Angle", //要设置的舵机角度(sint16_t，归一化后float)
             .displayData = displayDataAlone,
             .Self = &UIPages[4],
             .lineLength = 1,
             .targetDataType = INTEGER,
             .confirmAction = changeDisplayType,
     },
     [5] = {
             .description = "SPID.Res", //舵机PID的输出(float)
             .displayData = displayDataAlone,
             .Self = &UIPages[5],
             .lineLength = 1,
             .targetDataType = FLOAT,
             .confirmAction = openSettingPage,
     },

     [6] = {
             .description = "SPwmValue", //舵机最终的占空比(uint16_t)
             .displayData = displayDataAlone,
             .Self = &UIPages[6],
             .lineLength = 1,
             .targetDataType = INTEGER,
             .confirmAction = openSettingPage,
     },
     [7] = {
             .description = "SPID.Kp", //舵机PID的三个参数(float)
             .displayData = displayDataAlone,
             .Self = &UIPages[7],
             .lineLength = 1,
             .targetDataType = FLOAT,
             .confirmAction = openSettingPage,
     },
     [8] = {
             .description = "SPID.Ki", //舵机PID的三个参数(float)
             .displayData = displayDataAlone,
             .Self = &UIPages[8],
             .lineLength = 1,
             .targetDataType = FLOAT,
             .confirmAction = openSettingPage,
     },
     [9] = {
             .description = "SPID.Kd", //舵机PID的三个参数(float)
             .displayData = displayDataAlone,
             .Self = &UIPages[9],
             .lineLength = 1,
             .targetDataType = FLOAT,
             .confirmAction = openSettingPage,
     },
    [10] = {
            .description = "Speed", //要设置的电机转速(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[10],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [11] = {
            .description = "Speed", //要设置的电机转速(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[11],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = openSettingPage,
    },
    [12] = {
            .description = "ASpeed", //电机的实际速度(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[12],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = changeDisplayType,
    },
    [13] = {
            .description = "MPID.Res", //电机PID输出(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[13],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [14] = {
            .description = "MPwmValue", //电机最终的占空比(sint16_t)
            .displayData = displayDataAlone,
            .Self = &UIPages[14],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = emptyConfirmAction,
    },
    [15] = {
            .description = "MPID.Kp", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[15],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [16] = {
            .description = "MPID.Ki", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[16],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [17] = {
            .description = "MPID.Kd", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[17],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [18] = {
            .description = "LoadParameterFromSD",
            .displayData = displayDesciption,
            .Self = &UIPages[18],
            .lineLength = 1,
            .confirmAction = loadFromSD,
    },
    [19] = {
            .description = "SaveParameterToSD",
            .displayData = displayDesciption,
            .Self = &UIPages[19],
            .lineLength = 1,
            .confirmAction = saveToSD,
    },

    [20] = {
            .description = "AIState",
            .displayData = displayAIInfo,
            .Self = &UIPages[20],
            .lineLength = 1,
            .confirmAction = emptyConfirmAction,
    },
    [21] = {
            .description = "MSU",
            .displayData = displaySysInfo,
            .Self = &UIPages[21],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
    [22] = {
            .description = "MDU",
            .displayData = displaySysInfo,
            .Self = &UIPages[22],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
    [23] = {
            .description = "MEU",
            .displayData = displaySysInfo,
            .Self = &UIPages[23],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
    [24] = {
            .description = "SSU",
            .displayData = displaySysInfo,
            .Self = &UIPages[24],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
    [25] = {
            .description = "SDU",
            .displayData = displaySysInfo,
            .Self = &UIPages[25],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
    [26] = {
            .description = "SEU",
            .displayData = displaySysInfo,
            .Self = &UIPages[26],
            .lineLength = 1,
            .confirmAction = changeSysStatus,
    },
};

