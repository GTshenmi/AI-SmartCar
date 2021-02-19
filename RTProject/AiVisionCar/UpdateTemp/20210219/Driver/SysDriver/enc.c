/*
 * enc.c
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */
#include <enc.h>

uint8_t ENCx_Init(encx_t *enc)
{
    ENC_InitConfig(enc->InputPin,enc->DirPin);
    return 0;
}
sint16_t ENCx_Read(encx_t *enc)
{
    return ENC_GetCounter(enc->InputPin);
}

senc_m ENCx =
{
        .Init = ENCx_Init,
        .Read = ENCx_Read,
};



