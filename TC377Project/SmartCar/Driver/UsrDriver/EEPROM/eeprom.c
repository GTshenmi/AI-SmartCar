/*
 * eeprom.c
 *
 *  Created on: 2021��4��2��
 *      Author: 936305695
 */
#include "eeprom.h"


/*************************************************************************
*  �������ƣ�void EEPROM_EraseSector(unsigned char sector)
*  ����˵����eeprom��������
*  ����˵����sector   ��  ������Χ  0-11
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��EEPROM_EraseSector(0);   //��������0
*************************************************************************/
void EEPROM_EraseSector(unsigned char sector)
{
    /* ����������ʼ��ַ */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    /* �������� */
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    IfxFlash_eraseSector(sector_addr);
    IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

    IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
}

/*************************************************************************
*  �������ƣ�void EEPROM_EraseSector(unsigned char sector)
*  ����˵����eepromд������
*  ����˵����
  * @param    sector   ��  ����   ��Χ  0-11
  * @param    page     ��  ҳ     ��Χ  0-1023
  * @param    buff     ��  ���д������
  * @param    len      ��  д�����ݸ���
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��EEPROM_Write(0, 0, u32wBuff, 24);   //������0  ��0ҳ  д��24��unsigned long����
*************************************************************************/
void EEPROM_Write(unsigned char sector, unsigned short page, unsigned long * buff, unsigned short len)
{
    int i;
    /* ����������ʼ��ַ */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    unsigned short endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

    for(i = 0; i < len/2; i++)
    {
        /* Ҫд��ҳ��ַ */
        unsigned long pageAddr = sector_addr + (page + i)* IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* ��æ */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* д�뻺����  */
        IfxFlash_loadPage2X32(pageAddr, buff[2*i], buff[2*i + 1]);

        /* д��eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
    }

    if(len%2)
    {
        /* Ҫд��ҳ��ַ */
        unsigned long pageAddr = sector_addr + (page + len/2)* IFXFLASH_DFLASH_PAGE_LENGTH;

        IfxFlash_enterPageMode(pageAddr);

        /* ��æ */
        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

        /* д�뻺����  */
        IfxFlash_loadPage2X32(pageAddr, buff[len], 0);

        /* д��eeprom  */
        IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
        IfxFlash_writePage(pageAddr);
        IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

        IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

    }
}


/*************************************************************************
*  �������ƣ�void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
*  ����˵����eeprom��ȡ
*  ����˵����
  * @param    sector   ��  ����   ��Χ  0-11
  * @param    page     ��  ҳ     ��Χ  0-1023
  * @param    buff     ��  ���д������
  * @param    len      ��  д�����ݸ���
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��EEPROM_Read(0, 0, u32rBuff, 24);   //��ȡ����0  ��0ҳ  ��ʼ��24��unsigned long����
*************************************************************************/
void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
{
    int i;
    /* ����������ʼ��ַ */
    unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

    /* Ҫ��ҳ��ַ */
    volatile unsigned long * pageAddr = (unsigned long*)(sector_addr + page * IFXFLASH_DFLASH_PAGE_LENGTH);

    for(i = 0; i < len; i++)
    {
        rbuff[i] = *(pageAddr++);
    }
}


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
