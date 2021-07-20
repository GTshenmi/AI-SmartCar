/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include <LQ_OLED096.h>
#include <LQ_EEPROM.h>


/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////




/*************************************************************************
*  函数名称：void Test_EEPROM(void)
*  功能说明：测试演示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void Test_EEPROM(void)
{
	int i;

	OLED_Init();
	OLED_P6x8Str(0,3,"eeprom");         //字符串
	unsigned long u32wBuff[24];
	unsigned long u32rBuff[24];

	float f32wBuff[24];
	float f32rBuff[24];

	/* 赋值 */
	for(i = 0; i < 24; i++)
	{
		u32wBuff[i] = i * 100;
		f32wBuff[i] = i * 2.5f;
	}

	EEPROM_EraseSector(0);
	EEPROM_EraseSector(1);

	OLED_P6x8Str(0,1,"eeporm u write");

	/* 写入 */
	EEPROM_Write(0, 0, u32wBuff, 24);

	OLED_P6x8Str(0,2,"eeporm f write");

	EEPROM_Write(1, 0, (unsigned long*)f32wBuff, 24);

	OLED_P6x8Str(0,3,"eeporm u read");

	/* 读出 */
	EEPROM_Read(0, 0, u32rBuff, 24);

	OLED_P6x8Str(0,4,"eeporm f read");

	EEPROM_Read(1, 0, (unsigned long*)f32rBuff, 24);

	/* 比较 */
	for(i = 0; i < 24; i++)
	{
		if(u32wBuff[i] != u32rBuff[i])
		{
			OLED_P6x8Str(0,5,"u32error");
		}
		if(f32wBuff[i] != f32rBuff[i])
		{
			OLED_P6x8Str(0,6,"ferror");
		}
	}
	OLED_P6x8Str(0,7,"eeprom is ok");

#pragma warning 557         // 屏蔽警告
	while (1);
#pragma warning default     // 打开警告
}




