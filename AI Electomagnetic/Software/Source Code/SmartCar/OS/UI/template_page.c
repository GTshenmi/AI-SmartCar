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
#include "matrix_page.h"


uint8_t CalibrationSensorPageOn = 0;

uint UIPagesGetIndexByDescription(char *description)
{
    uint index = 0;

    for(uint i = 0 ; i < TOTAL_PAGE_NUMBER ; i++)
    {
        if(!strcmp(UIPages[i].description,description))
        {
            index = i;
            break;
        }
    }

    if(index >= TOTAL_PAGE_NUMBER)
        index = TOTAL_PAGE_NUMBER - 1;

    //printf("index = %u",index);

    return index;
}

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
        if (dataLineTmp >= 1)
        {
            sprintf(stringBuffer, "%s%d: %.6f", Self->description, dataLineTmp+1,(((float*) (Self->secondaryTargetData))[dataLineTmp]));
        }
        else
        {
            sprintf(stringBuffer, "%s: %.6f", Self->description, (((float*) (Self->secondaryTargetData))[dataLineTmp]));
        }
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));
    }

}

void displayDesciption (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{
    Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) Self->description,
            cursorSelected(Self));
}

void displayElement (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{
    char stringBuffer[30];

    if(UIData.ElementType)
    {
        switch(*UIData.ElementType){
            case 0:
                sprintf(stringBuffer,"%s: None",Self->description);
                break;
            case 1:
                sprintf(stringBuffer,"%s: RightAngle",Self->description);
                break;
            case 2:
                sprintf(stringBuffer,"%s: Cross",Self->description);
                break;
            case 3:
                sprintf(stringBuffer,"%s: Cycle",Self->description);
                break;
        }
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));
    }
    else
    {
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) "ERROR",
                cursorSelected(Self));
    }
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
        Screen.Fill(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2 + 1,
                Screen.Width / 2 + lengthToFill, (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
        Screen.Fill(Screen.Self, Screen.Width / 2 + lengthToFill + 1, beginLineTmp * Screen.Font.Hight * 2 + 1,
                Screen.Width - 1, (beginLineTmp * 2 + 1) * Screen.Font.Hight, WHITE);
    }
    else if (Self->targetDataType == FLOAT)
    {
        char stringBuffer[30];
        sprintf(stringBuffer, "%s:%.2f", Self->description, (((float*) (Self->secondaryTargetData))[dataLineTmp]));
        Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
                cursorSelected(Self));

        Screen.DrawSqr(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2, Screen.Width - 1,
                (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
        uint16_t lengthToFill = (uint16_t) Screen.Width / 2.0
                * (((float*) (Self->secondaryTargetData))[dataLineTmp] / 100.0f);
        Screen.Fill(Screen.Self, Screen.Width / 2, beginLineTmp * Screen.Font.Hight * 2,
                Screen.Width / 2 + lengthToFill, (beginLineTmp * 2 + 1) * Screen.Font.Hight, BLACK);
        Screen.Fill(Screen.Self, Screen.Width / 2 + lengthToFill + 1, beginLineTmp * Screen.Font.Hight * 2 + 1,
                Screen.Width - 1, (beginLineTmp * 2 + 1) * Screen.Font.Hight, WHITE);

    }

}

void loadFromSD (UIPageStruct *Self)
{
    LoadParameterFromSD();
}

void saveToSD (UIPageStruct *Self)
{
    SaveParameterToSD();
}

void changeDisplayType (UIPageStruct *Self) // 是否归一化
{
    if (Self->targetDataType == INTEGER && Self->secondaryTargetData != NULL)
    {
        Self->targetDataType = FLOAT;
    }
    else if (Self->targetDataType == FLOAT && Self->secondaryTargetData != NULL)
    {
        Self->targetDataType = INTEGER;
    }

    UIPageDisplay(Self);
}

void emptyConfirmAction (UIPageStruct *Self)
{
}

void openSettingPage (UIPageStruct *Self)
{
    SettingPage.Open(SettingPage.Self, Self);
}

void openMatrixPage (UIPageStruct *Self)
{
    MatrixPage.Open(MatrixPage.Self, Self);
}

void calibrationSensor(UIPageStruct *Self){
    //ClearScreen();
    CalibrationSensorPageOn = 1;
    if(Self == &UIPages[1])
        pCalibrationVESensor(0,0);
    else if(Self == &UIPages[2])
        pCalibrationVESensor(0,1);
    else if(Self == &UIPages[3])
        pCalibrationVESensor(1,0);
    else if(Self == &UIPages[4])
        pCalibrationVESensor(1,1);
    else if(Self == &UIPages[5])
        pCalibrationHOESensor(0);
    else if(Self == &UIPages[6])
        pCalibrationHOESensor(1);

}

//    UIPages[0].primaryTargetData = UIData.LADC;
//    UIPages[0].secondaryTargetData = UIData.NLADC;

//    UIPages[1].primaryTargetData = UIData.SADC;
//    UIPages[1].secondaryTargetData = UIData.NSADC;

//    UIPages[2].primaryTargetData = UIData.Speed;
//    UIPages[2].secondaryTargetData = UIData.NSpeed;

//    UIPages[3].primaryTargetData = UIData.Actual_Speed;
//    UIPages[3].secondaryTargetData = UIData.NActual_Speed;

//    UIPages[4].primaryTargetData = UIData.Actual_Speed;
//    UIPages[4].secondaryTargetData = UIData.NActual_Speed;

//    UIPages[5].secondaryTargetData = UIData.MPID_Kp;
//    UIPages[6].secondaryTargetData = UIData.MPID_Ki;
//    UIPages[7].secondaryTargetData = UIData.MPID_Kd;
//    UIPages[8].secondaryTargetData = UIData.MPID_Result;
//    UIPages[9].primaryTargetData = UIData.M_PwmDuty;
//    UIPages[10].primaryTargetData = UIData.Angle;

//    UIPages[10].secondaryTargetData = UIData.NAngle;
//    UIPages[11].secondaryTargetData = UIData.Bias;
//    UIPages[12].secondaryTargetData = UIData.SPID_Kp;

//    UIPages[13].secondaryTargetData = UIData.SPID_Ki;
//    UIPages[14].secondaryTargetData = UIData.SPID_Kd;
//    UIPages[15].secondaryTargetData = UIData.SPID_Result;
//    UIPages[16].primaryTargetData = UIData.S_PwmDuty;

//    UIPages[21].primaryTargetData = UIData.MotorSysState[0];
//    UIPages[22].primaryTargetData = UIData.MotorSysState[1];
//    UIPages[23].primaryTargetData = UIData.MotorSysState[2];
//    UIPages[24].primaryTargetData = UIData.ServoSysState[0];
//    UIPages[25].primaryTargetData = UIData.ServoSysState[1];
//    UIPages[26].primaryTargetData = UIData.ServoSysState[2];

void UIPagesInit (void)
{
    UIPages[7].primaryTargetData = UIData.LADC;
    UIPages[7].secondaryTargetData = UIData.NLADC;

    UIPages[8].primaryTargetData = UIData.SADC;
    UIPages[8].secondaryTargetData = UIData.NSADC;

    UIPages[9].secondaryTargetData = UIData.Bias;

    UIPages[10].secondaryTargetData = UIData.Angle;
    //UIPages[4].secondaryTargetData = UIData.NAngle;

//    UIPages[5].secondaryTargetData = UIData.SPID_Result;
    UIPages[11].primaryTargetData = UIData.S_PwmDuty;
//    UIPages[7].secondaryTargetData = UIData.SPID_Kp;
//    UIPages[8].secondaryTargetData = UIData.SPID_Ki;
//    UIPages[9].secondaryTargetData = UIData.SPID_Kd;


    UIPages[12].secondaryTargetData = UIData.Speed;
    //UIPages[10].secondaryTargetData = UIData.NSpeed;

//    UIPages[11].secondaryTargetData = UIData.Speed;
    //UIPages[11].secondaryTargetData = UIData.NSpeed;

    UIPages[13].secondaryTargetData = UIData.Actual_Speed;
    //UIPages[12].secondaryTargetData = UIData.NActual_Speed;

    UIPages[14].secondaryTargetData = UIData.MPID_Result;
    UIPages[15].primaryTargetData = UIData.M_PwmDuty;
    UIPages[16].secondaryTargetData = UIData.MPID_Kp;
    UIPages[17].secondaryTargetData = UIData.MPID_Ki;
    UIPages[18].secondaryTargetData = UIData.MPID_Kd;

//    UIPages[21].secondaryTargetData = UIData.DynamicKp;

    UIPages[19].secondaryTargetData = UIData.o_difference;
    UIPages[20].secondaryTargetData = UIData.h_difference;
    UIPages[21].secondaryTargetData = UIData.v_difference;

    UIPages[22].secondaryTargetData = UIData.o_sum;
    UIPages[23].secondaryTargetData = UIData.h_sum;
    UIPages[24].secondaryTargetData = UIData.v_sum;

    UIPages[25].secondaryTargetData = UIData.o_bias;
    UIPages[26].secondaryTargetData = UIData.h_bias;
    UIPages[27].secondaryTargetData = UIData.v_bias;

    UIPages[30].secondaryTargetData = UIData.Ke;

    UIPages[31].secondaryTargetData = UIData.x;

    UIPages[32].secondaryTargetData = UIData.CycleInDistance;
    UIPages[33].secondaryTargetData = UIData.CycleWaitInDistance;

    UIPages[34].secondaryTargetData = UIData.Err;

    UIPages[35].secondaryTargetData = UIData.Ea;
    UIPages[36].secondaryTargetData = UIData.Da;

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
             .description = "CaliLVESen0",
             .displayData = displayDesciption,
             .Self = &UIPages[1],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [2] = {
             .description = "CaliLVESen1",
             .displayData = displayDesciption,
             .Self = &UIPages[2],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [3] = {
             .description = "CaliSVESen0",
             .displayData = displayDesciption,
             .Self = &UIPages[3],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [4] = {
             .description = "CaliSVESen1",
             .displayData = displayDesciption,
             .Self = &UIPages[4],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [5] = {
             .description = "CaliLHOESen",
             .displayData = displayDesciption,
             .Self = &UIPages[5],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [6] = {
             .description = "CaliSHOESen",
             .displayData = displayDesciption,
             .Self = &UIPages[6],
             .lineLength = 1,
             .confirmAction = calibrationSensor,
    },
    [7] = {
            .description = "LADC",
            .displayData = displayDataWithGraph,
            .Self = &UIPages[7],
            .lineLength = MAX_LESENSOR_NUM,
            .targetDataType = FLOAT,
            .confirmAction = changeDisplayType,
    },
    [8] = {
            .description = "SADC",
            .displayData = displayDataWithGraph,
            .Self = &UIPages[8],
            .lineLength = MAX_SESENSOR_NUM,
            .targetDataType = FLOAT,
            .confirmAction = changeDisplayType,
    },
    [9] = {
            .description = "Bias", //中线偏差(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[9],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,

    },
    [10] = {
             .description = "Angle", //要设置的舵机角度(sint16_t，归一化后float)
             .displayData = displayDataAlone,
             .Self = &UIPages[10],
             .lineLength = 1,
             .targetDataType = FLOAT,
             .confirmAction = openSettingPage,

     },
//     [11] = {
//             .description = "SPID.Res", //舵机PID的输出(float)
//             .displayData = displayDataAlone,
//             .Self = &UIPages[11],
//             .lineLength = 1,
//             .targetDataType = FLOAT,
//             .confirmAction = openSettingPage,
//     },

     [11] = {
             .description = "SPwmValue", //舵机最终的占空比(uint16_t)
             .displayData = displayDataAlone,
             .Self = &UIPages[11],
             .lineLength = 1,
             .targetDataType = INTEGER,
             .confirmAction = openSettingPage,
     },
//     [13] = {
//             .description = "SPID.Kp", //舵机PID的三个参数(float)
//             .displayData = displayDataAlone,
//             .Self = &UIPages[13],
//             .lineLength = 1,
//             .targetDataType = FLOAT,
//             .confirmAction = openSettingPage,
//
//
//     },
//     [14] = {
//             .description = "SPID.Ki", //舵机PID的三个参数(float)
//             .displayData = displayDataAlone,
//             .Self = &UIPages[14],
//             .lineLength = 1,
//             .targetDataType = FLOAT,
//             .confirmAction = openSettingPage,
//
//     },
//     [15] = {
//             .description = "SPID.Kd", //舵机PID的三个参数(float)
//             .displayData = displayDataAlone,
//             .Self = &UIPages[15],
//             .lineLength = 1,
//             .targetDataType = FLOAT,
//             .confirmAction = openSettingPage,
//
//     },
//     [16] = {
//            .description = "Speed", //要设置的电机转速(原数据sint16_t，归一化后float)
//            .displayData = displayDataAlone,
//            .Self = &UIPages[16],
//            .lineLength = 1,
//            .targetDataType = FLOAT,
//            .confirmAction = changeDisplayType,
//     },
     [12] = {
            .description = "Speed", //要设置的电机转速(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,//*
            .Self = &UIPages[12],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
     },
    [13] = {
            .description = "ASpeed", //电机的实际速度(原数据sint16_t，归一化后float)
            .displayData = displayDataAlone,
            .Self = &UIPages[13],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = changeDisplayType,
    },
    [14] = {
            .description = "MPID.Res", //电机PID输出(float)
            .displayData = displayDataAlone,
            .Self = &UIPages[14],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [15] = {
            .description = "MPwmValue", //电机最终的占空比(sint16_t)
            .displayData = displayDataAlone,
            .Self = &UIPages[15],
            .lineLength = 1,
            .targetDataType = INTEGER,
            .confirmAction = emptyConfirmAction,
    },
    [16] = {
            .description = "MPID.Kp", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[16],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [17] = {
            .description = "MPID.Ki", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[17],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [18] = {
            .description = "MPID.Kd", //电机PID的三个参数
            .displayData = displayDataAlone,
            .Self = &UIPages[18],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
//    [24] = {
//            .description = "LoadParameterFromSD",
//            .displayData = displayDesciption,
//            .Self = &UIPages[24],
//            .lineLength = 1,
//            .confirmAction = loadFromSD,
//    },
//    [25] = {
//            .description = "SaveParameterToSD",
//            .displayData = displayDesciption,
//            .Self = &UIPages[25],
//            .lineLength = 1,
//            .confirmAction = saveToSD,
//    },

//    [26] = {
//            .description = "AIState",
//            .displayData = displayAIInfo,
//            .Self = &UIPages[26],
//            .lineLength = 1,
//            .confirmAction = emptyConfirmAction,
//
//    },
//    [27] = {
//            .description = "DymKp",
//            .displayData = displayDataAlone,
//            .Self = &UIPages[27],
//            .lineLength = 1,
//            .targetDataType = FLOAT,
//            .confirmAction = openSettingPage,
//    },
    [19] = {
            .description = "o-diff",
            .displayData = displayDataAlone,
            .Self = &UIPages[19],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [20] = {
            .description = "h-diff",
            .displayData = displayDataAlone,
            .Self = &UIPages[20],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [21] = {
            .description = "v-diff",
            .displayData = displayDataAlone,
            .Self = &UIPages[21],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [22] = {
            .description = "o-sum",
            .displayData = displayDataAlone,
            .Self = &UIPages[22],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [23] = {
            .description = "h-sum",
            .displayData = displayDataAlone,
            .Self = &UIPages[23],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [24] = {
            .description = "v-sum",
            .displayData = displayDataAlone,
            .Self = &UIPages[24],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [25] = {
            .description = "o-bias",
            .displayData = displayDataAlone,
            .Self = &UIPages[25],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [26] = {
            .description = "h-bias",
            .displayData = displayDataAlone,
            .Self = &UIPages[26],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [27] = {
            .description = "v-bias",
            .displayData = displayDataAlone,
            .Self = &UIPages[27],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [28] = {
            .description = "FuzzyCtrl",
            .displayData = displayDesciption,
            .Self = &UIPages[28],
            .lineLength = 1,
            .confirmAction = openMatrixPage,
    },
    [29] = {
            .description = "Element",
            .displayData = displayElement,
            .Self = &UIPages[29],
            .lineLength = 1,
            .confirmAction = emptyConfirmAction,
    },
    [30] = {
            .description = "Ke",
            .displayData = displayDataAlone,
            .Self = &UIPages[30],
            .lineLength = 8,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [31] = {
            .description = "x",
            .displayData = displayDataAlone,
            .Self = &UIPages[31],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [32] = {
            .description = "CycleInDis",         //*
            .displayData = displayDataAlone,
            .Self = &UIPages[32],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [33] = {
            .description = "CycleWtInDis",  //*
            .displayData = displayDataAlone,
            .Self = &UIPages[33],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [34] = {
            .description = "Err",
            .displayData = displayDataAlone,
            .Self = &UIPages[34],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = openSettingPage,
    },
    [35] = {
            .description = "Ea",
            .displayData = displayDataAlone,
            .Self = &UIPages[35],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
    [36] = {
            .description = "Da",
            .displayData = displayDataAlone,
            .Self = &UIPages[36],
            .lineLength = 1,
            .targetDataType = FLOAT,
            .confirmAction = emptyConfirmAction,
    },
};

