/*
 * cc936.h
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */

#ifndef OS_FATFS_SRC_OPTION_CC936_H_
#define OS_FATFS_SRC_OPTION_CC936_H_

#include "ff.h"

WCHAR ff_convert (  /* Converted code, 0 means conversion error */
    WCHAR   chr,    /* Character code to be converted */
    UINT    dir     /* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
WCHAR ff_wtoupper ( /* Upper converted character */
    WCHAR chr       /* Input character */
);


#endif /* OS_FATFS_SRC_OPTION_CC936_H_ */
