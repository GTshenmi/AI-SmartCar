#ifndef OS_DEBUG_ANO_DT_H_
#define OS_DEBUG_ANO_DT_H_

#include "device.h"

typedef struct ano_debug
{

    void (*SendBool)(void *data);

    void (*SendUint8)(void *data);
    void (*SendUint16)(void *data);
    void (*SendUint32)(void *data);

    void (*SendInt8)(void *data);
    void (*SendInt16)(void *data);
    void (*SendInt32)(void *data);

    void (*SendFloat)(void *data);

}ano_debug_t;

extern ano_debug_t ANO;

void ANO_Send8Bit(void *data);

void ANO_Send16Bit(void *data);

void ANO_Send32Bit(void *data);

#endif /* OS_DEBUG_ANO_DT_H_ */
