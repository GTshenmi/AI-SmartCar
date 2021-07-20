/*
 * enc.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_SYSDRIVERLQ_ENC_H_
#define DRIVER_SYSDRIVERLQ_ENC_H_

#include "LQ_GPT12_ENC.h"
#include "chipdatatype.h"

typedef struct
{
        ENC_InputPin_t InputPin;
        ENC_DirPin_t DirPin;
}encx_t;
typedef struct
{
        uint8_t (*Init)(encx_t *);
        sint16_t (*Read)(encx_t *);
}senc_m;

extern senc_m ENCx;
#endif /* DRIVER_SYSDRIVERLQ_ENC_H_ */
