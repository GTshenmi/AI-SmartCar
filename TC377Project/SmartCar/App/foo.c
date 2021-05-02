/*
 * foo.c
 *
 *  Created on: 2021��1��22��
 *      Author: 936305695
  *   ��֪д�ĵĺ�����ʱ����
 */
#include "foo.h"
#include "include.h"

uint SaveParameterSD(uint16_t *LADC_Value,uint16_t *SADC_Value,sint16_t *Angle){
    char buffer[50];
    char* bufferPointer = buffer;

    for(uint8_t i = 0;i < MAX_LESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%u ",LADC_Value[i]);
    }
    bufferPointer += sprintf(bufferPointer,"\n");

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%u ",SADC_Value[i]);
    }
    bufferPointer += sprintf(bufferPointer,"\n");

    bufferPointer += sprintf(bufferPointer,"%d",*Angle);
    bufferPointer += sprintf(bufferPointer,"\n");

    return SD.fastWrite("Parameter.txt",buffer);

}


