/*
 * eeprom.c
 *
 *  Created on: 2021年4月2日
 *      Author: 936305695
 */
#include "eeprom.h"

#define MAX_SECTION_LENGTH 1024
#define MAX_PAGE_LENGTH 12

/*************************************************************************
*  函数名称：void EEPROM_EraseSector(unsigned char sector)
*  功能说明：eeprom擦出扇区
*  参数说明：sector   ：  扇区范围  0-11
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：EEPROM_EraseSector(0);   //擦出扇区0
*************************************************************************/
void EEPROM_EraseSector(unsigned char sector)
{
#if defined(Chip) && (Chip == TC377 || Chip == TC264)

    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    /* 擦出扇区 */
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    IfxFlash_eraseSector(sector_addr);
    IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

    IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
	
#endif
}

/*************************************************************************
*  函数名称：void EEPROM_EraseSector(unsigned char sector)
*  功能说明：eeprom写入数据
*  参数说明：
  * @param    sector   ：  扇区   范围  0-11
  * @param    page     ：  页     范围  0-1023
  * @param    buff     ：  存放写入数据
  * @param    len      ：  写入数据个数
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：EEPROM_Write(0, 0, u32wBuff, 24);   //向扇区0  第0页  写入24个unsigned long数据
*************************************************************************/
void EEPROM_Write(unsigned char sector, unsigned short page, unsigned long * buff, unsigned short len)
{
#if defined(Chip) && (Chip == TC377 || Chip == TC264)
    int i;
    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    for(i = 0; i < len/2; i++)
    {
        /* 要写入页地址 */
        unsigned long pageAddr = sector_addr + (page + i)* IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* 察忙 */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* 写入缓冲区  */
        IfxFlash_loadPage2X32(pageAddr, buff[2*i], buff[2*i + 1]);

        /* 写入eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
    }

    if(len%2)
    {
        /* 要写入页地址 */
        unsigned long pageAddr = sector_addr + (page + len/2)* IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* 察忙 */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* 写入缓冲区  */
        IfxFlash_loadPage2X32(pageAddr, buff[len], 0);

        /* 写入eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

    }
#endif	
}


/*************************************************************************
 *  函数名称：void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
 *  功能说明：eeprom读取
 *  参数说明：
 * @param    sector   ：  扇区   范围  0-11
 * @param    page     ：  页     范围  0-1023
 * @param    buff     ：  存放写入数据
 * @param    len      ：  写入数据个数
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  备    注：EEPROM_Read(0, 0, u32rBuff, 24);   //读取扇区0  第0页  开始的24个unsigned long数据
*************************************************************************/
void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
{
#if defined(Chip) && (Chip == TC377 || Chip == TC264)
    int i;
    /* 计算扇区起始地址 */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    /* 要读页地址 */
    volatile unsigned long * pageAddr = (unsigned long*)(sector_addr + page * IFXFLASH_DFLASH_PAGE_LENGTH);

    for(i = 0; i < len; i++)
    {
        rbuff[i] = *(pageAddr++);
    }
#endif
}


/*
 * u32_buf[0] : bytes[3]   uint8_buff[3]
 * u32_buf[0] : bytes[2]   uint8_buff[2]
 * u32_buf[0] : bytes[1]   uint8_buff[1]
 * u32_buf[0] : bytes[0]   uint8_buff[0]
 * */
uint8_t EEPROM_WriteSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len)
{
#if defined(Chip) && (Chip == TC377 || Chip == TC264)
    uint32_t writebuf[MAX_PAGE_LENGTH];
    uint32_t writelen = len / 4 + 1;

  //  uint32_t i;

    memcpy(writebuf,buff,len * sizeof(char));

    printf("Write:\n");

    EEPROM_Write(1,0,writebuf,(uint16_t)writelen);

    return 0;
#endif
}

/* D  J     I  s     D  o  g  .
 * 44 4A 20 49 73 20 44 6F 67 2E 00 00
 * 4D 6A 6A 4D 7F 44 64 7F 00 00 00 00
 * */
uint8_t EEPROM_ReadSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len)
{
#if defined(Chip) && (Chip == TC377 || Chip == TC264)
    uint32_t readbuf[MAX_PAGE_LENGTH];
    uint32_t readlen = len / 4 + 1;

   // int i;

    EEPROM_Read(1,0,readbuf,(uint16_t)readlen);

    memcpy(buff,readbuf,len * sizeof(char));


    return 0;
#endif
}

uint32_t EEPROM_GetSectorNum(void)
{
    return 12;
}

void* EEPROMGetData(uint32_t dataLocation,void * dataPointer) {
	
	uint32_t readBuffer[MAX_SECTION_LENGTH];
	EEPROM_Read(0, 0, readBuffer, MAX_SECTION_LENGTH);
	*(uint32_t *)dataPointer = readBuffer[dataLocation];
	return dataPointer;
}


void EEPROMSetData(uint32_t dataLocation, void * data) {
	uint32_t readBuffer[MAX_SECTION_LENGTH];
	EEPROM_Read(0, 0, readBuffer, MAX_SECTION_LENGTH);
	readBuffer[dataLocation] = *(uint32_t*)data;

	EEPROM_EraseSector(0);
	EEPROM_Write(0,0,readBuffer,MAX_SECTION_LENGTH);
}


#define TRY_LENGTH 1024
void TryEEPROM(void){
	uint32_t readBuffer[TRY_LENGTH];
	EEPROM_Read(0, 0, readBuffer, TRY_LENGTH);
	for (int i = 0; i < TRY_LENGTH; ++i) {
		printf("%d:%lu ",i,readBuffer[i]);
	}
	printf("\n");

	memset(readBuffer,0,TRY_LENGTH);
	for (int i = 0; i < TRY_LENGTH; ++i) {
		readBuffer[i] = i+2;
	}
	EEPROM_EraseSector(0);
	EEPROM_Write(0,0,readBuffer,TRY_LENGTH);

	memset(readBuffer,0,TRY_LENGTH);
	EEPROM_Read(0, 0, readBuffer, TRY_LENGTH);
	for (int i = 0; i < TRY_LENGTH; ++i) {
		printf("%d:%lu ",i,readBuffer[i]);
	}
	printf("\n");
}
