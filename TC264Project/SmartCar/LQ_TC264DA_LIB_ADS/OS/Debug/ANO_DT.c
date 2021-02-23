#include "ANO_DT.h"

#define ANO_DT_AscLin UART_Resources[0].UARTN

#define ANO_SendBool   ANO_Send8Bit

#define ANO_SendUint8  ANO_Send8Bit
#define ANO_SendUint16 ANO_Send16Bit
#define ANO_SendUint32 ANO_Send32Bit

#define ANO_SendInt8   ANO_Send8Bit
#define ANO_SendInt16  ANO_Send16Bit
#define ANO_SendInt32  ANO_Send32Bit

#define ANO_SendFloat  ANO_Send32Bit

//使用匿名4.3上位机协议
/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )     /*!< uint32_t 数据拆分 byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t 数据拆分 byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t 数据拆分 byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t 数据拆分 byte3  */

/**  发送数据缓存 */
unsigned char data_to_send[50];

/*!
  * @brief    Send_Data函数是协议中所有发送数据功能使用到的发送函数
  *
  * @param    dataToSend   :   要发送的数据首地址
  * @param    length       :   要发送的数据长度
  *
  * @return   无
  *
  * @note     移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
  *
  * @see      内部调用
  *
  * @date     2019/5/28 星期二
  */
void ANO_SendData(unsigned char *dataToSend , unsigned short length)
{

    /**使用串口正常发送数据，大概需要1.5ms*/
	UARTx.WriteBytes(&ANO_DT_AscLin,dataToSend,length,UARTx.Time_Infinite);

}





/*!
  * @brief    向上位机发送发送8位数据
  */
void ANO_Send8Bit(void *data)
{
    char *arg = data;

    unsigned char  _cnt=0;
	unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1
    data_to_send[_cnt++] = 1;

	data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];

	data_to_send[_cnt++]=sum;

	ANO_SendData(data_to_send, _cnt);
}

void ANO_Send16Bit(void *data)
{
    char *arg = data;
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1
    data_to_send[_cnt++] = 2;

    data_to_send[_cnt++]=BYTE1(*arg);
    data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);
}

void ANO_Send32Bit(void *data)
{
    char *arg = data;
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;

    data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1
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
};
