/*
 * foo.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
  *   不知写哪的函数暂时放这
 */
#include "foo.h"
#include "include.h"

FIL ExcelFp;
FIL ParameterTxtFp;

void DataSaveSysInit(char * excel_filename,char * txt_filename)
{
//    f_open(&ExcelFp,excel_filename,FA_READ | FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW);
//    f_open(&ParameterTxtFp,txt_filename,FA_READ | FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW);
}
void DataSaveSysDeInit(FIL *excel_fp,FIL *txt_fp)
{
    f_close(excel_fp);
    f_close(txt_fp);
}

void SavePIDStrToSDCard(PID_TypeDef *PID,char * name)
{
    f_printf(&ParameterTxtFp,"%s.Gain.Input = %f;\n",name,PID->Gain.Input);
    f_printf(&ParameterTxtFp,"%s.Gain.Input = %f;\n",name,PID->Gain.Output);
    f_printf(&ParameterTxtFp,"%s.IntegralValue = %f;\n",name,PID->IntegralValue);

    f_printf(&ParameterTxtFp,"%s.Kp = %f;\n",name,PID->Kp);
    f_printf(&ParameterTxtFp,"%s.Ki = %f;\n",name,PID->Ki);
    f_printf(&ParameterTxtFp,"%s.Kd = %f;\n",name,PID->Kd);

    f_printf(&ParameterTxtFp,"%s.MaxIntegralValue = %f;\n",name,PID->MaxIntegralValue);

    for(uint i = 0 ; i < 3 ; i++)
        f_printf(&ParameterTxtFp,"%s.PID_Error[%d] = %f;\n",name,i,PID->PID_Error[i]);

    f_printf(&ParameterTxtFp,"%s.PID_Type = %u(%s);\n",name,PID->PID_Type,(PID->PID_Type == 0 ? "Positional PID":"Incremental PID"));
    f_printf(&ParameterTxtFp,"%s.Result = %f;\n",name,PID->Result);
}

void SaveParameterToSDCard(void *data)
{
    data_t *pdata = data;

    f_printf(&ParameterTxtFp,"AI State = %u(%s);\n",pdata->AI_State,GetAIState(pdata->AI_State));

    f_printf(&ParameterTxtFp,"Actual Speed = %d;\n",pdata->Actual_Speed);

    f_printf(&ParameterTxtFp,"Angle = %d;\n",pdata->Angle);

    f_printf(&ParameterTxtFp,"Bias = %f;\n",pdata->Bias);

    f_printf(&ParameterTxtFp,"Car Mode = %u;\n(%s)",pdata->CarMode,GetCarMode(pdata->CarMode));

    f_printf(&ParameterTxtFp,"Car State = %u;\n(%s)",pdata->CarState,GetCarState(pdata->CarState));

    f_printf(&ParameterTxtFp,"Element Type = %u(%s);\n",pdata->ElementType,GetElementType(pdata->ElementType));

    for(uint i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
    {
        f_printf(&ParameterTxtFp,"Long-Term ADC Value[%u] = %u;\n",i,pdata->LADC_Value[i]);
        f_printf(&ParameterTxtFp,"NormalLized Long-Term ADC Value[%u] = %f;\n",i,pdata->N_LADC[i]);
    }

    for(uint i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
    {
        f_printf(&ParameterTxtFp,"Short-Term ADC Value[%u] = %u;\n",i,pdata->LADC_Value[i]);
        f_printf(&ParameterTxtFp,"NormalLized Short-Term ADC Value[%u] = %f;\n",i,pdata->N_SADC[i]);
    }

    SavePIDStrToSDCard(&pdata->M_PID,"MPID");
    SavePIDStrToSDCard(&pdata->S_PID,"SPID");

    f_printf(&ParameterTxtFp,"Speed = %d;\n",pdata->Speed);

    f_printf(&ParameterTxtFp,"Max Key Num = %u;\n",CData.MaxKEYDeviceNum);
    f_printf(&ParameterTxtFp,"Max L-ADC Num = %u;\n",CData.MaxLADCDeviceNum);
    f_printf(&ParameterTxtFp,"Max S-ADC Num = %u;\n",CData.MaxSADCDeviceNum);
}

void ReadPIDStrFromSDCard(PID_TypeDef *MPID,PID_TypeDef *SPID)
{
    f_scanf(&ParameterTxtFp,"MPID.Gain.Input = %f;\n",&MPID->Gain.Input);
    f_scanf(&ParameterTxtFp,"MPID.Gain.Input = %f;\n",&MPID->Gain.Output);
    f_scanf(&ParameterTxtFp,"MPID.IntegralValue = %f;\n",&MPID->IntegralValue);

    f_scanf(&ParameterTxtFp,"MPID.Kp = %f;\n",&MPID->Kp);
    f_scanf(&ParameterTxtFp,"MPID.Ki = %f;\n",&MPID->Ki);
    f_scanf(&ParameterTxtFp,"MPID.Kd = %f;\n",&MPID->Kd);

    f_scanf(&ParameterTxtFp,"MPID.MaxIntegralValue = %f;\n",&MPID->MaxIntegralValue);

    for(uint i = 0 ; i < 3 ; i++)
        f_scanf(&ParameterTxtFp,"MPID.PID_Error[%d] = %f;\n",i,&MPID->PID_Error[i]);

    f_scanf(&ParameterTxtFp,"MPID.PID_Type = %u(%s);\n",&MPID->PID_Type);
    f_scanf(&ParameterTxtFp,"MPID.Result = %f;\n",&MPID->Result);

    f_scanf(&ParameterTxtFp,"MPID.Gain.Input = %f;\n",&SPID->Gain.Input);
    f_scanf(&ParameterTxtFp,"MPID.Gain.Input = %f;\n",&SPID->Gain.Output);
    f_scanf(&ParameterTxtFp,"MPID.IntegralValue = %f;\n",&SPID->IntegralValue);

    f_scanf(&ParameterTxtFp,"MPID.Kp = %f;\n",&SPID->Kp);
    f_scanf(&ParameterTxtFp,"MPID.Ki = %f;\n",&SPID->Ki);
    f_scanf(&ParameterTxtFp,"MPID.Kd = %f;\n",&SPID->Kd);

    f_scanf(&ParameterTxtFp,"MPID.MaxIntegralValue = %f;\n",&SPID->MaxIntegralValue);

    for(uint i = 0 ; i < 3 ; i++)
        f_scanf(&ParameterTxtFp,"MPID.PID_Error[%d] = %f;\n",i,&SPID->PID_Error[i]);

    f_scanf(&ParameterTxtFp,"MPID.PID_Type = %u(%s);\n",&SPID->PID_Type);
    f_scanf(&ParameterTxtFp,"MPID.Result = %f;\n",&SPID->Result);
}

void ReadParameterFromSDCard(void *data)
{
    data_t *pdata = data;

    f_scanf(&ParameterTxtFp,"AI State = %u;\n",&pdata->AI_State);

    f_scanf(&ParameterTxtFp,"Actual Speed = %d;\n",&pdata->Actual_Speed);

    f_scanf(&ParameterTxtFp,"Angle = %d;\n",&pdata->Angle);

    f_scanf(&ParameterTxtFp,"Bias = %f;\n",&pdata->Bias);

    f_scanf(&ParameterTxtFp,"Car Mode = %u;\n(%s)",&pdata->CarMode);

    f_scanf(&ParameterTxtFp,"Car State = %u;\n(%s)",&pdata->CarState);

    f_scanf(&ParameterTxtFp,"Element Type = %u;\n(%s)",&pdata->ElementType);

    for(uint i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
    {
        f_scanf(&ParameterTxtFp,"Long-Term ADC Value[%u] = %u;\n",&i,&pdata->LADC_Value[i]);
        f_scanf(&ParameterTxtFp,"NormalLized Long-Term ADC Value[%u] = %f;\n",&i,&pdata->N_LADC[i]);
    }

    for(uint i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
    {
        f_scanf(&ParameterTxtFp,"Short-Term ADC Value[%u] = %u;\n",&i,&pdata->LADC_Value[i]);
        f_scanf(&ParameterTxtFp,"NormalLized Short-Term ADC Value[%u] = %f;\n",&i,&pdata->N_SADC[i]);
    }

    ReadPIDStrFromSDCard(&pdata->M_PID,&pdata->S_PID);

    f_scanf(&ParameterTxtFp,"Speed = %d;\n",&pdata->Speed);
}

void SaveTrackingDataToEEPROM(void *data)
{
    data_t *pdata = data;

   // for(uint i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
   //     EEPROM_Write(&ExcelFp,"%f",pdata->N_SADC[i]);



    f_printf(&ExcelFp,"%d\n",pdata->Angle);
}

void SaveAutoBootModeDataToExcel(void *data)
{
    data_t *pdata = data;

    for(uint i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        f_printf(&ExcelFp,"%u ",pdata->LADC_Value[i]);

    for(uint i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        f_printf(&ExcelFp,"%u ",pdata->SADC_Value[i]);

    f_printf(&ExcelFp,"%d\n",pdata->Angle);
}
