/*
 * spi.h
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_SPI_H_
#define DRIVER_SYSDRIVER_SPI_H_

#include "LQ_QSPI.h"
#include "LQ_SPI.h"
#include "chipdatatype.h"
#include "systime.h"

#define SPIx_MAX_TIME_OUT TIME_INFINITE

typedef enum
{
     SPI = 2,
     QSPI = 3,
}spi_source_t;

typedef struct
{
        uint32_t     SPIn;
        uint32_t     ClkPin;
        uint32_t     MisoPin;
        uint32_t     MosiPin;
        uint32_t     CsPin;
        uint8_t     Mode;
        spi_source_t Spi_Source;
        uint32_t BaudRate;
}spix_t;

typedef struct
{
        uint8_t (*Init)(spix_t *);
        uint8_t (*ReadWriteBytes)(spix_t *,uint8_t *txData, uint8_t *rxData, uint32_t len,sint64_t time_out);
        Ifx_TickTime Time_Infinite;
}sspi_m;

extern sspi_m SPIx;



#endif /* DRIVER_SYSDRIVER_SPI_H_ */
