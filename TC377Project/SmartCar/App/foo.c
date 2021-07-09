/*
 * foo.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
  *   不知写哪的函数暂时放这
 */
#include "foo.h"
#include "include.h"

uint SaveMotorSystemInfo(float *input,float *output,uint32_t len)
{

    char buffer[100];
    char *bufferPointer = buffer;
    float time = 0.0;

    for(int i = 0 ; i < len ; i++)
    {
        bufferPointer = buffer;

        time = i * 0.002;

        bufferPointer += sprintf(bufferPointer,"%f %f %f\n",time,input[i],output[i]);

        SD.fastWrite("MotorSystem.txt",buffer);
    }

    return 0;
}

char *ElementDict[] =\
{
    "None",\
    "RightAngle",\
    "Cross",\
    "Cycle"\
    "Ramp"\
    "Lost",\
    "Straight",\
    "Corner"\
    "Other"\
    "Parameter"
};

uint SaveSensorDataAndAngleAI(data_t *data,char *path){

    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

//    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%f ",LADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",data->SESensor_NormalizedValue[i]);
    }

    bufferPointer += sprintf(bufferPointer,"%f ",data->CorAngle);

    bufferPointer += sprintf(bufferPointer,ElementDict[data->Element.Point]);

    bufferPointer += sprintf(bufferPointer," AI ");

    bufferPointer += sprintf(bufferPointer,"%f E\n",data->Speed);

    line++;

    uint res = 0;

    res = SD.fastWrite(path,buffer);

    return res;
}

uint SaveSensorDataAndAngle(data_t *data,char *path){

    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

//    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%f ",LADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",data->SESensor_NormalizedValue[i]);
    }
    //bufferPointer += sprintf(bufferPointer,"\n");

    //bufferPointer += sprintf(bufferPointer,"%f E\n",*Angle);

    bufferPointer += sprintf(bufferPointer,"%f ",data->Angle);

    bufferPointer += sprintf(bufferPointer,ElementDict[data->Element.Point]);

    bufferPointer += sprintf(bufferPointer," LAutoBoot ");

    bufferPointer += sprintf(bufferPointer,"%f E\n",data->Speed);

    //bufferPointer += sprintf(bufferPointer,"\n");

    line++;

    uint res = 0;

    res = SD.fastWrite(path,buffer);

    return res;

//    if(data->Element.Type == Cycle)
//        res = SD.fastWrite("Cycle.txt",buffer);
//    else if(data->Element.Type == RightAngle)
//        res = SD.fastWrite("RightAngle.txt",buffer);
//    else if(data->Element.Type == Cross)
//        res = SD.fastWrite("Cross.txt",buffer);
//    else if(data->h_bias >= 20.0 && data->v_bias >= 20.0)
//        res = SD.fastWrite("Corner.txt",buffer);
//    else if(data->h_bias <= 20.0 && data->v_bias <= 20.0)
//        res = SD.fastWrite("Straight.txt",buffer);
//    else
//        res = SD.fastWrite("Other.txt",buffer);
//
//    return res;
//    if(data->Element.Type == Cycle)
//        return SD.fastWrite("Cycle.txt",buffer);
//    else
//        return SD.fastWrite("Other.txt",buffer);

//    char buffer[50];
//    char* bufferPointer = buffer;
//
//    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%u ",LADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%u ",SADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    bufferPointer += sprintf(bufferPointer,"%d",*Angle);
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    return SD.fastWrite("Parameter.txt",buffer);

}


uint SaveParameterSD(data_t *data,float *LADC_Value,float *SADC_Value,float *Angle){

    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

//    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%f ",LADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",SADC_Value[i]);
    }
    //bufferPointer += sprintf(bufferPointer,"\n");

    //bufferPointer += sprintf(bufferPointer,"%f E\n",*Angle);

    bufferPointer += sprintf(bufferPointer,"%f ",*Angle);

    bufferPointer += sprintf(bufferPointer,"%f E\n",data->Speed);

    //bufferPointer += sprintf(bufferPointer,"\n");

    line++;

    return SD.fastWrite("Parameter.txt",buffer);
//    char buffer[50];
//    char* bufferPointer = buffer;
//
//    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%u ",LADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
//        bufferPointer += sprintf(bufferPointer,"%u ",SADC_Value[i]);
//    }
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    bufferPointer += sprintf(bufferPointer,"%d",*Angle);
//    bufferPointer += sprintf(bufferPointer,"\n");
//
//    return SD.fastWrite("Parameter.txt",buffer);

}


