/*
 * iic.h
 *
 *  Created on: 2021Äê2ÔÂ8ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_IIC_H_
#define DRIVER_SYSDRIVER_IIC_H_

#include "LQ_GPIO.h"
#include "chipdatatype.h"

enum
{
    SoftWare_IIC,HardWare_IIC
};
enum
{
    IIC_Master,IIC_Slave,
};
enum
{
    IIC_Input = 0,
    IIC_Output = 1,
};


#define IICx_GPIO_Init(gpio)         PIN_InitConfig((gpio),PIN_MODE_OUTPUT,1)
#define IICx_GPIO_SetDir(gpio,dir)   PIN_Dir((gpio),(dir))
#define IICx_GPIO_Write(gpio,state)  PIN_Write((gpio),(state))
#define IICx_GPIO_Read(gpio)         PIN_Read((gpio))


typedef struct
{
        GPIO_Name_t SCLPin;
        GPIO_Name_t SDAPin;
        uint16_t IIC_Type;
        uint16_t IIC_DeviceType;
        uint32_t BaudRate;
}iicx_t;

typedef struct
{
        uint8_t (*Init)(iicx_t *);

        uint8_t (*WriteByte)(iicx_t *,uint8_t byte,sint64_t time_out);
        uint8_t (*WriteBytes)(iicx_t *,uint8_t *string,uint32_t len,sint64_t time_out);
        uint8_t (*WriteBytesToSlave)(iicx_t *iic,uint8_t dev, uint8_t reg, uint8_t length, uint8_t *bytes);

        uint8_t (*ReadByte)(iicx_t *,sint64_t time_out,uint8_t ack);
        uint8_t (*ReadBytes)(iicx_t *,uint8_t *string,uint32_t len,sint64_t time_out,uint8_t ack);
        uint8_t (*ReadBytesFromSlave)(iicx_t *iic,uint8_t dev, uint8_t reg, uint8_t length, uint8_t *bytes);

        uint8_t (*SetSpeed)(iicx_t *,uint32_t speed);
        sint64_t Time_Infinite;
}siic_m;

extern siic_m IICx;

#endif /* DRIVER_SYSDRIVER_IIC_H_ */
