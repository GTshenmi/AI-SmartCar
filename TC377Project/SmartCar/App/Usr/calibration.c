/*
 * calibration.c
 *
 *  Created on: 2021Äê7ÔÂ29ÈÕ
 *      Author: 936305695
 */
#include "calibration.h"
#include "include.h"

void CalibrationESensor(esensor_t *esensor,uint16_t line,uint16_t index,float ref)
{
    uint16_t res = 0;
    float val = 0.0;
    float gain = 0.0;

    esensor->SetGain(esensor->Self,1.0);
    esensor->EnableGain(esensor->Self,true);
    res = esensor->SelfCalibration(esensor->Self,ref);

    val = esensor->Read(esensor->Self) * 100.0 / 4096.0;

    if(res || fabs(val - ref) >= 2.5)
    {
        Screen.SetFontColor(Screen.Self,RED);
    }
    else if(fabs(val - ref) >= 0.5)
    {
        Screen.SetFontColor(Screen.Self,ORANGE);
    }

    gain = esensor->Gain;

    Screen.WriteXLine(Screen.Self,line,"(LV[%d],%.2f,%.1f,%d)",index,gain,val,res);

    Screen.SetFontColor(Screen.Self,BLACK);
}

uint16_t VESensorCalIndex[2][2] = {{CalibrationLVESensor0,CalibrationLVESensor1},
                               {CalibrationSVESensor0,CalibrationSVESensor1}};
void CalibrationVESensor(uint16_t esensor,uint16_t index)
{
    data_t *data = (data_t *)&Data[data_pointer];

    SelfCalibration(data,VESensorCalIndex[esensor][index]);
}

uint16_t HOESensorCalIndex[2] = {CalibrationLHOESensor,CalibrationSHOESensor};

void CalibrationHOESensor(uint16_t esensor)
{
    data_t *data = (data_t *)&Data[data_pointer];
    SelfCalibration(data,HOESensorCalIndex[esensor]);
}

void SelfCalibration(void *argv,uint16_t object)
{
    //data_t *data = (data_t *)argv;

    float LCalValue[7] = {45.5,35.5,35.5,35.5,35.5,35.5,45.5};
    float SCalValue[8] = {35.5,45.5,35.5,35.5,35.5,35.5,45.5,35.5};

    uint16_t LHOIndex[5] = {1,2,3,4,5};
    uint16_t SHOIndex[6] = {0,2,3,4,5,7};

    uint16_t LVIndex[2] = {0,6};
    uint16_t SVIndex[2] = {1,6};

//    data->UIEnable = false;

    os.time.delay(1.0,s);

    Screen.Clear(Screen.Self,WHITE);

    uint16_t index = 0;

    switch(object)
    {
        case PrepareCalibration:

            break;

        case CalibrationLHOESensor:

            for(uint16_t i = 0;i < 5;i++)
            {
                index = LHOIndex[i];

                CalibrationESensor(LESensor[index].Self,Screen.CenterLine - 6 /2 + i,index,LCalValue[index]);
            }

            break;

        case CalibrationLVESensor0:

            index = LVIndex[0];
            CalibrationESensor(LESensor[index].Self,Screen.CenterLine,index,LCalValue[index]);

            break;

        case CalibrationLVESensor1:

            index = LVIndex[1];
            CalibrationESensor(LESensor[index].Self,Screen.CenterLine,index,LCalValue[index]);

            break;

        case CalibrationSHOESensor:

            for(uint16_t i = 0;i < 6;i++)
            {
                index = SHOIndex[i];

                CalibrationESensor(SESensor[index].Self,Screen.CenterLine - 6 /2 + i,index,SCalValue[index]);
            }

            break;

        case CalibrationSVESensor0:

            index = SVIndex[0];
            CalibrationESensor(SESensor[index].Self,Screen.CenterLine,index,SCalValue[index]);

            break;

        case CalibrationSVESensor1:

            index = SVIndex[1];
            CalibrationESensor(SESensor[index].Self,Screen.CenterLine,index,SCalValue[index]);

            break;

        case CalibrationIMU:

            break;
    }

    //data->UIEnable = true;
}



































































































//void PageUpKeySysCallBack(key_t *self,void *argv,uint16_t argc)
//{
//    data_t *data = &Data[data_pointer];
//
//    data->VESensorCalConfirmState = Yes;
//}
//
//void PageDownKeySysCallBack(key_t *self,void *argv,uint16_t argc)
//{
//    data_t *data = &Data[data_pointer];
//
//    data->VESensorCalConfirmState = No;
//}
//
//void CalibrationVESensor(esensor_t *vesensor0,esensor_t *vesensor1,uint16_t *index,float *ref)
//{
//    data_t *data = (data_t *)&Data[data_pointer];
//
//    uint32_t k1 = 0.0,k2 = 0.0;
//
//    Screen.Clear(Screen.Self,WHITE);
//
//    Screen.WriteXLine(Screen.Self,Screen.CenterLine - 2,"Cal LVESensor[0]?");
//
//    Screen.WriteXLine(Screen.Self,Screen.CenterLine + 2,"    Y       N    ");
//
//    k1 = PageDownKeyIndex;
//    k2 = PageUpKeyIndex;
//
//    KEY[k1].ChangeGroup(KEY[k1].Self,Sys,"Sys Enable:123456");
//    KEY[k2].ChangeGroup(KEY[k2].Self,Sys,"Sys Enable:123456");
//
//    KEY[k1].SysCallBack = PageDownKeySysCallBack;
//    KEY[k2].SysCallBack = PageUpKeySysCallBack;
//
//    while(!data->VESensorCalConfirmState)
//    {
//        KEY[k1].Scan(KEY[k1].Self);
//        KEY[k2].Scan(KEY[k2].Self);
//
//        os.time.delay(0.02,s);
//    }
//
//    if(data->VESensorCalConfirmState == Yes)
//    {
//        CalibrationESensor(vesensor0,0,index[0],ref[index[0]]);
//    }
//    else
//    {
//
//    }
//
//    data->VESensorCalConfirmState = UnConfirmed;
//
//    Screen.WriteXLine(Screen.Self,Screen.CenterLine - 2,"Cal LVESensor[1]?");
//
//    Screen.WriteXLine(Screen.Self,Screen.CenterLine + 2,"    Y       N    ");
//
//    while(!data->VESensorCalConfirmState)
//    {
//        KEY[k1].Scan(KEY[k1].Self);
//        KEY[k2].Scan(KEY[k2].Self);
//
//        os.time.delay(0.02,s);
//    }
//
//    if(data->VESensorCalConfirmState == Yes)
//    {
//        CalibrationESensor(vesensor1,1,index[1],ref[index[1]]);
//    }
//    else
//    {
//
//    }
//    data->VESensorCalConfirmState = UnConfirmed;
//
//    KEY[k1].ChangeGroup(KEY[k1].Self,Usr,"NULL");
//    KEY[k2].ChangeGroup(KEY[k2].Self,Usr,"NULL");
//}


