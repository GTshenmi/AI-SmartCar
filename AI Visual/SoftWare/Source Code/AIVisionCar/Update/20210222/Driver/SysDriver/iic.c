/*
 * iic.c
 *
 *  Created on: 2021��2��8��
 *      Author: 936305695
 */
#include "iic.h"

void IICx_Delay(iicx_t *iic)
{
    for(uint32_t i = 0;i < iic->BaudRate ; i++)
        __asm("NOP");
}

uint8_t IICx_Init(iicx_t *iic)
{
    if(iic->IIC_Type == SoftWare_IIC)
    {
        IICx_GPIO_Init(iic->SCLPin);
        IICx_GPIO_Init(iic->SDAPin);
        IICx_GPIO_Write(iic->SCLPin,1);
        IICx_GPIO_Write(iic->SDAPin,0);
    }
    else if(iic->IIC_Type == HardWare_IIC)
    {

    }
    else
        return 1;

    return 0;
}

void IICx_Start(iicx_t *iic)
{
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Output);

    IICx_GPIO_Write(iic->SDAPin,1);
    IICx_GPIO_Write(iic->SCLPin,1);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SDAPin,0); //START:when CLK is high,DATA change form high to low
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,0); //ǯסI2C���ߣ�׼�����ͻ��������
}

void IICx_Stop(iicx_t *iic)
{
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Output); //sda�����
    IICx_GPIO_Write(iic->SCLPin,0);
    IICx_GPIO_Write(iic->SDAPin,0); //STOP:when CLK is high DATA change form low to high
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,1);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SDAPin,1); //����I2C���߽����ź�
    IICx_Delay(iic);
}

unsigned char IICx_WaitAck(iicx_t *iic)
{
    uint8_t  ucErrTime=0;
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Input); //SDA����Ϊ����  ���ӻ���һ���͵�ƽ��ΪӦ��
    IICx_GPIO_Write(iic->SDAPin,1);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,1);
    IICx_Delay(iic);
    while(IICx_GPIO_Read(iic->SDAPin))
    {
        ucErrTime++;
        if(ucErrTime>100)
        {
            IICx_Stop(iic);
            return 1;
        }
    }
    IICx_GPIO_Write(iic->SCLPin,0); //ʱ�����0
    return 0;
}

void IICx_Ack(iicx_t *iic)
{
    IICx_GPIO_Write(iic->SCLPin,0);
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Output);
    IICx_GPIO_Write(iic->SDAPin,0);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,1);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,0);
}

void IICx_NAck(iicx_t *iic)
{
    IICx_GPIO_Write(iic->SCLPin,0);
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Output);
    IICx_GPIO_Write(iic->SDAPin,1);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,1);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_Delay(iic);
    IICx_GPIO_Write(iic->SCLPin,0);
}

uint8_t IICx_WriteByte(iicx_t *iic,uint8_t byte,sint64_t time_out)
{
    uint8_t  t;
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Output);
    IICx_GPIO_Write(iic->SCLPin,0);
    //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        if(byte&0x80)
        {
            IICx_GPIO_Write(iic->SDAPin,1);
        }
        else
        {
            IICx_GPIO_Write(iic->SDAPin,0);
        }

        IICx_GPIO_Write(iic->SCLPin,1);
        IICx_Delay(iic);
        byte<<=1;
        IICx_Delay(iic);
        IICx_Delay(iic);
        IICx_GPIO_Write(iic->SCLPin,0);
        IICx_Delay(iic);
    }
    IICx_Delay(iic);
    return 0;
}

uint8_t IICx_ReadByte(iicx_t *iic,sint64_t time_out,uint8_t ack)
{
    uint8_t  i,receive=0;
    IICx_GPIO_SetDir(iic->SDAPin,IIC_Input); //SDA����Ϊ����ģʽ �ȴ����մӻ���������
    for(i=0;i<8;i++ )
    {
        IICx_GPIO_Write(iic->SCLPin,0);
        IICx_Delay(iic);
        IICx_Delay(iic);
        IICx_Delay(iic);
        IICx_GPIO_Write(iic->SCLPin,1);
        receive<<=1;
        if(IICx_GPIO_Read(iic->SDAPin))
            receive++; //�ӻ����͵ĵ�ƽ
        IICx_Delay(iic);
    }
    if(ack)
        IICx_Ack(iic); //����ACK
    else
        IICx_NAck(iic); //����nACK
    return receive;
}

uint8_t IICx_WriteBytes(iicx_t *iic,uint8_t *bytes,uint32_t len,sint64_t time_out)
{
    for(uint32_t i = 0 ; i < len ;i++)
        IICx_WriteByte(iic,bytes[i],time_out);
    return 0;
}

uint8_t IICx_ReadBytes(iicx_t *iic,uint8_t *bytes,uint32_t len,sint64_t time_out,uint8_t ack)
{
    for(uint32_t i = 0 ; i < len ;i++)
        bytes[i] = IICx_ReadByte(iic,time_out,ack);

    return 0;
}

uint8_t IICx_ReadBytesFromSlave(iicx_t *iic,uint8_t dev, uint8_t reg, uint8_t length, uint8_t *bytes)
{
    uint8_t  count = 0;
    uint8_t  temp;
    IICx_Start(iic);
    IICx_WriteByte(iic,dev,0); //���ʹӻ���ַ
    if(IICx_WaitAck(iic))
    {
        IICx_Stop(iic);
        return 1; //�ӻ���ַд��ʧ��
    }
    IICx_WriteByte(iic,reg,0); //���ͼĴ�����ַ
    IICx_WaitAck(iic);
    IICx_Start(iic);
    IICx_WriteByte(iic,dev+1,0); //�������ģʽ
    IICx_WaitAck(iic);
    for(count=0;count<length;count++)
    {
        if(count!=(length-1))
            temp = IICx_ReadByte(iic,0,1); //��ACK�Ķ�����
        else
            temp = IICx_ReadByte(iic,0,0); //���һ���ֽ�NACK

        bytes[count] = temp;
    }
    IICx_Stop(iic); //����һ��ֹͣ����

    return count;
}

uint8_t IICx_WriteBytesToSlave(iicx_t *iic,uint8_t dev, uint8_t reg, uint8_t length, uint8_t *bytes)
{

    uint8_t  count = 0;
    IICx_Start(iic);
    IICx_WriteByte(iic,dev,0); //���ʹӻ���ַ
    if(IICx_WaitAck(iic))
    {
        IICx_Stop(iic);
        return 1; //�ӻ���ַд��ʧ��
    }
    IICx_WriteByte(iic,reg,0); //���ͼĴ�����ַ
    IICx_WaitAck(iic);
    for(count=0;count<length;count++)
    {
        IICx_WriteByte(iic,bytes[count],0);
        if(IICx_WaitAck(iic)) //ÿһ���ֽڶ�Ҫ�ȴӻ�Ӧ��
        {
            IICx_Stop(iic);
            return 1; //����д��ʧ��
        }
    }
    IICx_Stop(iic); //����һ��ֹͣ����

    return 0;
}

siic_m IICx =
{
        .Init = IICx_Init,

        .ReadByte = IICx_ReadByte,
        .ReadBytes = IICx_ReadBytes,
        .ReadBytesFromSlave = IICx_ReadBytesFromSlave,

        .WriteByte = IICx_WriteByte,
        .WriteBytes = IICx_WriteBytes,
        .WriteBytesToSlave = IICx_WriteBytesToSlave,

};

