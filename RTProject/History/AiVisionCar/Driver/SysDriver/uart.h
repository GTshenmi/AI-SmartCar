/*
 * uarn.h
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_UART_H_
#define DRIVER_SYSDRIVER_UART_H_

#include "LQ_UART.h"
#include "platform.h"
#include "stdbool.h"

#define UARTx_MAX_TIME_OUT 0x7FFFFFFFFFFFFFFFLL

typedef struct
{
        LPUART_Type *Base;
        uint32_t BaudRate;
}uartx_t;

typedef struct
{
        uint8_t (*Init)(uartx_t *);
        uint8_t (*WriteByte)(uartx_t *,uint8_t byte,sint64_t time_out);
        uint8_t (*WriteBytes)(uartx_t *,uint8_t *string,uint32_t len,sint64_t time_out);

        uint8_t (*ReadByte)(uartx_t *,sint64_t time_out);
        uint8_t (*ReadBytes)(uartx_t *,uint8_t *string,uint32_t len,sint64_t time_out);

        sint64_t Time_Infinite;
}suart_m;

extern suart_m UARTx;

#endif /* DRIVER_SYSDRIVER_UART_H_ */
