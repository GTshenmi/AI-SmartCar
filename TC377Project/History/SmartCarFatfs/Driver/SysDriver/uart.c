/*
 * uart.c
 *
 *  Created on: 2020��12��13��
 *      Author: 936305695
 */
#include "uart.h"
#include "gpio.h"



uint8_t UARTx_Init(uartx_t *uart)
{
    int i,j;
    //�ر�CPU�ж�
    IfxCpu_disableInterrupts();

    Ifx_P *portRx = PIN_GetModule(uart->RxPin);
    uint8 pinIndexRx = PIN_GetIndex(uart->RxPin);

    Ifx_P *portTx = PIN_GetModule(uart->TxPin);
    uint8 pinIndexTx = PIN_GetIndex(uart->TxPin);

    IfxAsclin_Rx_In  * IfxAsclin_Rx = NULL_PTR;
    IfxAsclin_Tx_Out * IfxAsclin_Tx = NULL_PTR;

    for( i = 0; i < IFXASCLIN_PINMAP_NUM_MODULES; i++)
    {
        for( j = 0; j < IFXASCLIN_PINMAP_RX_IN_NUM_ITEMS; j++)
        {
            if(IfxAsclin_Rx_In_pinTable[i][j] == NULL_PTR)
            {

            }
            else if((unsigned long)portRx == (unsigned long)(IfxAsclin_Rx_In_pinTable[i][j]->pin.port) && pinIndexRx == IfxAsclin_Rx_In_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Rx = IfxAsclin_Rx_In_pinTable[i][j];
            }
        }

        for(j = 0; j < IFXASCLIN_PINMAP_TX_OUT_NUM_ITEMS; j++)
        {
            if(IfxAsclin_Tx_Out_pinTable[i][j] == NULL_PTR)
            {

            }
            else if((unsigned long)portTx == (unsigned long)(IfxAsclin_Tx_Out_pinTable[i][j]->pin.port) && pinIndexTx == IfxAsclin_Tx_Out_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Tx = IfxAsclin_Tx_Out_pinTable[i][j];
            }
        }
    }

    if(IfxAsclin_Rx->module != IfxAsclin_Tx->module)
    {
#pragma warning 557         // ���ξ���
        while (1);          // ��� RX��TX �Ƿ�ΪͬһUART
#pragma warning default     // �򿪾���
    }

    //�½��������ýṹ��
    IfxAsclin_Asc_Config ascConfig;

    //��ʼ��ģ��
    IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);

    ascConfig.baudrate.baudrate  = (float)uart->BaudRate;        //������
    ascConfig.frame.frameMode    = IfxAsclin_FrameMode_asc;//����֡ģʽ
    ascConfig.frame.dataLength   = IfxAsclin_DataLength_8; //���ݳ���
    ascConfig.frame.stopBit      = IfxAsclin_StopBit_1;    //ֹͣλ
    ascConfig.frame.shiftDir     = IfxAsclin_ShiftDirection_lsbFirst;//��λ����
    ascConfig.frame.parityBit    = FALSE;//����żУ��

    unsigned char uartNum = IfxAsclin_getIndex(IfxAsclin_Tx->module);

    //�ж����ȼ�����
    ascConfig.interrupt.rxPriority = UartIrqPriority[uartNum * 3];
    ascConfig.interrupt.txPriority = UartIrqPriority[uartNum * 3 + 1];
    ascConfig.interrupt.erPriority = UartIrqPriority[uartNum * 3 + 2];
    ascConfig.interrupt.typeOfService = UartIrqVectabNum[uartNum];

    //���պͷ���FIFO������
    ascConfig.txBuffer     = &s_AscTxBuffer[uartNum][0];
    ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
    ascConfig.rxBuffer     = &s_AscRxBuffer[uartNum][0];
    ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

    IfxAsclin_Asc_Pins pins =
    {
        NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
        IfxAsclin_Rx,             IfxPort_InputMode_pullUp,        /* Rx pin */
        NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
        IfxAsclin_Tx,             IfxPort_OutputMode_pushPull,     /* Tx pin */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    //��������ṹ����Ԥ��Ĳ��������ģ��ĳ�ʼ��
    IfxAsclin_Asc_initModule(&g_UartConfig[uartNum], &ascConfig);

    //���գ����ͺʹ����ж�����
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3],     UartIrqPriority[uartNum * 3]);
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 1], UartIrqPriority[uartNum * 3 + 1]);
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 2], UartIrqPriority[uartNum * 3 + 2]);

    //����CPU�ж�
    IfxCpu_enableInterrupts();

    return 0;
}

uint8_t UARTx_WriteByte(uartx_t *uart,uint8_t byte,sint64_t time_out)
{
    //Cpu_AcquireAndLockMutex(&uart->MutexLock);

    Ifx_SizeT count = 1;

    uint8_t res = IfxAsclin_Asc_write(&g_UartConfig[uart->UARTn], &byte, &count,time_out);

    //Cpu_ReleaseMutex(&uart->MutexLock);

    return res;
}
uint8_t UARTx_WriteBytes(uartx_t *uart,uint8_t *bytes,uint32_t len,sint64_t time_out)
{
    //Cpu_AcquireAndLockMutex(&(uart->MutexLock));

    for(int i = 0 ; i < len ; i++)
    {
        UARTx_WriteByte(uart,bytes[i],time_out);
    }

    //Cpu_ReleaseMutex(&(uart->MutexLock));

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


