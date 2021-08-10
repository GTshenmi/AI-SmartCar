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
