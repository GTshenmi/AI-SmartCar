#ifndef __FATFS_SD_H
#define __FATFS_SD_H

#include "diskio.h"

#define SD_SPI           &SPI_Resources[1].SPIN

#define SD_SPI_CS_GPIO   &GPIO_Resources[22].GPION

#define SD_SPI_CLK_GPIO  NULL
#define SD_SPI_MOSI_GPIO NULL
#define SD_SPI_MOSO_GPIO NULL

#define SD_GPIO_Write(gpio,x) GPIOx.Write((gpio),(x))
#define SD_GPIO_Read(gpio)    GPIOx.Read(gpio)

#define W_SD_SPI_CS(x) SD_GPIO_Write((SD_SPI_CS_GPIO),(x))
#define W_SD_SPI_CLK(x) SD_GPIO_Write((SD_SPI_CLK_GPIO),(x))
#define W_SD_SPI_MOSI(x) SD_GPIO_Write((SD_SPI_MOSI_GPIO),(x))
#define W_SD_SPI_MOSO(x) SD_GPIO_Write((SD_SPI_MOSO_GPIO),(x))

#define R_SD_SPI_CS(x) SD_GPIO_Read((SD_SPI_CS_GPIO))
#define R_SD_SPI_CLK(x) SD_GPIO_Read((SD_SPI_CLK_GPIO))
#define R_SD_SPI_MOSI(x) SD_GPIO_Read((SD_SPI_MOSI_GPIO))
#define R_SD_SPI_MOSO(x) SD_GPIO_Read((SD_SPI_MOSO_GPIO))


#define SD_USING_HARDWARE_CTRL 1

#if SD_USING_HARDWARE_CTRL
#define SD_SPIReadWriteByte SD_HardWareReadWriteByte
#else
#define SD_SPIReadWriteByte SD_SoftWareReadWriteByte
#endif

/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     /* GO_IDLE_STATE */
#define CMD1     (0x40+1)     /* SEND_OP_COND */
#define CMD8     (0x40+8)     /* SEND_IF_COND */
#define CMD9     (0x40+9)     /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */

DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#define SPI_TIMEOUT 1000

#endif
