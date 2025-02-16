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

#define SD_CARD	 0  //SD卡,卷标为0
#define EX_FLASH 1	//外部flash,卷标为1

#define FLASH_SECTOR_SIZE 	512			  


//获得磁盘状态
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{ 
	return RES_OK;
}  

//初始化磁盘
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint8_t res=0;
	switch(pdrv)
	{
		case SD_CARD://SD卡
		    res = SD_Initialize(NULL);
  			break;
		default:
			res=1;
	}
	if(res)
	    return  STA_NOINIT;
	else
	    return 0; //初始化成功
} 

//读扇区
//pdrv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
    uint8_t res = 0;
    if (!count)
        return RES_PARERR; //count不能等于0，否则返回参数错误
    switch (pdrv)
    {
        case SD_CARD : //SD卡
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
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if (res == 0x00)
        return RES_OK;
    else
        return RES_ERROR;
}

//写扇区
//pdrv:磁盘编号0~9
//*buff:发送数据首地址
//sector:扇区地址
//count:需要写入的扇区数
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
        return RES_PARERR; //count不能等于0，否则返回参数错误
    switch (pdrv)
    {
        case SD_CARD : //SD卡
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
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if (res == 0x00)
        return RES_OK;
    else
        return RES_ERROR;
}
#endif

//其他表参数的获得
//pdrv:磁盘编号0~9
//ctrl:控制代码
//*buff:发送/接收缓冲区指针
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
	if(pdrv==SD_CARD)//SD卡
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
	else res=RES_ERROR;//其他的不支持
    return res;
}
#endif
//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0x00210000;//0b 0000000 0001 00001 00000 000000 00000
}			 
//动态分配内存
void *ff_memalloc (UINT size)			
{
    return (void *)malloc(size);
}
//释放内存
void ff_memfree (void* mf)		 
{
    free(mf);
}

















