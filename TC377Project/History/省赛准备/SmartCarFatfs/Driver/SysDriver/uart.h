/*
 * uarn.h
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_UART_H_
#define DRIVER_SYSDRIVER_UART_H_

#include "common.h"
#include "platform.h"

#define UARTx_MAX_TIME_OUT TIME_INFINITE

typedef struct
{
        UART_t    UARTn;
        UART_RX_t RxPin;
        UART_TX_t TxPin;
        uint32_t BaudRate;
        Cpu_MutexLock MutexLock;

}uartx_t;

typedef struct
{
        uint8_t (*Init)(uartx_t *);
        uint8_t (*WriteByte)(uartx_t *,uint8_t byte,sint64_t time_out);
        uint8_t (*WriteBytes)(uartx_t *,uint8_t *string,uint32_t len,sint64_t time_out);

        uint8_t (*ReadByte)(uartx_t *,sint64_t time_out);
        uint8_t (*ReadBytes)(uartx_t *,uint8_t *string,uint32_t len,sint64_t time_out);

        Ifx_TickTime Time_Infinite;
}suart_m;

extern suart_m UARTx;

#endif /* DRIVER_SYSDRIVER_UART_H_ */
