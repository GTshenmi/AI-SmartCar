/*
 * ano_dt.c
 *
 *  Created on: 2021骞�2鏈�24鏃�
 *      Author: 936305695
 */
#include "ano_dt.h"

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

//浣跨敤鍖垮悕4.3涓婁綅鏈哄崗璁�
/////////////////////////////////////////////////////////////////////////////////////
//鏁版嵁鎷嗗垎瀹忓畾涔夛紝鍦ㄥ彂閫佸ぇ浜�1瀛楄妭鐨勬暟鎹被鍨嬫椂锛屾瘮濡俰nt16銆乫loat绛夛紝闇�瑕佹妸鏁版嵁鎷嗗垎鎴愬崟鐙瓧鑺傝繘琛屽彂閫�
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )     /*!< uint32_t 鏁版嵁鎷嗗垎 byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t 鏁版嵁鎷嗗垎 byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t 鏁版嵁鎷嗗垎 byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t 鏁版嵁鎷嗗垎 byte3  */

/**  鍙戦�佹暟鎹紦瀛� */
unsigned char data_to_send[50];

/*!
  * @brief    Send_Data鍑芥暟鏄崗璁腑鎵�鏈夊彂閫佹暟鎹姛鑳戒娇鐢ㄥ埌鐨勫彂閫佸嚱鏁�
  *
  * @param    dataToSend   :   瑕佸彂閫佺殑鏁版嵁棣栧湴鍧�
  * @param    length       :   瑕佸彂閫佺殑鏁版嵁闀垮害
  *
  * @return   鏃�
  *
  * @note     绉绘鏃讹紝鐢ㄦ埛搴旀牴鎹嚜韬簲鐢ㄧ殑鎯呭喌锛屾牴鎹娇鐢ㄧ殑閫氫俊鏂瑰紡锛屽疄鐜版鍑芥暟
  *
  * @see      鍐呴儴璋冪敤
  *
  * @date     2019/5/28 鏄熸湡浜�
  */
void ANO_SendData(unsigned char *dataToSend , unsigned short length)
{

    /**浣跨敤涓插彛姝ｅ父鍙戦�佹暟鎹紝澶ф闇�瑕�1.5ms*/
    DebugUart.Transmit(DebugUart.Self,dataToSend,length,0);

}





/*!
 * @brief    鍚戜笂浣嶆満鍙戦�佸彂閫�8浣嶆暟鎹�
 */
void ANO_Send8Bit(void *data)
{
    char *arg = data;

    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
    data_to_send[_cnt++] = 1;

    data_to_send[_cnt++]=BYTE0(*arg);

    sum = 0;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_SendData(data_to_send, _cnt);
}

/*!
 * @brief    鍚戜笂浣嶆満鍙戦�佸彂閫乶um涓�8浣嶆暟鎹�
  *                    涓�娆℃渶澶氬彂閫�(50 - 5)/1 涓�
 */
void ANO_Send8Bits(uint8_t num,...)
{
    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;
    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
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
    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
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
    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
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

    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
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
    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
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

void ANO_Send(char *fmt,...)
{
    char *p = fmt;

    unsigned char  _cnt=0;
    unsigned char  sum = 0, i = 0;

    unsigned char len = 0;

    data_to_send[_cnt++] = 0xAA;      //鍖垮悕鍗忚甯уご  0xAAAA
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;      //浣跨敤鐢ㄦ埛鍗忚甯�0xF1
    data_to_send[_cnt++] = 0;

    char *data = NULL;

    va_list arg;
    va_start(arg,fmt);

    for(;*p;p++)
    {
        if(*p != '%')
            continue;

        p++;

        switch(*p)
        {
            case 'f':

                data = (char *)va_arg(arg,char *);

                data_to_send[_cnt++]=BYTE3(*data);
                data_to_send[_cnt++]=BYTE2(*data);
                data_to_send[_cnt++]=BYTE1(*data);
                data_to_send[_cnt++]=BYTE0(*data);

                len += 4;

                break;

            case 'u':case 's':

                p++;

                switch(*p)
                {
                    case '3':

                        data = (char *)va_arg(arg,char *);

                        data_to_send[_cnt++]=BYTE3(*data);
                        data_to_send[_cnt++]=BYTE2(*data);
                        data_to_send[_cnt++]=BYTE1(*data);
                        data_to_send[_cnt++]=BYTE0(*data);

                        len += 4;

                        p++;

                        break;

                    case '1':

                        data = (char *)va_arg(arg,char *);

                        data_to_send[_cnt++]=BYTE1(*data);
                        data_to_send[_cnt++]=BYTE0(*data);

                        len += 2;

                        p++;

                        break;

                    case '8':

                        data = (char *)va_arg(arg,char *);

                        data_to_send[_cnt++]=BYTE0(*data);

                        len += 1;

                        break;

                    default:

                        break;

                }

                break;

            default:

                break;
        }
    }

    data_to_send[3] = len;

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
    .Send = ANO_Send,

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




