/*
 * enc.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_SYSDRIVERLQ_ENC_H_
#define DRIVER_SYSDRIVERLQ_ENC_H_

#include "LQ_Encoder.h"
#include "platform.h"

typedef struct
{
    ENC_Type *Base; 
    bool Direction;
}encx_t;
typedef struct
{
        uint8_t (*Init)(encx_t *);
        sint16_t (*Read)(encx_t *);
}senc_m;

extern senc_m ENCx;
#endif /* DRIVER_SYSDRIVERLQ_ENC_H_ */
