#include <sd.h>

uint8_t  SD_Type=0;//SD��������
uint8_t SD_WaitReady(void);                         //�ȴ�SD��׼��
uint8_t SD_GetResponse(uint8_t Response);                   //�����Ӧ
uint8_t SD_GetCID(uint8_t *cid_data);                     //��SD��CID
uint8_t SD_GetCSD(uint8_t *csd_data);                     //��SD��CSD

uint8_t SD_HardWareReadWriteByte(uint8_t data)
{
    uint8_t rx_data = 0;

    return SPIx.ReadWriteBytes(SD_SPI,&data,&rx_data,1,0);

    return rx_data;
}	  

/*SD����ʼ����ʱ��,��Ҫ����*/
void SD_SPI_SpeedLow(void)
{
    //SPIx.SetSpeed(SD_SPI,SPI_BaudRatePrescaler_256);
}

/*SD������������ʱ��,���Ը�����*/
void SD_SPI_SpeedHigh(void)
{
    //SPIx.SetSpeed(SD_SPI,SPI_BaudRatePrescaler_2);
}


/*ȡ��ѡ��,�ͷ�SPI����*/
void SD_DisSelect(void)
{
    W_SD_SPI_CS(1);
 	SD_SPIReadWriteByte(0xff);//�ṩ�����8��ʱ��
}

uint8_t SD_Select(void)
{
    W_SD_SPI_CS(0);
	if(SD_WaitReady()==0)
	    return 0;//�ȴ��ɹ�
	SD_DisSelect();
	return 1;//�ȴ�ʧ��
}

uint8_t SD_WaitReady(void)
{
	uint32_t t=0;
	do
	{
		if(SD_SPIReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//�ȴ� 
	return 1;
}

uint8_t SD_GetResponse(uint8_t Response)
{
	uint16_t Count=0xFFFF;//�ȴ�����
	while ((SD_SPIReadWriteByte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ
	if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

uint8_t SD_RecvData(uint8_t*buf,uint16_t len)
{			  	  
	if(SD_GetResponse(0xFE))
	    return 1;//�ȴ�SD������������ʼ����0xFE

    while(len--)//��ʼ��������
    {
        *buf=SD_SPIReadWriteByte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    SD_SPIReadWriteByte(0xFF);
    SD_SPIReadWriteByte(0xFF);
    return 0;//��ȡ�ɹ�
}

uint8_t SD_SendBlock(uint8_t*buf,uint8_t cmd)
{	
	uint16_t t;
	if(SD_WaitReady())
	    return 1;//�ȴ�׼��ʧЧ
	SD_SPIReadWriteByte(cmd);
	if(cmd!=0XFD)//���ǽ���ָ��
	{
		for(t=0;t<512;t++)
		    SD_SPIReadWriteByte(buf[t]);//����ٶ�,���ٺ�������ʱ��
	    SD_SPIReadWriteByte(0xFF);//����crc
	    SD_SPIReadWriteByte(0xFF);
		t=SD_SPIReadWriteByte(0xFF);//������Ӧ
		if((t&0x1F)!=0x05)return 2;//��Ӧ����									  					    
	}						 									  					    
    return 0;//д��ɹ�
}

uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
	uint8_t Retry=0;
	SD_DisSelect();//ȡ���ϴ�Ƭѡ
	if(SD_Select())return 0XFF;//ƬѡʧЧ 
	//����
    SD_SPIReadWriteByte(cmd | 0x40);//�ֱ�д������
    SD_SPIReadWriteByte((uint8_t)(arg >> 24));
    SD_SPIReadWriteByte((uint8_t)(arg >> 16));
    SD_SPIReadWriteByte((uint8_t)(arg >> 8));
    SD_SPIReadWriteByte((uint8_t)(arg));
    SD_SPIReadWriteByte(crc);
	if(cmd==CMD12)SD_SPIReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //�ȴ���Ӧ����ʱ�˳�
	Retry=0X1F;
	do
	{
		r1=SD_SPIReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//����״ֵ̬
    return r1;
}

uint8_t SD_GetCID(uint8_t *cid_data)
{
    uint8_t r1;
    //��CMD10�����CID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//����16���ֽڵ�����	 
    }
	SD_DisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}																				  

uint8_t SD_GetCSD(uint8_t *csd_data)
{
    uint8_t r1;
    r1=SD_SendCmd(CMD9,0,0x01);//��CMD9�����CSD
    if(r1==0)
	{
    	r1=SD_RecvData(csd_data, 16);//����16���ֽڵ����� 
    }
	SD_DisSelect();//ȡ��Ƭѡ
	if(r1)
	    return 1;
	else
	    return 0;
}  

uint8_t SD_Initialize(void *config)
{
    uint8_t r1;      // ���SD���ķ���ֵ
    uint16_t retry;  // �������г�ʱ����
    uint8_t buf[4];
	uint16_t i;

#if SD_USING_HARDWARE_CTRL
    SPIx.Init(SD_SPI);
    GPIOx.Init(SD_SPI_CS_GPIO);
#else
    GPIOx.Init(SD_SPI_CS_GPIO);
    GPIOx.Init(SD_SPI_CLK_GPIO);
    GPIOx.Init(SD_SPI_MOSI_GPIO);
    GPIOx.Init(SD_SPI_MOSO_GPIO);
#endif

 	SD_SPI_SpeedLow();	//���õ�����ģʽ 

 	for(i=0;i<10;i++)
 	    SD_SPIReadWriteByte(0XFF);//��������74������
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);//����IDLE״̬
	}while((r1!=0X01) && retry--);
 	SD_Type=0;//Ĭ���޿�
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SD_SPIReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//����CMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//����CMD41
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//����SD2.0���汾��ʼ
				{
					for(i=0;i<4;i++)buf[i]=SD_SPIReadWriteByte(0XFF);//�õ�OCRֵ
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //���CCS
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);		//����CMD55
			r1=SD_SendCmd(CMD41,0,0X01);	//����CMD41
			if(r1<=1)
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{
					SD_SendCmd(CMD55,0,0X01);	//����CMD55
					r1=SD_SendCmd(CMD41,0,0X01);//����CMD41
				}while(r1&&retry--);
			}else//MMC����֧��CMD55+CMD41ʶ��
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);//����CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//����Ŀ�
		}
	}
	SD_DisSelect();//ȡ��Ƭѡ
	SD_SPI_SpeedHigh();//����
	if(SD_Type)
	    return 0;
	else if(r1)
	    return r1;
	return 0xaa;//��������
}

uint8_t SD_ReadSector(uint32_t sector,uint8_t*buf)
{
	uint8_t r1;

	if(SD_Type!=SD_TYPE_V2HC)
	    sector <<= 9;//ת��Ϊ�ֽڵ�ַ


	r1=SD_SendCmd(CMD17,sector,0X01);//������
	if(r1==0)//ָ��ͳɹ�
	{
		r1=SD_RecvData(buf,512);//����512���ֽ�
	}

	SD_DisSelect();//ȡ��Ƭѡ
	return r1;//
}

uint8_t SD_WriteSector(uint32_t sector,uint8_t*buf)
{
	uint8_t r1;
	if(SD_Type!=SD_TYPE_V2HC)
	    sector *= 512;//ת��Ϊ�ֽڵ�ַ

	r1=SD_SendCmd(CMD24,sector,0X01);//������
	if(r1==0)//ָ��ͳɹ�
	{
		r1=SD_SendBlock(buf,0xFE);//д512���ֽ�
	}

	SD_DisSelect();//ȡ��Ƭѡ

	return r1;//
}	

uint32_t SD_GetSectorCount(void)
{
    uint8_t csd[16];
    uint32_t Capacity;
    uint8_t n;
    uint16_t csize;
    //ȡCSD��Ϣ������ڼ����������0
    if(SD_GetCSD(csd)!=0)
        return 0;
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)  //V2.00�Ŀ�
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10;//�õ�������
    }else//V1.XX�Ŀ�
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity= (uint32_t)csize << (n - 9);//�õ�������
    }
    return Capacity;
}
