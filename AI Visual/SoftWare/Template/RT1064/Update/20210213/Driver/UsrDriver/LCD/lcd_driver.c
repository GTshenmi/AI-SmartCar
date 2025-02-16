
/*
 * lcd_driver.c
 *
 *  Created on: 2020年12月6日
 *      Author: GT_shenmi
 */
#if 1
#include <lcd_driver.h>
uint8_t LCD_Type = 1;
/**
    *
  * @brief  LCD 初始化
    * @param    无
  * @retval 无
  */
uint8_t LCD_Init(void *config)
{
    uint8_t *type = (uint8_t *)config;
#if !USING_HARDWARE_CONTROLER

        //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
        GPIOx.Init(LCD_DC_GPIO);
        GPIOx.Init(LCD_RST_GPIO);
        GPIOx.Init(LCD_SDA_GPIO);
        GPIOx.Init(LCD_SCL_GPIO);
        GPIOx.Init(LCD_CS_GPIO);
#else
        LCD_HardWareSPIInit(9600);
        GPIO_PinInit(LCD_DC_GPIO_PIN,GPO,0);
        GPIO_PinInit(LCD_RST_GPIO_PIN,GPO,0);

#endif
        LCD_RST(0);
        LCD_delayms(50);
        LCD_RST(1);
        LCD_delayms(50);
        LCD_WriteCmd(0x11);                 //关闭睡眠，振荡器工作
        LCD_delayms(10);
        LCD_WriteCmd(0x3a);                 //每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
        LCD_WriteByte(0x55);
        LCD_WriteCmd(0x26);
        LCD_WriteByte(0x04);
        LCD_WriteCmd(0xf2);                     //Driver Output Control(1)
        LCD_WriteByte(0x01);
        LCD_WriteCmd(0xe0);                     //Driver Output Control(1)
        LCD_WriteByte(0x3f);
        LCD_WriteByte(0x25);
        LCD_WriteByte(0x1c);
        LCD_WriteByte(0x1e);
        LCD_WriteByte(0x20);
        LCD_WriteByte(0x12);
        LCD_WriteByte(0x2a);
        LCD_WriteByte(0x90);
        LCD_WriteByte(0x24);
        LCD_WriteByte(0x11);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteCmd(0xe1);              //Driver Output Control(1)
        LCD_WriteByte(0x20);
        LCD_WriteByte(0x20);
        LCD_WriteByte(0x20);
        LCD_WriteByte(0x20);
        LCD_WriteByte(0x05);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x15);
        LCD_WriteByte(0xa7);
        LCD_WriteByte(0x3d);
        LCD_WriteByte(0x18);
        LCD_WriteByte(0x25);
        LCD_WriteByte(0x2a);
        LCD_WriteByte(0x2b);
        LCD_WriteByte(0x2b);
        LCD_WriteByte(0x3a);
        LCD_WriteCmd(0xb1);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteCmd(0xb4);
        LCD_WriteByte(0x07);
        LCD_WriteCmd(0xc0);
        LCD_WriteByte(0x0a);
        LCD_WriteByte(0x02);
        LCD_WriteCmd(0xc1);
        LCD_WriteByte(0x02);
        LCD_WriteCmd(0xc5);
        LCD_WriteByte(0x4f);
        LCD_WriteByte(0x5a);
        LCD_WriteCmd(0xc7);
        LCD_WriteByte(0x40);
        LCD_WriteCmd(0x2a);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0xa8);
        LCD_WriteCmd(0x2b);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);
        LCD_WriteByte(0xb3);
        LCD_WriteCmd(0x36);
        if(type)
        LCD_WriteByte(0xC0);                //竖屏显示
        else
        LCD_WriteByte(0xA0);                  //横屏显示

        LCD_WriteCmd(0xb7);        //LCD Driveing control
        LCD_WriteByte(0x00);
        LCD_WriteCmd(0x29);         //开启屏幕显示
        LCD_WriteCmd(0x2c);         //设置为LCD接收数据/命令模式
        //LCD_Clear(WHITE);
        return 0;
}
void LCD_DeInit()
{
        LCD_CS(1);
}


#if !USING_HARDWARE_CONTROLER
/**
    *
    * @brief    向LCD发送一个字节
    * @param    data：要发送的字节
  * @retval 无
    *   @attention: 软件模拟
    *
  */
void LCD_SoftWareWriteByte(uint8_t data)
{
        uint8_t i;
        LCD_DC(1);
        for(i=0;i<8;i++)
        {
            LCD_SCL(0);
            //LCD_delayus(1);
            if(data&0x80)
                LCD_SDA(1);
            else
                LCD_SDA(0);
            LCD_SCL(1);
            //LCD_delayus(1);
            data = (data<<1);
        }
}
/**
    *
    * @brief    向LCD发送一个字
    * @param    data：要发送的字
  * @retval 无
    *   @attention: 软件模拟
    *
  */
void LCD_SoftWareWriteWord(uint16_t data)
{
        uint8_t i;

        LCD_DC(1);

        for(i=0;i<16;i++)
        {
            LCD_SCL(0);
            //LCD_delayus(1);
            if(data&0x8000){ LCD_SDA(1);}
            else{LCD_SDA(0);}
            LCD_SCL(1);
            //LCD_delayus(1);
            data<<=1;
        }
}
/**
    *
    * @brief    向LCD发送命令
    * @param    cmd：要发送的命令
  * @retval 无
    *   @attention: 软件模拟
    *
  */
void LCD_SoftWareWriteCmd(uint8_t cmd)
{
        uint8_t i;

        LCD_DC(0);
        for(i=0;i<8;i++)
        {
            LCD_SCL(0);
            //LCD_delayus(1);
            if(cmd&0x80){LCD_SDA(1);}
            else{LCD_SDA(0);}
            LCD_SCL(1);
            //LCD_delayus(1);
            cmd = (cmd<<1);
        }
}
#else
/**
    *
    * @brief    硬件SPI初始化
    * @param    无
  * @retval 无
    *
  */
void LCD_HardWareSPIInit(uint16_t baudrate)
{
       SPI_Init(ADS1292_SPIx,ADS1292_SPI_CS,MASTER,ADS1292R_SPI_MODE,baudrate);
}

/**
    *
    * @brief    向LCD发送一个字节
    * @param    data：要发送的字节
  * @retval 无
    *   @attention: 硬件控制
    *
  */
void LCD_HardWareWriteByte(uint8_t data)
{
     LCD_DC=1;
     uint8_t ReceiveData = 0;
     SPI_RadeWrite(ADS1292_SPIx, ADS1292_SPI_CS,&data,&ReceiveData,1);
}
/**
    *
    * @brief    向LCD发送一个字
    * @param    data：要发送的字
  * @retval 无
    *   @attention: 硬件控制
    *
  */
void LCD_HardWareWriteWord(uint16_t data)
{
//          LCD_DC=1;
//          uint8_t spitxdata[2];
//          spitxdata[0]=(uint8_t)((data&0xFF00)>>8);
//          spitxdata[1]=(uint8_t)(data&0x00FF);
//          lpspi_transfer_t spi_tranxfer;
//
//          spi_tranxfer.configFlags=kLPSPI_MasterPcs1|kLPSPI_MasterPcsContinuous;     //PCS1
//          spi_tranxfer.txData=&spitxdata;                 //要发送的数据
//          spi_tranxfer.dataSize=2;                        //数据长度
//          LPSPI_MasterTransferBlocking(LPSPI3,&spi_tranxfer);     //SPI阻塞发送
            LCD_HardWareWriteByte((uint8_t)((data&0xFF00)>>8));
            LCD_HardWareWriteByte((uint8_t)(data&0x00FF));
}
/**
    *
    * @brief    向LCD发送命令
    * @param    cmd：要发送的命令
  * @retval 无
    *   @attention: 硬件控制
    *
  */
void LCD_HardWareWriteCmd(uint8_t cmd)
{
     LCD_DC=0;
     uint8_t ReceiveData = 0;
     SPI_RadeWrite(ADS1292_SPIx, ADS1292_SPI_CS,&cmd,&ReceiveData,1);
}
#endif
/**
    *
    * @brief    LCD地址复位
    * @param    无
  * @retval 无
    *
  */
void LCD_AddrReset(void)
{
        LCD_WriteCmd(0x2a);     //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
        LCD_WriteByte(0x00);       //横坐标起始地址0x0000
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);       //横坐标结束地址0x007f(127)
        LCD_WriteByte(0xa8);       //7f
        LCD_WriteCmd(0x2b);     //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
        LCD_WriteByte(0x00);       //纵坐标起始地址0x0000
        LCD_WriteByte(0x00);
        LCD_WriteByte(0x00);       //纵坐标结束地址0x009f(159)
        LCD_WriteByte(0xb3);       //9f
        LCD_WriteCmd(0x2C);     //GRAM接收MCU数据或命令
}
/**
    *
    * @brief    设置LCD画点范围
    * @param    xs：起始x坐标
    * @param    ys：起始y坐标
    * @param    xe：结束x坐标
    * @param    ye：结束y坐标
  * @retval 无
    *
  */
void LCD_SetPos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye)
{
        LCD_WriteCmd(0x2A);         //Colulm addRSTs set
        LCD_WriteByte(0x00);        //行起始坐标高8位，始终为零
        LCD_WriteByte(xs);          //行起始坐标低8位
        LCD_WriteByte(0x00);        //行终止坐标高8位，始终为零
        LCD_WriteByte(xe);          //行终止坐标低8位
        LCD_WriteCmd(0x2B);         //Colulm addRSTs set
        LCD_WriteByte(0x00);        //列起始坐标高8位，始终为零
        LCD_WriteByte(ys);          //列起始坐标低8位
        LCD_WriteByte(0x00);        //列终止坐标高8位，始终为零
        LCD_WriteByte(ye);          //列终止坐标低8位
        LCD_WriteCmd(0x2C);         //GRAM接收MCU数据或命令
}
/**
    *
    * @brief    画点
    * @param    xs：x坐标
    * @param    ys：y坐标
    * @param    color:填充颜色
  * @retval 无
    *
  */
void _LCD_DrawPoint(uint8_t xs,uint8_t ys,uint16_t color)
{
        LCD_WriteCmd(0x2A);         //Colulm addRSTs set
        LCD_WriteByte(0x00);        //行起始坐标高8位，始终为零
        LCD_WriteByte(xs);          //行起始坐标低8位
        LCD_WriteByte(0x00);        //行终止坐标高8位，始终为零
        LCD_WriteByte(xs+1);            //行终止坐标低8位
        LCD_WriteCmd(0x2B);         //Colulm addRSTs set
        LCD_WriteByte(0x00);        //列起始坐标高8位，始终为零
        LCD_WriteByte(ys);          //列起始坐标低8位
        LCD_WriteByte(0x00);        //列终止坐标高8位，始终为零
        LCD_WriteByte(ys);          //列终止坐标低8位
        LCD_WriteCmd(0x2C);         //GRAM接收MCU数据或命令
        LCD_WriteWord(color);
}

void LCD_DrawPoint(uint16_t xs,uint16_t ys,uint16_t color)
{
    _LCD_DrawPoint((uint8_t)xs,(uint8_t)ys,color);
}

/*                  ASCII字符编码                       */
uint8_t Font_code8[][6] = {
  {0x00,0x00,0x00,0x00,0x00,0x00},// (0)
  {0x00,0x00,0x00,0x4F,0x00,0x00},//!(1)
  {0x00,0x00,0x07,0x00,0x07,0x00},//"(2)
  {0x00,0x14,0x7F,0x14,0x7F,0x14},//#(3)
  {0x00,0x24,0x2A,0x7F,0x2A,0x12},//$(4)
  {0x00,0x23,0x13,0x08,0x64,0x62},//%(5)
  {0x00,0x36,0x49,0x55,0x22,0x50},//&(6)
  {0x00,0x00,0x05,0x03,0x00,0x00},//'(7)
  {0x00,0x00,0x1C,0x22,0x41,0x00},//((8)
  {0x00,0x00,0x41,0x22,0x1C,0x00},//)(9)
  {0x00,0x14,0x08,0x3E,0x08,0x14},//*(10)
  {0x00,0x08,0x08,0x3E,0x08,0x08},//+(11)
  {0x00,0x00,0x50,0x30,0x00,0x00},//,(12)
  {0x00,0x08,0x08,0x08,0x08,0x08},//-(13)
  {0x00,0x00,0x60,0x60,0x00,0x00},//.(14)
  {0x00,0x20,0x10,0x08,0x04,0x02},///(15)
  {0x00,0x3E,0x51,0x49,0x45,0x3E},//0(16)
  {0x00,0x00,0x42,0x7F,0x40,0x00},//1(17)
  {0x00,0x42,0x61,0x51,0x49,0x46},//2(18)
  {0x00,0x21,0x41,0x45,0x4B,0x31},//3(19)
  {0x00,0x18,0x14,0x12,0x7F,0x10},//4(20)
  {0x00,0x27,0x45,0x45,0x45,0x39},//5(21)
  {0x00,0x3C,0x4A,0x49,0x49,0x30},//6(22)
  {0x00,0x01,0x71,0x09,0x05,0x03},//7(23)
  {0x00,0x36,0x49,0x49,0x49,0x36},//8(24)
  {0x00,0x06,0x49,0x49,0x29,0x1E},//9(25)
  {0x00,0x00,0x36,0x36,0x00,0x00},//:(26)
  {0x00,0x00,0x56,0x36,0x00,0x00},//;(27)
  {0x00,0x08,0x14,0x22,0x41,0x00},//<(28)
  {0x00,0x14,0x14,0x14,0x14,0x14},//=(29)
  {0x00,0x00,0x41,0x22,0x14,0x08},//>(30)
  {0x00,0x02,0x01,0x51,0x09,0x06},//?(31)
  {0x00,0x32,0x49,0x79,0x41,0x3E},//@(32)
  {0x00,0x7E,0x11,0x11,0x11,0x7E},//A(33)
  {0x00,0x7F,0x49,0x49,0x49,0x3E},//B(34)
  {0x00,0x3E,0x41,0x41,0x41,0x22},//C(35)
  {0x00,0x7F,0x41,0x41,0x22,0x1C},//D(36)
  {0x00,0x7F,0x49,0x49,0x49,0x41},//E(37)
  {0x00,0x7F,0x09,0x09,0x09,0x01},//F(38)
  {0x00,0x3E,0x41,0x49,0x49,0x7A},//G(39)
  {0x00,0x7F,0x08,0x08,0x08,0x7F},//H(40)
  {0x00,0x00,0x41,0x7F,0x41,0x00},//I(41)
  {0x00,0x20,0x40,0x41,0x3F,0x01},//J(42)
  {0x00,0x7F,0x08,0x14,0x22,0x41},//K(43)
  {0x00,0x7F,0x40,0x40,0x40,0x40},//L(44)
  {0x00,0x7F,0x02,0x04,0x02,0x7F},//M(45)
  {0x00,0x7F,0x04,0x08,0x10,0x7F},//N(46)
  {0x00,0x3E,0x41,0x41,0x41,0x3E},//O(47)
  {0x00,0x7F,0x09,0x09,0x09,0x06},//P(48)
  {0x00,0x3E,0x41,0x51,0x21,0x5E},//Q(49)
  {0x00,0x7F,0x09,0x19,0x29,0x46},//R(50)
  {0x00,0x46,0x49,0x49,0x49,0x31},//S(51)
  {0x00,0x01,0x01,0x7F,0x01,0x01},//T(52)
  {0x00,0x3F,0x40,0x40,0x40,0x3F},//U(53)
  {0x00,0x1F,0x20,0x40,0x20,0x1F},//V(54)
  {0x00,0x3F,0x40,0x38,0x40,0x3F},//W(55)
  {0x00,0x63,0x14,0x08,0x14,0x63},//X(56)
  {0x00,0x03,0x04,0x78,0x04,0x03},//Y(57)
  {0x00,0x61,0x51,0x49,0x45,0x43},//Z(58)
  {0x00,0x00,0x7F,0x41,0x41,0x00},//[(59)
  {0x00,0x15,0x16,0x7C,0x16,0x15},//\(60)
  {0x00,0x00,0x41,0x41,0x7F,0x00},//](61)
  {0x00,0x04,0x02,0x01,0x02,0x04},//^(62)
  {0x00,0x40,0x40,0x40,0x40,0x40},//_(63)
  {0x00,0x00,0x01,0x02,0x04,0x00},//`(64)
  {0x00,0x20,0x54,0x54,0x54,0x78},//a(65)
  {0x00,0x7F,0x48,0x44,0x44,0x38},//b(66)
  {0x00,0x38,0x44,0x44,0x44,0x20},//c(67)
  {0x00,0x38,0x44,0x44,0x48,0x7F},//d(68)
  {0x00,0x38,0x54,0x54,0x54,0x18},//e(69)
  {0x00,0x08,0x7E,0x09,0x01,0x02},//f(70)
  {0x00,0x0C,0x52,0x52,0x52,0x3E},//g(71)
  {0x00,0x7F,0x08,0x04,0x04,0x78},//h(72)
  {0x00,0x00,0x44,0x7D,0x40,0x00},//i(73)
  {0x00,0x20,0x40,0x44,0x3D,0x00},//j(74)
  {0x00,0x7F,0x10,0x28,0x44,0x00},//k(75)
  {0x00,0x00,0x41,0x7F,0x40,0x00},//l(76)
  {0x00,0x7E,0x02,0x0C,0x02,0x7C},//m(77)
  {0x00,0x7E,0x04,0x02,0x02,0x7C},//n(78)
  {0x00,0x38,0x44,0x44,0x44,0x38},//o(79)
  {0x00,0x7C,0x14,0x14,0x14,0x08},//p(80)
  {0x00,0x08,0x14,0x14,0x18,0x7C},//q(81)
  {0x00,0x7C,0x08,0x04,0x04,0x08},//r(82)
  {0x00,0x48,0x54,0x54,0x54,0x20},//s(83)
  {0x00,0x04,0x3F,0x44,0x40,0x20},//t(84)
  {0x00,0x3C,0x40,0x40,0x20,0x7C},//u(85)
  {0x00,0x1C,0x20,0x40,0x20,0x1C},//v(86)
  {0x00,0x3C,0x40,0x30,0x40,0x3C},//w(87)
  {0x00,0x44,0x28,0x10,0x28,0x44},//x(88)
  {0x00,0x0C,0x50,0x50,0x50,0x3C},//y(89)
  {0x00,0x44,0x64,0x54,0x4C,0x44},//z(90)
  {0x00,0x00,0x08,0x36,0x41,0x00},//{(91)
  {0x00,0x00,0x00,0x7F,0x00,0x00},//|(92)
  {0x00,0x00,0x41,0x36,0x08,0x00},//}(93)
  {0x00,0x08,0x04,0x08,0x10,0x08},//~(94)
  {0x00,0x08,0x08,0x2A,0x1C,0x08},//→(127)
  {0x00,0x08,0x1C,0x2A,0x08,0x08},//←(128)
  {0x00,0x04,0x02,0x7F,0x02,0x04},//↑(129)
  {0x00,0x10,0x20,0x7F,0x20,0x10},//↓(130)
  {0x00,0x1C,0x2A,0x32,0x2A,0x1C},//笑面(131)
  {0x00,0x1C,0x22,0x44,0x22,0x1C}};//爱心(132)
#endif






