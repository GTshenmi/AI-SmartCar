/*
 * app.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"

void Core0_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core0 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core0 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core0 Running...");
    }

#if defined(Debug)
    Console.WriteLine("Core0 Running...");
#endif
}

void Core1_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core1 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core1 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core1 Running...");
    }

#if defined(Debug)
    Console.WriteLine("Core1 Running...");
#endif
}

void Core2_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,19,WHITE);
        Screen.WriteXLine(Screen.Self,19,"Core2 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,19,WHITE);
        Screen.WriteXLine(Screen.Self,19,"Core2 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,19,WHITE);
        Screen.WriteXLine(Screen.Self,19,"Core2 Running...");
    }

#if defined(Debug)
    Console.WriteLine("Core2 Running...");
#endif
}
void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0 ; i < 6 ; i++)
    {
        if(self == KEY[i].Self)
            Screen.WriteXLine(Screen.Self,0,"KEY[%d] Pressed.",i);
    }
}
/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    uint8_t res = SD_Initialize(NULL);

    if(res)
    {
        Screen.WriteXLine(Screen.Self,0,"SD Card Init Failed. ");
        //Screen.WriteXLine(Screen.Self,2,"Error Code: %u",res);
    }
    else
        Screen.WriteXLine(Screen.Self,0,"SD Card Init Success.");

    Screen.WriteXLine(Screen.Self,1,"Read Sector[0] :");
    //uint8_t buf[10];

    uint8_t read_buf[10];

//    for(int i = 0  ; i < 10 ; i++)
//        buf[i] = i;

    //SD_WriteSector(0,0,buf,10);

    SD_ReadSector(0,0,read_buf,10);
//
//    Screen.Clear(Screen.Self,WHITE);

    for(int i = 0  ; i < 10 ; i++)
    {
        Screen.WriteXLine(Screen.Self,i + 2,"Buf[%d] = %u",i,read_buf[i]);
    }




//    SPIx_Test(SD_SPI);
//    GPIOx.Init(SD_SPI_CS_GPIO);
//    SPIx.Init(SD_SPI);
//
//    while(1)
//    {
//        GPIOx.Write(SD_SPI_CS_GPIO,1);
//
//        os.time.delayms(2000);
//
//        GPIOx.Write(SD_SPI_CS_GPIO,0);
//        Screen.WriteXLine(Screen.Self,1,"    0v");
//
//        os.time.delayms(2000);
//    }


//    uint8_t res = SD_disk_initialize(0);
//
//    if(res & STA_NOINIT)
//        Screen.WriteXLine(Screen.Self,0,"SD Card Init Failed.");
//    else
//        Screen.WriteXLine(Screen.Self,0,"SD Card Init Success.");


//   Servo.SetAngle(Servo.Self,90.0);
//   Servo.Update(Servo.Self);
    TIMx.Init(&TIM_Resources[1].TIMN);
    TIMx.Init(&TIM_Resources[2].TIMN);

    while(1)
    {
//        for(int i = 0 ; i < 7 ; i ++)
//        {
//            Screen.ClearLine(Screen.Self,i,WHITE);
//            Screen.WriteXLine(Screen.Self,i,"SADC[%d]: %u",i,Data[data_pointer].SADC_Value[i]);
//        }
        GLED.Toggle(GLED.Self);

        Core0_CheckStatus();

        os.time.delay(1.0,s);
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Nerual Network.
 * */
void Core1_Main()
{

    while(1)
    {
        //os.task.UiUpdate(&UIData,sizeof(UIData));

        Core1_CheckStatus();
    }
}

/*
 * @Brief:CPU2 Main Func 暂时不用 可能会供电不足
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    BEEP.ON(BEEP.Self);

    BEEP.OFF(BEEP.Self);

    uint32_t times = 0;

    while(1)
    {
        times++;

        if(!(times% 50))
        {
            BLED.Toggle(BLED.Self);
            Core2_CheckStatus();
        }

        os.task.KeyScan(NULL,0);
        os.task.SoftTimerUpdate(NULL,0);
        os.task.DebugConsole(NULL,0);

        os.time.delay(0.02,s);
    }
}

