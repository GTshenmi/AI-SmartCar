/*
 * communicate.h
 *
 *  Created on: 2020��12��13��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_COMMUNICATE_COMMUNICATE_H_
#define OS_DEVICE_COMMUNICATE_COMMUNICATE_H_

#include "platform.h"

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#define Com_AcquireMutex(lock) IfxCpu_acquireMutex(lock)
#define Com_ReleaseMutex(lock) IfxCpu_releaseMutex(lock)
#define Com_LockMutex(lock)    IfxCpu_setSpinLock((lock),10)
#endif

#define COMM_MAX_BUF_LEN 256

enum
{
    C_UART,C_SPI,C_IIC
};
typedef struct communicate
{
     private

        void *Communicatorn;
        uint32_t CommunicationType;
        uint Is_Busy;


     public

        uint8_t (*Init)(struct communicate *self);
        uint8_t (*Transmit)(struct communicate *self,uint8_t *str,uint32_t len,sint64_t time_out);
        uint8_t (*Receive)(struct communicate *self,uint8_t *str,uint32_t len,sint64_t time_out);

        uint32_t (*Write)(struct communicate *self,sint64_t time_out,const char *fmt,...);
        uint32_t (*WriteLine)(struct communicate *self,sint64_t time_out,const char *fmt,...);
        uint32_t (*Read)(struct communicate *self,sint64_t time_out,const char *fmt,...);
        uint32_t (*ReadLine)(struct communicate *self,sint64_t time_out,const char *fmt,...);

        void (*Test)(struct communicate *self);
        struct communicate *Self;

}communicate_t;

uint8_t ComInit(struct communicate *self);

#endif /* OS_DEVICE_COMMUNICATE_COMMUNICATE_H_ */
