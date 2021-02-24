/*
 * ano_dt.c
 *
 *  Created on: 2021��2��24��
 *      Author: 936305695
 */
#include "ano_dt.h"

#define ANO_DT_AscLin UART_Resources[0].UARTN

#define ANO_SendBool   ANO_Send8Bit
#define ANO_SendUint8  ANO_Send8Bit
#define ANO_SendUint16 ANO_Send16Bit
#define ANO_SendUint32 ANO_Send32Bit
#define ANO_SendInt8   ANO_Send8Bit
#define ANO_SendInt16  ANO_Send16Bit
#define ANO_SendInt32  ANO_Send32Bit
#define ANO_SendFloat  ANO_Send32Bit

#define ANO_SendBools   ANO_Send8Bits
#define ANO_SendUint8s  ANO_Send8Bits
#define ANO_SendUint16s ANO_Send16Bits
#define ANO_SendUint32s ANO_Send32Bits
#define ANO_SendInt8s   ANO_Send8Bits
#define ANO_SendInt16s  ANO_Send16Bits
#define ANO_SendInt32s  ANO_Send32Bits
#define ANO_SendFloats  ANO_Send32Bits

//ʹ������4.3��λ��Э��
/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )     /*!< uint32_t ���ݲ�� byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t ���ݲ�� byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t ���ݲ�� byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t ���ݲ�� byte3  */

/**  �������ݻ��� */
unsigned char data_to_send[50];

/*!
  * @brief    Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
  *
  * @param    dataToSend   :   Ҫ���͵������׵�ַ
  * @param    length       :   Ҫ���͵����ݳ���
  *
  * @return   ��
  *
  * @note     ��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
  *
  * @see      �ڲ�����
  *
  * @date     2019/5/28 ���ڶ�
  */
void ANO_SendData(unsigned char *dataToSend , unsigned short length)
{

    /**ʹ�ô��������������ݣ������Ҫ1.5ms*/
    UARTx.WriteBytes(&ANO_DT_AscLin,dataToSend,length,UARTx.Time_Infinite);

}





/*!
 * @brief    ����λ�����ͷ���8λ����
 */
void ANO_Send8Bit(void *data)
{
    char *arg = data;

    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 1;

    data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);
}

/*!
 * @brief    ����λ�����ͷ���num��8λ����
  *                    һ����෢��(50 - 5)/1 ��
 */
void ANO_Send8Bits(uint8_t num,...)
{
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = num;

    char *data = NULL;

    va_list arg;
    va_start(arg,num);

    while(num--)
    {
        data = (char *)va_arg(arg,char *);

        data_to_send[_cnt++]=BYTE0(*data);

        if(num == 0)
            break;
    }

    sum = 0;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);

    va_end(arg);

}

void ANO_Send16Bit(void *data)
{
    char *arg = data;
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 2;

    data_to_send[_cnt++]=BYTE1(*arg);
    data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);
}

void ANO_Send16Bits(uint8_t num,...)
{
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 2 * num;

    char *data = NULL;

    va_list arg;
    va_start(arg,num);

    while(num--)
    {
        data = (char *)va_arg(arg,char *);

        data_to_send[_cnt++]=BYTE1(*data);
        data_to_send[_cnt++]=BYTE0(*data);

        if(num == 0)
            break;
    }

    sum = 0;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);

    va_end(arg);

}

void ANO_Send32Bit(void *data)
{
    char *arg = data;
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;

    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 4;

    data_to_send[_cnt++]=BYTE3(*arg);
    data_to_send[_cnt++]=BYTE2(*arg);
    data_to_send[_cnt++]=BYTE1(*arg);
    data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);
}

void ANO_Send32Bits(uint8_t num,...)
{
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 4 * num;

    char *data = NULL;

    va_list arg;
    va_start(arg,num);

    while(num--)
    {
        data = (char *)va_arg(arg,char *);

        data_to_send[_cnt++]=BYTE3(*data);
        data_to_send[_cnt++]=BYTE2(*data);
        data_to_send[_cnt++]=BYTE1(*data);
        data_to_send[_cnt++]=BYTE0(*data);

        if(num == 0)
            break;
    }

    sum = 0;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);

    va_end(arg);

}

void ANO_Test(void)
{
    uint32_t x1 = 0x0fffffff;
    int x2 = -0x0fffffff;
    float x3 = 1000.0;

    ANO_Send32Bit(&x3);

    ANO_Send32Bits(3,&x1,&x2,&x3);
}

ano_debug_t ANO =
{
    .SendBool = ANO_SendBool,

    .SendInt8 = ANO_SendInt8,
    .SendInt16 = ANO_SendInt16,
    .SendInt32 = ANO_SendInt32,

    .SendUint8 = ANO_SendUint8,
    .SendUint16 = ANO_SendUint16,
    .SendUint32 = ANO_SendUint32,

    .SendFloat = ANO_SendFloat,


    .SendBools = ANO_SendBools,

    .SendInt8s = ANO_SendInt8s,
    .SendInt16s = ANO_SendInt16s,
    .SendInt32s = ANO_SendInt32s,

    .SendUint8s = ANO_SendUint8s,
    .SendUint16s = ANO_SendUint16s,
    .SendUint32s = ANO_SendUint32s,

    .SendFloats = ANO_SendFloats,

    .Test = ANO_Test,
};




