/*
 * spi.h
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_SPI_H_
#define DRIVER_SYSDRIVER_SPI_H_

#include "LQ_SPI.h"
#include "platform.h"
#include "systime.h"

#define SPIx_MAX_TIME_OUT 0x7FFFFFFFFFFFFFFFLL

typedef enum
{
     SPI = 2,
     QSPI = 3,
}spi_source_t;

typedef struct
{
        
        LPSPI_Type * Base; 
        
        uint8_t Cs_num; 
        
        uint8_t Mode;
          
        uint32_t BaudRate;
}spix_t;

typedef struct
{
        uint8_t (*Init)(spix_t *);
        uint8_t (*ReadWriteBytes)(spix_t *,uint8_t *txData, uint8_t *rxData, uint32_t len,sint64_t time_out);
        sint64_t Time_Infinite;
}sspi_m;

extern sspi_m SPIx;



#endif /* DRIVER_SYSDRIVER_SPI_H_ */
