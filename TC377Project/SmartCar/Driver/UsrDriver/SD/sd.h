#ifndef _MMC_SD_H_
#define _MMC_SD_H_		 

#include "sys_driver.h"

#define SD_SPI &SPI_Resources[1].SPIN
#define SD_SPI_CS_GPIO NULL
#define SD_SPI_CLK_GPIO NULL
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


#define SD_USING_HARDWARE_CTRL 0

#if SD_USING_HARDWARE_CTRL
#define SD_SPIReadWriteByte SD_HardWareReadWriteByte
#else
#define SD_SPIReadWriteByte SD_SoftWareReadWriteByte
#endif

// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

extern uint8_t  SD_Type;			//SD��������

uint8_t SD_Initialize(void *config);

uint8_t SD_ReadSector(uint32_t sector,uint8_t*buf);
uint8_t SD_WriteSector(uint32_t sector,uint8_t*buf);

uint32_t SD_GetSectorCount(void);
 
#endif




