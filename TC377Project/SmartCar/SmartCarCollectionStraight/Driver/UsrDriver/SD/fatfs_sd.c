#include "fatfs_sd.h"

void ______________________________________foo____________________________________________()
{
     //消除警告
}

////#define TRUE  1
////#define FALSE 0
////#define bool BYTE
//
//#include "sys_driver.h"
//
//#include "screen.h"
////#include "diskio.h"
//
//#include "fatfs_sd.h"
//
//
///*
// * SPI 正常
// * 0XFF; 0x01
// *
// * */
////#define SD_CS_GPIO_Port GPIOB
////#define SD_CS_Pin GPIO_PIN_0
//
////extern SPI_HandleTypeDef hspi1;
////extern volatile uint8_t Timer1, Timer2;                    /* 10ms 毵堧嫟 臧愳唽頃橂姅 韮�鞚措ǜ */
//
//static volatile DSTATUS Stat = STA_NOINIT;              /* 霐旍姢韥� 靸來儨 Flag*/
//static uint8_t CardType;                                /* SD 韮�鞛� 0:MMC, 1:SDC, 2:Block addressing */
//static uint8_t PowerFlag = 0;                           /* Power 靸來儨 Flag */
//
//uint8_t SD_HardWareReadWriteByte(uint8_t data)
//{
//    uint8_t rx_data = 0;
//
//    SPIx.ReadWriteBytes(SD_SPI,&data,&rx_data,1,0);
//
////    Screen.ClearLines(Screen.Self,1,2,WHITE);
////    Screen.WriteXLine(Screen.Self,1,"tx = 0x%x",data);
////    Screen.WriteXLine(Screen.Self,2,"rx = 0x%x",rx_data);
////
////    Systime.Delayms(200);
//
//
//    return rx_data;
//}
//
//uint8_t SD_SoftWareReadWriteByte(uint8_t data)
//{
//    uint8_t rx_data = 0;
//
//    //SPIx.ReadWriteBytes(SD_SPI,&data,&rx_data,1,0);
//
//    return rx_data;
//}
//
///* SPI Chip Select */
//static void SELECT(void)
//{
//    //HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
//    W_SD_SPI_CS(0);
//    //SD_GPIO_Write(SD_SPI_CS_GPIO,0);
//}
//
///* SPI Chip Deselect */
//static void DESELECT(void)
//{
//    //HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
//    W_SD_SPI_CS(1);
//    //SD_GPIO_Write(SD_SPI_CS_GPIO,1);
//}
//
///* SPI 雿办澊韯� 鞝勳啞 */
//static void SPI_TxByte(BYTE data)
//{
//    SD_SPIReadWriteByte(data);
////    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
////    HAL_SPI_Transmit(&hspi1, &data, 1, SPI_TIMEOUT);
//}
//
///* SPI 雿办澊韯� 靻§垬鞁� 毽劥順� 頃垬 */
//static uint8_t SPI_RxByte(void)
//{
//    uint8_t data = 0xff;
//
//    return SD_SPIReadWriteByte(data);
//
////  uint8_t dummy, data;
////  dummy = 0xFF;
////  data = 0;
////
////  while ((HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY));
////  HAL_SPI_TransmitReceive(&hspi1, &dummy, &data, 1, SPI_TIMEOUT);
////
////  return data;
//}
//
///* SPI 雿办澊韯� 靻§垬鞁� 韽澑韯绊槙 頃垬 */
//static void SPI_RxBytePtr(uint8_t *buff)
//{
//    *buff = SPI_RxByte();
//}
//
///* SD旃措摐 Ready 雽�旮� */
//static uint8_t SD_ReadyWait(void)
//{
//  uint8_t res;
//  uint32_t Timer2;
//
//  /* 500ms 旃挫毚韯� 欷�牍� */
//  Timer2 = 50;
//
//  SPI_RxByte();
//
//  do
//  {
//
//    /* 0xFF 臧掛澊 靾橃嫚霅� 霑� 旯岇 SPI 韱奠嫚 */
//    res = SPI_RxByte();
//    Systime.Delayms(10);
//    Timer2--;
//
////    Screen.ClearLine(Screen.Self,1,WHITE);
////    Screen.WriteXLine(Screen.Self,1,"Res = %u ",res);
//
//  } while ((res != 0xFF) && Timer2);
//
//  return res;
//}
//
///* 鞝勳洂 旒滉赴 */
//static void SD_PowerOn(void)
//{
//  uint8_t cmd_arg[6];
//  uint32_t Count = 0x1FFF;
//
//  /* Deselect 靸來儨鞐愳劀 SPI 氅旍嫓歆�毳� 鞝勳啞頃橃棳 雽�旮办儊韮滊 毵岆摖雼�. */
//  DESELECT();
//
//  //for(int i = 0; i < 10; i++)
//  for(int i = 0 ; i < 74 ; i++)
//  {
//    SPI_TxByte(0xFF);
//  }
//
//  /* SPI Chips Select */
//  SELECT();
//
//  /* 齑堦赴 GO_IDLE_STATE 靸來儨 鞝勴櫂 */
//  cmd_arg[0] = (CMD0 | 0x40);
//  cmd_arg[1] = 0;
//  cmd_arg[2] = 0;
//  cmd_arg[3] = 0;
//  cmd_arg[4] = 0;
//  cmd_arg[5] = 0x95;
//
//  /* 氇呺牴 鞝勳啞 */
//  for (int i = 0; i < 6; i++)
//  {
//    SPI_TxByte(cmd_arg[i]);
//  }
//
//  /* 鞚戨嫷 雽�旮� */
//  while ((SPI_RxByte() != 0x01) && Count)
//  {
//     Count--;
//  }
//
//  DESELECT();
//
//  SPI_TxByte(0XFF);
//
//  PowerFlag = 1;
//}
//
///* 鞝勳洂 雭勱赴 */
//static void SD_PowerOff(void)
//{
//  PowerFlag = 0;
//}
//
///* 鞝勳洂 靸來儨 頇曥澑 */
//static uint8_t SD_CheckPower(void)
//{
//  /*  0=off, 1=on */
//  return PowerFlag;
//}
//
///* 雿办澊韯� 韺偡 靾橃嫚 */
//static bool SD_RxDataBlock(BYTE *buff, UINT btr)
//{
//  uint8_t token;
//
//  uint32_t Timer1;
//  /* 100ms 韮�鞚措ǜ */
//  Timer1 = 10;
//
//  /* 鞚戨嫷 雽�旮� */
//  do
//  {
//    token = SPI_RxByte();
//    Systime.Delayms(10);
//    Timer1--;
//  } while((token == 0xFF) && Timer1);
//
//  /* 0xFE 鞚挫櫢 Token 靾橃嫚 鞁� 鞐愲煬 觳橂Μ */
//  if(token != 0xFE)
//    return FALSE;
//
//  /* 氩勴嵓鞐� 雿办澊韯� 靾橃嫚 */
//  do
//  {
//    SPI_RxBytePtr(buff++);
//    SPI_RxBytePtr(buff++);
//  } while(btr -= 2);
//
//  SPI_RxByte(); /* CRC 氍挫嫓 */
//  SPI_RxByte();
//
//  return TRUE;
//}
//
///* 雿办澊韯� 鞝勳啞 韺偡 */
//#if _READONLY == 0
//static bool SD_TxDataBlock(const BYTE *buff, BYTE token)
//{
//  uint8_t resp, wc;
//  uint8_t i = 0;
//
//  /* SD旃措摐 欷�牍� 雽�旮� */
//  if (SD_ReadyWait() != 0xFF)
//    return FALSE;
//
//  /* 韱犿伆 鞝勳啞 */
//  SPI_TxByte(token);
//
//  /* 雿办澊韯� 韱犿伆鞚� 瓴届毎 */
//  if (token != 0xFD)
//  {
//    wc = 0;
//
//    /* 512 氚旍澊韸� 雿办澊韯� 鞝勳啞 */
//    do
//    {
//      SPI_TxByte(*buff++);
//      SPI_TxByte(*buff++);
//    } while (--wc);
//
//    SPI_RxByte();       /* CRC 氍挫嫓 */
//    SPI_RxByte();
//
//    /* 雿办澊韸� 鞚戨嫷 靾橃嫚 */
//    while (i <= 64)
//    {
//      resp = SPI_RxByte();
//
//      /* 鞐愲煬 鞚戨嫷 觳橂Μ */
//      if ((resp & 0x1F) == 0x05)
//        break;
//
//      i++;
//    }
//
//    /* SPI 靾橃嫚 氩勴嵓 Clear */
//    while (SPI_RxByte() == 0);
//  }
//
//  if ((resp & 0x1F) == 0x05)
//    return TRUE;
//  else
//    return FALSE;
//}
//#endif /* _READONLY */
//
///* CMD 韺偡 鞝勳啞 */
//static BYTE SD_SendCmd(BYTE cmd, DWORD arg)
//{
//  uint8_t crc, res;
//
//  /* SD旃措摐 雽�旮� */
//  if (SD_ReadyWait() != 0xFF)
//  {
////      Screen.ClearLine(Screen.Self,1,WHITE);
////      Screen.WriteXLine(Screen.Self,1,"Wait Is ");
//      return 0xFF;
//  }
//  else
//  {
////      Screen.ClearLine(Screen.Self,1,WHITE);
////      Screen.WriteXLine(Screen.Self,1,"Wait Is 0xFF.");
//  }
//
//  /* 氇呺牴 韺偡 鞝勳啞 */
//  SPI_TxByte(cmd); 			/* Command */
//  SPI_TxByte((BYTE) (arg >> 24)); 	/* Argument[31..24] */
//  SPI_TxByte((BYTE) (arg >> 16)); 	/* Argument[23..16] */
//  SPI_TxByte((BYTE) (arg >> 8)); 	/* Argument[15..8] */
//  SPI_TxByte((BYTE) arg); 		/* Argument[7..0] */
//
//  /* 氇呺牴氤� CRC 欷�牍� */
//  crc = 0;
//  if (cmd == CMD0)
//    crc = 0x95; /* CRC for CMD0(0) */
//
//  if (cmd == CMD8)
//    crc = 0x87; /* CRC for CMD8(0x1AA) */
//
//  /* CRC 鞝勳啞 */
//  SPI_TxByte(crc);
//
//  /* CMD12 Stop Reading 氇呺牴鞚� 瓴届毎鞐愲姅 鞚戨嫷 氚旍澊韸� 頃橂倶毳� 氩勲Π雼� */
//  if (cmd == CMD12)
//    SPI_RxByte();
//
//  /* 10須� 雮挫棎 鞝曥儊 雿办澊韯半ゼ 靾橃嫚頃滊嫟. */
//  uint8_t n = 10;
//  do
//  {
//    res = SPI_RxByte();
//  } while ((res & 0x80) && --n);
//
//  return res;
//}
//
///*-----------------------------------------------------------------------
//  fatfs鞐愳劀 靷毄霅橂姅 Global 頃垬霌�
//  user_diskio.c 韺岇澕鞐愳劀 靷毄霅滊嫟.
//-----------------------------------------------------------------------*/
//
///* SD旃措摐 齑堦赴頇� */
//DSTATUS SD_disk_initialize(BYTE drv)
//{
//  uint8_t n, type, ocr[4];
//
//  GPIOx.Init(SD_SPI_CS_GPIO);
//
//  //W_SD_SPI_CS(1);
//
//  SPIx.Init(SD_SPI);
//
//  /* 頃滌毳橃潣 霌滊澕鞚措笇毵� 歆�鞗� */
//  if(drv)
//    return STA_NOINIT;
//
//  /* SD旃措摐 氙胳偨鞛� */
//  if(Stat & STA_NODISK)
//    return Stat;
//
//  /* SD旃措摐 Power On */
//  SD_PowerOn();
//
//  /*ADD Deley 74 Clocks.*/
//
//  DESELECT();
//
//  for(int i = 0 ; i < 74 ; i++)
//  {
//    SPI_TxByte(0xFF);
//  }
//
//
//  /* SPI 韱奠嫚鞚� 鞙勴暣 Chip Select */
//  SELECT();
//
//  for(int i = 0 ; i < 74 ; i++)
//  {
//    SPI_TxByte(0xFF);
//  }
//
//  /* SD旃措摐 韮�鞛呺硛靾� 齑堦赴頇� */
//  type = 0;
//  uint32_t Timer1;
//
//  /* Idle 靸來儨 歆勳瀰 */
//  if (SD_SendCmd(CMD0, 0) == 1)
//  {
//    /* 韮�鞚措ǜ 1齑� 靹れ爼 */
//    Timer1 = 100;
//
//    /* SD 鞚疙劙韼橃澊鞀� 霃欖瀾 臁瓣贝 頇曥澑 */
//    if (SD_SendCmd(CMD8, 0x1AA) == 1)
//    {
//      /* SDC Ver2+ */
//      for (n = 0; n < 4; n++)
//      {
//        ocr[n] = SPI_RxByte();
//      }
//
//      if (ocr[2] == 0x01 && ocr[3] == 0xAA)
//      {
//        /* 2.7-3.6V 鞝勳晻氩旍渼 霃欖瀾 */
//        do {
//          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0)
//            break; /* ACMD41 with HCS bit */
//          Systime.Delayms(10);
//          Timer1--;
//        } while (Timer1);
//
//        if (Timer1 && SD_SendCmd(CMD58, 0) == 0)
//        {
//          /* Check CCS bit */
//          for (n = 0; n < 4; n++)
//          {
//            ocr[n] = SPI_RxByte();
//          }
//
//          type = (ocr[0] & 0x40) ? 6 : 2;
//        }
//      }
//    }
//    else
//    {
//      /* SDC Ver1 or MMC */
//      type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? 2 : 1; /* SDC : MMC */
//
//      do {
//        if (type == 2)
//        {
//          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0)
//            break; /* ACMD41 */
//        }
//        else
//        {
//          if (SD_SendCmd(CMD1, 0) == 0)
//            break; /* CMD1 */
//        }
//        Systime.Delayms(10);
//        Timer1--;
//      } while (Timer1);
//
//      if (!Timer1 || SD_SendCmd(CMD16, 512) != 0)
//      {
//        /* 敫旊煭 旮胳澊 靹犿儩 */
//        type = 0;
//      }
//    }
//  }
//
//  CardType = type;
//
//  DESELECT();
//
//  SPI_RxByte(); /* Idle 靸來儨 鞝勴櫂 (Release DO) */
//
//  if (type)
//  {
//    /* Clear STA_NOINIT */
//    Stat &= ~STA_NOINIT;
//  }
//  else
//  {
//    /* Initialization failed */
//    SD_PowerOff();
//  }
//
//  return Stat;
//}
//
///* 霐旍姢韥� 靸來儨 頇曥澑 */
//DSTATUS SD_disk_status(BYTE drv)
//{
//  if (drv)
//    return STA_NOINIT;
//
//  return Stat;
//}
//
///* 靹鬼劙 鞚疥赴 */
//DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
//{
//  if (pdrv || !count)
//    return RES_PARERR;
//
//  if (Stat & STA_NOINIT)
//    return RES_NOTRDY;
//
//  if (!(CardType & 4))
//    sector *= 512;      /* 歆�鞝� sector毳� Byte addressing 雼渼搿� 氤�瓴� */
//
//  SELECT();
//
//  if (count == 1)
//  {
//    /* 鞁标竴 敫旊 鞚疥赴 */
//    if ((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512))
//      count = 0;
//  }
//  else
//  {
//    /* 雼れ 敫旊 鞚疥赴 */
//    if (SD_SendCmd(CMD18, sector) == 0)
//    {
//      do {
//        if (!SD_RxDataBlock(buff, 512))
//          break;
//
//        buff += 512;
//      } while (--count);
//
//      /* STOP_TRANSMISSION, 氇摖 敫旊煭鞚� 雼� 鞚届潃 頉�, 鞝勳啞 欷戩 鞖旍箔 */
//      SD_SendCmd(CMD12, 0);
//    }
//  }
//
//  DESELECT();
//  SPI_RxByte(); /* Idle 靸來儨(Release DO) */
//
//  return count ? RES_ERROR : RES_OK;
//}
//
///* 靹鬼劙 鞊瓣赴 */
//#if _READONLY == 0
//DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
//{
//  if (pdrv || !count)
//    return RES_PARERR;
//
//  if (Stat & STA_NOINIT)
//    return RES_NOTRDY;
//
//  if (Stat & STA_PROTECT)
//    return RES_WRPRT;
//
//  if (!(CardType & 4))
//    sector *= 512; /* 歆�鞝� sector毳� Byte addressing 雼渼搿� 氤�瓴� */
//
//  SELECT();
//
//  if (count == 1)
//  {
//    /* 鞁标竴 敫旊 鞊瓣赴 */
//    if ((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE))
//      count = 0;
//  }
//  else
//  {
//    /* 雼れ 敫旊 鞊瓣赴 */
//    if (CardType & 2)
//    {
//      SD_SendCmd(CMD55, 0);
//      SD_SendCmd(CMD23, count); /* ACMD23 */
//    }
//
//    if (SD_SendCmd(CMD25, sector) == 0)
//    {
//      do {
//        if(!SD_TxDataBlock(buff, 0xFC))
//          break;
//
//        buff += 512;
//      } while (--count);
//
//      if(!SD_TxDataBlock(0, 0xFD))
//      {
//        count = 1;
//      }
//    }
//  }
//
//  DESELECT();
//  SPI_RxByte();
//
//  return count ? RES_ERROR : RES_OK;
//}
//#endif /* _READONLY */
//
///* 旮绊儉 頃垬 */
//DRESULT SD_disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
//{
//  DRESULT res;
//  BYTE n, csd[16], *ptr = buff;
//  WORD csize;
//
//  if (drv)
//    return RES_PARERR;
//
//  res = RES_ERROR;
//
//  if (ctrl == CTRL_POWER)
//  {
//    switch (*ptr)
//    {
//    case 0:
//      if (SD_CheckPower())
//        SD_PowerOff();          /* Power Off */
//      res = RES_OK;
//      break;
//    case 1:
//      SD_PowerOn();             /* Power On */
//      res = RES_OK;
//      break;
//    case 2:
//      *(ptr + 1) = (BYTE) SD_CheckPower();
//      res = RES_OK;             /* Power Check */
//      break;
//    default:
//      res = RES_PARERR;
//    }
//  }
//  else
//  {
//    if (Stat & STA_NOINIT)
//      return RES_NOTRDY;
//
//    SELECT();
//
//    switch (ctrl)
//    {
//    case GET_SECTOR_COUNT:
//      /* SD旃措摐 雮� Sector鞚� 臧滌垬 (DWORD) */
//      if ((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16))
//      {
//        if ((csd[0] >> 6) == 1)
//        {
//          /* SDC ver 2.00 */
//          csize = csd[9] + ((WORD) csd[8] << 8) + 1;
//          *(DWORD*) buff = (DWORD) csize << 10;
//        }
//        else
//        {
//          /* MMC or SDC ver 1.XX */
//          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
//          csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
//          *(DWORD*) buff = (DWORD) csize << (n - 9);
//        }
//
//        res = RES_OK;
//      }
//      break;
//
//    case GET_SECTOR_SIZE:
//      /* 靹鬼劙鞚� 雼渼 韥赴 (WORD) */
//      *(WORD*) buff = 512;
//      res = RES_OK;
//      break;
//
//    case CTRL_SYNC:
//      /* 鞊瓣赴 霃欔赴頇� */
//      if (SD_ReadyWait() == 0xFF)
//        res = RES_OK;
//      break;
//
//    case MMC_GET_CSD:
//      /* CSD 鞝曤炒 靾橃嫚 (16 bytes) */
//      if (SD_SendCmd(CMD9, 0) == 0 && SD_RxDataBlock(ptr, 16))
//        res = RES_OK;
//      break;
//
//    case MMC_GET_CID:
//      /* CID 鞝曤炒 靾橃嫚 (16 bytes) */
//      if (SD_SendCmd(CMD10, 0) == 0 && SD_RxDataBlock(ptr, 16))
//        res = RES_OK;
//      break;
//
//    case MMC_GET_OCR:
//      /* OCR 鞝曤炒 靾橃嫚 (4 bytes) */
//      if (SD_SendCmd(CMD58, 0) == 0)
//      {
//        for (n = 0; n < 4; n++)
//        {
//          *ptr++ = SPI_RxByte();
//        }
//
//        res = RES_OK;
//      }
//      else
//        res = RES_PARERR;
//      break;
//    default:
//      res = RES_PARERR;
//    }
//
//    DESELECT();
//    SPI_RxByte();
//  }
//
//  return res;
//}
