#include <sd.h>

//#include "devices_config.h"

uint8_t  SD_Type=0;//SD卡的类型
uint8_t SD_WaitReady(void);                         //等待SD卡准备
uint8_t SD_GetResponse(uint8_t Response);                   //获得相应
uint8_t SD_GetCID(uint8_t *cid_data);                     //读SD卡CID
uint8_t SD_GetCSD(uint8_t *csd_data);                     //读SD卡CSD

uint8_t rx_data = 0;
uint8_t tx_data = 0;

uint8_t SD_HardWareReadWriteByte(uint8_t data)
{
//    uint8_t rx_data = 0;
    tx_data = data;



    SPIx.ReadWriteBytes(SD_SPI,&data,&rx_data,1,0);


//    Screen.ClearLines(Screen.Self,3,4,WHITE);
//    Screen.WriteXLine(Screen.Self,3,"Tx = %u",data);
//    Screen.WriteXLine(Screen.Self,4,"Rx = %u",rx_data);

    return rx_data;
}

uint8_t SD_SoftWareReadWriteByte(uint8_t data)
{
    uint8_t rx_data = 0;

    //SPIx.ReadWriteBytes(SD_SPI,&data,&rx_data,1,0);

    return rx_data;
}

/*SD卡初始化的时候,需要低速*/
void SD_SPI_SpeedLow(void)
{
//    SPIx.SetSpeed(SD_SPI,SPI_BaudRatePrescaler_256);
}

/*SD卡正常工作的时候,可以高速了*/
void SD_SPI_SpeedHigh(void)
{
    SPI_Resources[1].SPIN.BaudRate = 64000000;
    SPIx.Init(&SPI_Resources[1].SPIN);
}


/*取消选择,释放SPI总线*/
void SD_DisSelect(void)
{
    W_SD_SPI_CS(1);
 	SD_SPIReadWriteByte(0xff);//提供额外的8个时钟
}

uint8_t SD_Select(void)
{
    W_SD_SPI_CS(0);
	if(SD_WaitReady()==0)
	    return 0;//等待成功
	SD_DisSelect();
	return 1;//等待失败
}

uint8_t SD_WaitReady(void)
{
	uint32_t t=0;
	do
	{
		if(SD_SPIReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;
	}while(t<0XFFFFFF);//等待

	return 1;
}

uint8_t SD_GetResponse(uint8_t Response)
{
	uint16_t Count=0xFFFF;//等待次数
	while ((SD_SPIReadWriteByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}

uint8_t SD_RecvData(uint8_t* buf,uint16_t len)
{
	if(SD_GetResponse(0xFE))
	    return 1;//等待SD卡发回数据起始令牌0xFE

    while(len--)//开始接收数据
    {
        *buf=SD_SPIReadWriteByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SD_SPIReadWriteByte(0xFF);
    SD_SPIReadWriteByte(0xFF);
    return 0;//读取成功
}

uint8_t SD_SendBlock(uint8_t*buf,uint8_t cmd)
{
	uint16_t t;
	if(SD_WaitReady())
	    return 1;//等待准备失效
	SD_SPIReadWriteByte(cmd);
	if(cmd!=0XFD)//不是结束指令
	{
		for(t=0;t<512;t++)
		    SD_SPIReadWriteByte(buf[t]);//提高速度,减少函数传参时间
	    SD_SPIReadWriteByte(0xFF);//忽略crc
	    SD_SPIReadWriteByte(0xFF);
		t=SD_SPIReadWriteByte(0xFF);//接收响应
		if((t&0x1F)!=0x05)return 2;//响应错误
	}
    return 0;//写入成功
}

uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
	uint8_t Retry=0;
	SD_DisSelect();//取消上次片选
	if(SD_Select())return 0XFF;//片选失效
	//发送
    SD_SPIReadWriteByte(cmd | 0x40);//分别写入命令
    SD_SPIReadWriteByte((uint8_t)(arg >> 24));
    SD_SPIReadWriteByte((uint8_t)(arg >> 16));
    SD_SPIReadWriteByte((uint8_t)(arg >> 8));
    SD_SPIReadWriteByte((uint8_t)(arg));
    SD_SPIReadWriteByte(crc);
	if(cmd==CMD12)SD_SPIReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //等待响应，或超时退出
	Retry=0X1F;
	do
	{
		r1=SD_SPIReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);
	//返回状态值
    return r1;
}

uint8_t SD_GetCID(uint8_t *cid_data)
{
    uint8_t r1;
    //发CMD10命令，读CID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//接收16个字节的数据
    }
	SD_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}

uint8_t SD_GetCSD(uint8_t *csd_data)
{
    uint8_t r1;
    r1=SD_SendCmd(CMD9,0,0x01);//发CMD9命令，读CSD
    if(r1==0)
	{
    	r1=SD_RecvData(csd_data, 16);//接收16个字节的数据
    }
	SD_DisSelect();//取消片选
	if(r1)
	    return 1;
	else
	    return 0;
}

uint8_t SD_Initialize(void *config)
{
    uint8_t r1;      // 存放SD卡的返回值
    uint16_t retry;  // 用来进行超时计数
    uint8_t buf[4];
	uint16_t i;

#if SD_USING_HARDWARE_CTRL


    GPIOx.Init(SD_SPI_CS_GPIO);

    W_SD_SPI_CS(1);

    SPIx.Init(SD_SPI);

    //W_SD_SPI_CS(0);

#else
    GPIOx.Init(SD_SPI_CS_GPIO);
    GPIOx.Init(SD_SPI_CLK_GPIO);
    GPIOx.Init(SD_SPI_MOSI_GPIO);
    GPIOx.Init(SD_SPI_MOSO_GPIO);
#endif

 	SD_SPI_SpeedLow();	//设置到低速模式

 	for(i=0;i<10;i++)
 	    SD_SPIReadWriteByte(0XFF);//发送最少74个脉冲

	retry=20;

	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态

		//Screen.ClearLine(Screen.Self,3,WHITE);
		//Screen.WriteXLine(Screen.Self,3,"r1 = %u",r1);

	}while((r1!=0X01) && retry--);

 	SD_Type=0;//默认无卡

	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SD_SPIReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)buf[i]=SD_SPIReadWriteByte(0XFF);//得到OCR值
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //检查CCS
					else SD_Type=SD_TYPE_V2;
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);		//发送CMD55
			r1=SD_SendCmd(CMD41,0,0X01);	//发送CMD41
			if(r1<=1)
			{
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41
				}while(r1&&retry--);
			}else//MMC卡不支持CMD55+CMD41识别
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					r1=SD_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//错误的卡
		}
	}
	SD_DisSelect();//取消片选
	SD_SPI_SpeedHigh();//高速

	//Screen.WriteXLine(Screen.Self,4,"SD Type : 0x%x",SD_Type);

	if(SD_Type)
	    return 0;
	else if(r1)
	    return r1;
	return 0xaa;//其他错误
}

uint8_t SD_ReadSector(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{
	uint8_t r1;

	if(SD_Type!=SD_TYPE_V2HC)
	    sector <<= 9;//转换为字节地址


	r1=SD_SendCmd(CMD17,sector,0X01);//读命令
	if(r1==0)//指令发送成功
	{
		r1=SD_RecvData(buf,512);//接收512个字节
	}

	SD_DisSelect();//取消片选
	return r1;//
}

uint8_t SD_WriteSector(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{

	uint8_t r1;
	if(SD_Type!=SD_TYPE_V2HC)
	    sector *= 512;//转换为字节地址

	r1=SD_SendCmd(CMD24,sector,0X01);//读命令
	if(r1==0)//指令发送成功
	{
		r1=SD_SendBlock(buf,0xFE);//写512个字节
	}

	SD_DisSelect();//取消片选

	return r1;

#if 0
    uint8_t r1;
    if (SD_Type != SD_TYPE_V2HC)
        sector *= 512; //转换为字节地址

    while (len)
    {
        r1 = SD_SendCmd(CMD24, sector, 0X01); //读命令
        if(r1)
            return r1;
        r1 = SD_SendBlock(buf, 0xFE); //写512个字节
        if(r1)
            return r1;
        len--;
        buf++;
    }


    SD_DisSelect();//取消片选

    return r1;
#endif
}

//uint8_t SD_ReadDisk(uint8_t *buf,u32 sector,uint8_t cnt)
//{
//    uint8_t sta=SD_OK;
//    long long lsector=sector;
//    uint8_t n;
//    lsector<<=9;
//    if((u32)buf%4!=0)
//    {
//        for(n=0;n<cnt;n++)
//        {
//            sta=SD_ReadBlock(SDIO_DATA_BUFFER,lsector+512*n,512);//单个sector的读操作
//
//            memcpy(buf,SDIO_DATA_BUFFER,512);
//
//            buf+=512;
//        }
//    }else
//    {
//        if(cnt==1)sta=SD_ReadBlock(buf,lsector,512);        //单个sector的读操作
//        else sta=SD_ReadMultiBlocks(buf,lsector,512,cnt);//多个sector
//    }
//    return sta;
//}

uint32_t SD_GetSectorCount(void)
{
    uint8_t csd[16];
    uint32_t Capacity;
    uint8_t n;
    uint16_t csize;
    //取CSD信息，如果期间出错，返回0
    if(SD_GetCSD(csd)!=0)
        return 0;
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)  //V2.00的卡
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10;//得到扇区数
    }else//V1.XX的卡
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity= (uint32_t)csize << (n - 9);//得到扇区数
    }
    return Capacity;
}

