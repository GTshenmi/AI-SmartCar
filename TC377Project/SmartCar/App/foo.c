/*
 * foo.c
 *
 *  Created on: 2021年1月22日
 *      Author: 936305695
  *   不知写哪的函数暂时放这
 */
#include "foo.h"
#include "include.h"

uint SaveParameterSD(float *LADC_Value,float *SADC_Value,float *Angle){
    char buffer[50];
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

    bufferPointer += sprintf(bufferPointer,"%f E\n",*Angle);
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


