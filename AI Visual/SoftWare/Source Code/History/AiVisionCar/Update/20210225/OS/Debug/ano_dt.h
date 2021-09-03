/*
 * ano_dt.h
 *
 *  Created on: 2021��2��24��
 *      Author: 936305695
 */

#ifndef OS_DEBUG_ANO_DT_H_
#define OS_DEBUG_ANO_DT_H_

#include "device.h"

typedef struct ano_debug
{
/*
 *@ ���º���������������λ���鿴���Ρ�
 *@ �����Ҫ�������������Ϣ����Console.WriteLine();
 *@ ��ֹ������ֵ�ͱ��ʽ
 *@ ����ȷ�ĵ���:
 *     1.SendUint16(0xaa);        //0xaa����Ϊ�ǵ�ַ���ú�����ӵ�ַ0xaa��ȡֵ����������һ����0xaa��
 *     2.uint16_t x = 0��
 *       SendUint16(&(x + 1));    //��Ҫ�����,����ú���ǰ���,����ʱֻ����������ַ��
 *     3.SendUint16(function(a)); //ԭ��ͬ1
 *       uint16_t function(uint16_t a);
 *@ ��ȷ�ĵ���:
 *     1.
 *       uint16_t x = 0;
 *       int16_t y = 0;
 *       x = x + 1;
 *       SendUint16(&x);
 *       SendUint16s(1,&x);
 *       SendUint16s(2,&x,&y);
 *       SendInt16s(2,&x,&y);
 *     2.
 *       uint32_t x = 0;
 *       int32_t  y = 0;
 *       float32_t z = 0.0;
 *       SendUint32s(3,&x,&y,&z); or SendInt32s(3,&x,&y,&z); or SendFloats(3,&x,&y,&z);
 *     3.
 *       float x = 1000.0;
 *       uint16_t y = 0x1234;
 *       sint8_t z = 0x12;
 *       sint32_t i = 0x12345678;
 *       Send("%f%u16%s8%s32",&x,&y,&z,&i);
 *
 */
    void (*Send)(char *fmt,...);

    void (*SendBool)(void *data);

    void (*SendUint8)(void *data);
    void (*SendUint16)(void *data);
    void (*SendUint32)(void *data);

    void (*SendInt8)(void *data);
    void (*SendInt16)(void *data);
    void (*SendInt32)(void *data);

    void (*SendFloat)(void *data);



    void (*SendBools)(uint8_t num,...);

    void (*SendUint8s)(uint8_t num,...);   /* һ����෢��(50 - 5)/1 �� */
    void (*SendUint16s)(uint8_t num,...);  /* һ����෢��(50 - 5)/2 �� */
    void (*SendUint32s)(uint8_t num,...);  /* һ����෢��(50 - 5)/4 �� */

    void (*SendInt8s)(uint8_t num,...);
    void (*SendInt16s)(uint8_t num,...);
    void (*SendInt32s)(uint8_t num,...);

    void (*SendFloats)(uint8_t num,...);
/*
 *@����Test�󣬴��ڳ����������ݣ���ʾ��������(����û�л���)
 *0xaa
 *0xaa
 *0xf1
 *0x4
 *0x44
 *0x7a
 *0x0
 *0x0
 *0x7
 *0xaa
 *0xaa
 *0xf1
 *0xc
 *0xf
 *0xff
 *0xff
 *0xff
 *0xf0
 *0x0
 *0x0
 *0x1
 *0x44
 *0x7a
 *0x0
 *0x0
 *0xc
 */
    void (*Test)(void);

}ano_debug_t;

extern ano_debug_t ANO;

void ANO_Send8Bit(void *data);

void ANO_Send16Bit(void *data);

void ANO_Send32Bit(void *data);


#endif /* OS_DEBUG_ANO_DT_H_ */
