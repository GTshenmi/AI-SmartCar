/*
 * eeprom.c
 *
 *  Created on: 2021年4月2日
 *      Author: 936305695
 */
#include "eeprom.h"

/*
 * u32_buf[0] : bytes[3]   uint8_buff[3]
 * u32_buf[0] : bytes[2]   uint8_buff[2]
 * u32_buf[0] : bytes[1]   uint8_buff[1]
 * u32_buf[0] : bytes[0]   uint8_buff[0]
 * */
uint8_t EEPROM_WriteSector(unsigned char sector, unsigned short page, uint8_t * buff, unsigned short len)
{
    uint32_t writebuf[128];
    uint32_t writelen = len / 4 + 1;

    uint32_t i;

    uint32_t full_uint32_num = ((uint32_t)(len/4));

    uint32_t remain_bytes;

    remain_bytes = len - full_uint32_num * 4;

    uint8_t *p = (uint8_t *)writebuf;

    memcpy(writebuf,buff,len * sizeof(char));

//    if(remain_bytes != 0)
//        memset(((uint8_t *)(&writebuf[full_uint32_num * 4]+ 4 - remain_bytes)),'\0',sizeof(char) * (4 - remain_bytes));
//    else
//        memset(((uint8_t *)(&writebuf[full_uint32_num * 4])),'\0',sizeof(char) * (4 - remain_bytes));

    printf("Write:\n");
    for(i = 0 ; i < writelen * 4; i++ )
        printf("%c",*(p + i));

    EEPROM_Write(sector,page,writebuf,writelen);

    return 0;
//    /* 计算扇区起始地址 */
//    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;
//
//    unsigned short endinitSfty_pw;
//
//    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();
//
//    for(i = 0; i < writelen/2; i++)
//    {
//        /* 要写入页地址 */
//        unsigned long pageAddr = sector_addr + (page + i)* IFXFLASH_DFLASH_PAGE_LENGTH;
//
//        IfxFlash_enterPageMode(pageAddr);
//
//        /* 察忙 */
//        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
//
//        /* 写入缓冲区  */
//        IfxFlash_loadPage2X32(pageAddr, writebuf[2*i], writebuf[2*i + 1]);
//
//        /* 写入eeprom  */
//        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
//        IfxFlash_writePage(pageAddr);
//        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);
//
//        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
//    }
//
//    if(writelen%2)
//    {
//        /* 要写入页地址 */
//        unsigned long pageAddr = sector_addr + (page + writelen/2)* IFXFLASH_DFLASH_PAGE_LENGTH;
//
//        IfxFlash_enterPageMode(pageAddr);
//
//        /* 察忙 */
//        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
//
//        /* 写入缓冲区  */
//        IfxFlash_loadPage2X32(pageAddr, writebuf[writelen], 0);
//
//        /* 写入eeprom  */
//        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
//        IfxFlash_writePage(pageAddr);
//        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);
//
//        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
//
//    }
//
//    return 0;
}

uint8_t EEPROM_ReadSector(unsigned char sector, unsigned short page, uint8_t * buff, uint32_t len)
{
    uint32_t readbuf[128];
    uint32_t readlen = len / 4 + 1;

    uint8_t *p = (uint8_t *)readbuf;


    EEPROM_Read(sector,page,readbuf,readlen);

    int i;
//    /* 计算扇区起始地址 */
//    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;
//
//    /* 要读页地址 */
//    volatile unsigned long * pageAddr = (unsigned long*)(sector_addr + page * IFXFLASH_DFLASH_PAGE_LENGTH);
//
//    for(i = 0; i < readlen; i++)
//    {
//        readbuf[i] = *(pageAddr++);
//    }

    printf("\nRead:\n");

    for(i = 0 ; i < readlen * 4; i++ )
        printf("%c",*(p + i));

    memcpy(buff,readbuf,len * sizeof(char));


    return 0;
}

