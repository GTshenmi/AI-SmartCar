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
    sint16_t value = 0;

    switch(ENC_GetTimer(enc->InputPin))
    {
        case 2:
            value = (signed short)MODULE_GPT120.T2.U;
            MODULE_GPT120.T2.U = 0;
            break;

        case 3:
          value = (signed short)MODULE_GPT120.T3.U;
          MODULE_GPT120.T3.U = 0;
          break;

        case 4:
          value = (signed short)MODULE_GPT120.T4.U;
          MODULE_GPT120.T4.U = 0;
          break;

        case 5:
          value = (signed short)MODULE_GPT120.T5.U;
          MODULE_GPT120.T5.U = 0;
          break;

        case 6:
          value = (signed short)MODULE_GPT120.T6.U;
          MODULE_GPT120.T6.U = 0;
          break;
    }

    return value;
}

senc_m ENCx =
{
        .Init = ENCx_Init,
        .Read = ENCx_Read,
};



