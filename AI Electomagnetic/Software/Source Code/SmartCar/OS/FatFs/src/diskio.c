/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h"			/* FatFs lower layer API */
#include "stdlib.h"
#include "sd.h"

#define SD_CARD	 0  //SD��,����Ϊ0
#define EX_FLASH 1	//�ⲿflash,����Ϊ1

#define FLASH_SECTOR_SIZE 	512			  


//��ô���״̬
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{ 
	return RES_OK;
}  

//��ʼ������
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint8_t res=0;
	switch(pdrv)
	{
		case SD_CARD://SD��
		    res = SD_Initialize(NULL);
  			break;
		default:
			res=1;
	}
	if(res)
	    return  STA_NOINIT;
	else
	    return 0; //��ʼ���ɹ�
} 

//������
//pdrv:���̱��0~9
//*buff:���ݽ��ջ����׵�ַ
//sector:������ַ
//count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
    uint8_t res = 0;
    if (!count)
        return RES_PARERR; //count���ܵ���0�����򷵻ز�������
    switch (pdrv)
    {
        case SD_CARD : //SD��
            while (count--)
            {
                res = SD_ReadSector(sector, 1, buff, count);
                if (res)
                    return res;
                buff += 512;
            }
            break;

        default :
            res = 1;
    }
    //��������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if (res == 0x00)
        return RES_OK;
    else
        return RES_ERROR;
}

//д����
//pdrv:���̱��0~9
//*buff:���������׵�ַ
//sector:������ַ
//count:��Ҫд���������
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
    uint8_t res = 0;
    if (!count)
        return RES_PARERR; //count���ܵ���0�����򷵻ز�������
    switch (pdrv)
    {
        case SD_CARD : //SD��
            while (count--)
            {
                res = SD_WriteSector(sector, 1, (BYTE*)buff, count);
                if (res)
                    return res;
                buff += 512;
            }
            break;
        default :
            res = 1;
    }
    //��������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if (res == 0x00)
        return RES_OK;
    else
        return RES_ERROR;
}
#endif

//�����������Ļ��
//pdrv:���̱��0~9
//ctrl:���ƴ���
//*buff:����/���ջ�����ָ��
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
	if(pdrv==SD_CARD)//SD��
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
		        res = SD_WaitReady() ? RES_NOTRDY : RES_OK;
		        break;
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512;
		        res = RES_OK;
		        break;
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = 1;
		        res = RES_OK;
		        break;
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SD_GetSectorCount();
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}
	else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}
#endif
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0x00210000;//0b 0000000 0001 00001 00000 000000 00000
}			 
//��̬�����ڴ�
void *ff_memalloc (UINT size)			
{
    return (void *)malloc(size);
}
//�ͷ��ڴ�
void ff_memfree (void* mf)		 
{
    free(mf);
}
















