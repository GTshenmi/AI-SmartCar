/*
 * app.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */

#include <app.h>
#include "include.h"
#include "fuzzycontrol.h"

void SmartCarSysStateUpdate(void *data);
void Core0_CheckStatus(void);
void Core1_CheckStatus(void);
void Core2_CheckStatus(void);

void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc);

void SmartCarSysDataReport(void *data)
{
    //data_t *pdata = (data_t *)data;

    //Console.WriteLine("MPID:%f,%f,%f",pdata->Speed,pdata->Actual_Speed,pdata->MPwmValue);
}

bool StartRecord = false;
bool FinRecord = false;

void SmartCarSysDataSave(data_t *data)
{
    extern bool RecordFlags;

    if(RecordFlags && data->CarState)
    {
        if(!data->IsAddNoise)
        {
            if(data->Element.Type == Cycle)
                SaveSensorDataAndAngle(data,"Cycle/Cycle.txt");
            else if(data->Element.Type == RightAngle)
                SaveSensorDataAndAngle(data,"RightAngle/RightAngle.txt");
            else if(data->Element.Type == Cross)
                SaveSensorDataAndAngle(data,"Cross/Cross.txt");
            else if(Is_LoseLine(data))
                SaveSensorDataAndAngle(data,"LoseLine/LoseLine.txt");
            else if(Is_Straight(data))
                SaveSensorDataAndAngle(data,"Straight/Straight.txt");
            else if(Is_Corner(data))
                SaveSensorDataAndAngle(data,"Corner/Corner.txt");
            else
                SaveSensorDataAndAngle(data,"Other/Other.txt");

        }

        RecordFlags = false;
    }
}

float InputPwm[SystemIdeLen];
float OutputSpeed[SystemIdeLen];

void MotorSystemIdentification()
{
    //data_t *data = &Data[data_pointer];

    //float dt = 0.002;

    /*Input : Step Singal*/
    for(int i = 0 ; i < SystemIdeLen/3 ; i++)
    {
        InputPwm[i] = 0;
    }
    for(int i = 0 ; i < SystemIdeLen/3 ; i++)
    {
        InputPwm[i + SystemIdeLen/3] = 10000;
    }
    for(int i = 0 ; i < SystemIdeLen/3 ; i++)
    {
        InputPwm[i + (SystemIdeLen * 2)/3] = 0;
    }

    StartRecord = true;

    /*Record Response*/
//    for(int i = 0 ; i < SystemIdeLen ; i ++)
//    {
//        Motor.SetPwmValue(Motor.Self,(sint16_t)InputPwm[i]);
//        //os.time.delayms(2);
//        OutputSpeed[i] = data->Actual_Speed;
//    }

    while(!FinRecord);

    GLED.ON(GLED.Self);

    SaveMotorSystemInfo(InputPwm,OutputSpeed,SystemIdeLen);

    BLED.ON(BLED.Self);

    Screen.SetFontColor(Screen.Self,RED);

    Screen.WriteXLine(Screen.Self,5,"Finished.");

    while(1);
}
/*
 * @Brief:CPU0 Main Func
 *  This Core is for Servo Control and Dadta Process.
 * */
void Core0_Main()
{
    //extern bool RecordFlags;

    TIMx.Init(&TIM_Resources[2].TIMN);
    TIMx.Init(&TIM_Resources[3].TIMN);

    data_t *data = &Data[data_pointer];

    //Motor.SetPwmValue(Motor.Self,2000);

    //MotorSystemIdentification();

    //NNCU_Test();

    while(1)
    {
        SmartCarSysDataSave(data);

        //Screen.WriteXLine(Screen.Self,7,"Motor Pwm Value = %d",Motor.GetPwmValue(Motor.Self));

//        if(data->CarState)
//        {
//            startTime = os.time.getTime(s);
//        }
//        else
//        {
//            endTime = os.time.getTime(s);
//        }
//
//        dt = startTime - endTime;
//
//        speed = Length1/dt;

        if(os.time.getnmsFlag(2))
        {
            //Console.WriteLine("Speed:%f",data->Actual_Speed);
            //Console.WriteLine("Ke:%.3f,%.3f,%.3f,%.3F",data->Ke[0],data->Ke[6],data->V_ESensorValue[0],data->V_ESensorValue[1]);
        }


        //os.time.delay(0.002,s);

        if(os.time.getnmsFlag(1000))
        {
            Core0_CheckStatus();
        }
    }
}

/*
 * @Brief:CPU1 Main Func
 *  This Core is for Motor Control.
 * */
void Core1_Main()
{
    TIMx.Init(&TIM_Resources[4].TIMN);

    while(1)
    {
        os.task.UiUpdate(&UIData,sizeof(UIData));
        Core1_CheckStatus();
    }
}

/*
 * @Brief:CPU2 Main Func
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    //data_t *pdata = &Data[data_pointer];

    while(1)
    {
        if(os.time.getnmsFlag(20))
        {
            os.task.KeyScan(NULL,0);
            os.task.SoftTimerUpdate(NULL,0);
            os.task.DebugConsole(NULL,0);
        }

        if(os.time.getnmsFlag(1000))
        {
            Core2_CheckStatus();
        }

        //os.time.delay(0.001,s);
    }
}



void SmartCarSysStateUpdate(void *data)
{
    //data_t *pdata = (data_t *)data;

    uint32_t bits = DIPSwitch.Read(DIPSwitch.Self);

    // if(pdata->CarState == true)
    // {
    //     Motor.Start(Motor.Self);
    //     Servo.Start(Servo.Self);
    // }
    // else
    // {
    //     Motor.Stop(Motor.Self);
    //     Servo.Stop(Servo.Self);
    // }

    if(bits & 0x04)
        Screen.SetEnable(Screen.Self,true);
    else
        Screen.SetEnable(Screen.Self,false);
}

void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0 ; i < 6 ; i++)
    {
        if(self == KEY[i].Self)
            Screen.WriteXLine(Screen.Self,0,"KEY[%d] Pressed.",i);
    }
}

void Core0_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,16,WHITE);
        Screen.WriteXLine(Screen.Self,16,"Core0 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,16,WHITE);
        Screen.WriteXLine(Screen.Self,16,"Core0 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,16,WHITE);
        Screen.WriteXLine(Screen.Self,16,"Core0 Running...");
    }
}

void Core1_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core1 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core1 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,17,WHITE);
        Screen.WriteXLine(Screen.Self,17,"Core1 Running...");
    }
}

void Core2_CheckStatus()
{
    static uint times = 0;

    times++;
    if(times % 3 == 0)
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core2 Running.");
    }
    else if(times % 3 == 1)
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core2 Running..");
    }
    else
    {
        Screen.ClearLine(Screen.Self,18,WHITE);
        Screen.WriteXLine(Screen.Self,18,"Core2 Running...");
    }
}
