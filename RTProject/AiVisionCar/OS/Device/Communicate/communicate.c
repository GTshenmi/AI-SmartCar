/*
 * communicate.c
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */
#include "communicate.h"

uint8_t CommTransmit(struct communicate *self,uint8_t *str,uint32_t len,sint64_t time_out)
{
    if(self->CommunicationType == C_SPI)
    {
        if(time_out == 0)
            time_out = SPIx.Time_Infinite;
        return SPIx.ReadWriteBytes((spix_t *)self->Communicatorn,str,NULL,len,time_out);
    }
    else if(self->CommunicationType == C_UART)
    {
        if(time_out == 0)
            time_out = UARTx.Time_Infinite;
        return UARTx.WriteBytes((uartx_t *)self->Communicatorn,str,len,time_out);
    }
    return 0;
}
uint8_t CommReceive(struct communicate *self,uint8_t *str,uint32_t len,sint64_t time_out)
{
    if(self->CommunicationType == C_SPI)
    {
        if(time_out == 0)
            time_out = SPIx.Time_Infinite;
        return SPIx.ReadWriteBytes((spix_t *)self->Communicatorn,NULL,str,len,time_out);
    }
    else if(self->CommunicationType == C_UART)
    {
        if(time_out == 0)
            time_out = UARTx.Time_Infinite;
        return UARTx.ReadBytes((uartx_t *)self->Communicatorn,str,len,time_out);
    }
    return 0;
}



uint32_t CommRead(struct communicate *self,sint64_t time_out,const char *fmt,...)
{
    uint32_t len = 0;

    char readbuf[COMM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    for(; len < COMM_MAX_BUF_LEN - 1;len++)
    {
         self->Receive(self,(uint8_t *)&readbuf[len],1,time_out);
         if(readbuf[len] == '\0')
             break;
    }

    vsscanf(readbuf,fmt,arg);

    va_end(arg);

    return len;
}
uint32_t CommReadLine(struct communicate *self,sint64_t time_out,const char *fmt,...)
{
    uint32_t len = 0;

    char readbuf[COMM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    for(; len < COMM_MAX_BUF_LEN - 1;len++)
    {
        self->Receive(self,(uint8_t *)&readbuf[len],1,time_out);
        if(readbuf[len] == '\0' || readbuf[len] == '\n')
            break;
    }

    vsscanf(readbuf,fmt,arg);

    va_end(arg);

    return len;
}



uint32_t CommWrite(struct communicate *self,sint64_t time_out,const char *fmt,...)
{
    char writebuf[COMM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    uint32_t len = vsprintf(writebuf,fmt,arg);

    self->Transmit(self,(uint8_t *)writebuf,len,time_out);

    va_end(arg);

    return len;
}
uint32_t CommWriteLine(struct communicate *self,sint64_t time_out,const char *fmt,...)
{
    char writebuf[COMM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    uint32_t len = vsprintf(writebuf,fmt,arg);

    if(len < COMM_MAX_BUF_LEN - 1)
        writebuf[len++] = '\n';
    else
        writebuf[COMM_MAX_BUF_LEN - 1] = '\n';

    self->Transmit(self,(uint8_t *)writebuf,len,time_out);

    va_end(arg);

    return len;
}



uint8_t CommInit(struct communicate *self)
{
    self->Receive = CommReceive;
    self->Transmit = CommTransmit;
    self->Read = CommRead;
    self->ReadLine = CommReadLine;
    self->Write = CommWrite;
    self->WriteLine = CommWriteLine;

    uint8_t res = 0;

    if(self->CommunicationType == C_SPI)
    {
        res = SPIx.Init((spix_t *)self->Communicatorn);
    }
    else if(self->CommunicationType == C_UART)
    {
        res = UARTx.Init((uartx_t *)self->Communicatorn);
    }

    return res;
}



