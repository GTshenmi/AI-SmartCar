/*
 * gpsr.h
 *
 *  Created on: 2021Äê4ÔÂ11ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_GPSR_H_
#define DRIVER_SYSDRIVER_GPSR_H_

#include "common.h"
#include "platform.h"

void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index);
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index);

#endif /* DRIVER_SYSDRIVER_GPSR_H_ */
