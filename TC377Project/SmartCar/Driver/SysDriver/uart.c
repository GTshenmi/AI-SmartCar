/*
 * uart.c
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */
#include "uart.h"

uint8_t UARTx_Init(uartx_t *uart)
{
    UART_InitConfig(uart->RxPin,uart->TxPin,uart->BaudRate);
    return 0;
}

uint8_t UARTx_WriteByte(uartx_t *uart,uint8_t byte,sint64_t time_out)
{
    Cpu_AcquireAndLockMutex(&uart->MutexLock);

    Ifx_SizeT count = 1;

    uint8_t res = IfxAsclin_Asc_write(&g_UartConfig[uart->UARTn], &byte, &count,time_out);

    Cpu_ReleaseMutex(&uart->MutexLock);

    return res;
}
uint8_t UARTx_WriteBytes(uartx_t *uart,uint8_t *bytes,uint32_t len,sint64_t time_out)
{
    for(int i = 0 ; i < len ; i++)
    {
        UARTx_WriteByte(uart,bytes[i],time_out);
    }
    return 0;
}


uint8_t UARTx_ReadByte(uartx_t *uart,sint64_t time_out)
{
    uint8_t data=0;
    Ifx_SizeT count = 1;

    Cpu_AcquireAndLockMutex(&uart->MutexLock);

    IfxAsclin_Asc_read(&g_UartConfig[uart->UARTn], &data, &count, (Ifx_TickTime)time_out);

    Cpu_ReleaseMutex(&uart->MutexLock);

    return  data;
}
uint8_t UARTx_ReadBytes(uartx_t *uart,uint8_t *bytes,uint32_t len,sint64_t time_out)
{
    for(int i = 0 ; i < len ; i++)
        bytes[i] = UARTx_ReadByte(uart,time_out);

    return 0;
}

suart_m UARTx =
{
        .Init = UARTx_Init,

        .WriteByte =UARTx_WriteByte ,
        .WriteBytes = UARTx_WriteBytes,

        .ReadByte = UARTx_ReadByte,
        .ReadBytes = UARTx_ReadBytes,
        .Time_Infinite = UARTx_MAX_TIME_OUT,
};


