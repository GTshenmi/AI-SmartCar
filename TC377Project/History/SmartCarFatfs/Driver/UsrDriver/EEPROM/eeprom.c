/*
 * eeprom.c
 *
 *  Created on: 2021Äê4ÔÂ2ÈÕ
 *      Author: 936305695
 */
#include "eeprom.h"

/*
 * u32_buf[0] : bytes[3]   uint8_buff[3]
 * u32_buf[0] : bytes[2]   uint8_buff[2]
 * u32_buf[0] : bytes[1]   uint8_buff[1]
 * u32_buf[0] : bytes[0]   uint8_buff[0]
 * */
uint8_t EEPROM_WriteSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len)
{
    uint32_t writebuf[128];
    uint32_t writelen = len / 4 + 1;

  //  uint32_t i;

    memcpy(writebuf,buff,len * sizeof(char));

    printf("Write:\n");

    EEPROM_Write(1,0,writebuf,(uint16_t)writelen);

    return 0;
}

/* D  J     I  s     D  o  g  .
 * 44 4A 20 49 73 20 44 6F 67 2E 00 00
 * 4D 6A 6A 4D 7F 44 64 7F 00 00 00 00
 * */
uint8_t EEPROM_ReadSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len)
{
    uint32_t readbuf[128];
    uint32_t readlen = len / 4 + 1;

   // int i;

    EEPROM_Read(1,0,readbuf,(uint16_t)readlen);

    memcpy(buff,readbuf,len * sizeof(char));


    return 0;
}

uint32_t EEPROM_GetSectorNum(void)
{
    return 12;
}
