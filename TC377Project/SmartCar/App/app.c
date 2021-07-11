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

inline bool AICor(data_t *data)
{
    return fabs(data->NNOutput - data->CorAngle) >= 10.0;
}

void ErrorMsg(void *argv,uint error)
{
    data_t *data = (data_t *)argv;

    switch(error)
    {
        case FileSysInitError:

            data->UIEnable = false;

            os.time.delay(0.5,s);

            Screen.SetFontColor(Screen.Self,WHITE);
            Screen.Font.Backcolor = BLUE;

            Screen.Clear(Screen.Self,BLUE);

            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) - 2," :) System Error     ");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) - 1,"    Reason:          ");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) + 0,"    File Sys Not Init.");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) + 1,"    Please Restart.  ");

            __Reset__();

            break;

        case BusError:

            data->Error = BusError;

            data->UIEnable = false;

            Screen.SetFontColor(Screen.Self,WHITE);
            Screen.Font.Backcolor = BLUE;

            Screen.Clear(Screen.Self,BLUE);
            Screen.Clear(Screen.Self,BLUE);
            Screen.Clear(Screen.Self,BLUE);

            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) - 2," :) System Error     ");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) - 1,"    Reason:          ");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) + 0,"    Bus Error.       ");
            Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2) + 1,"    Please Restart.  ");
            break;

        default:

            break;
    }
}
void SmartCarSysDataSave(data_t *data)
{
    extern bool RecordFlags;

    static bool Is_Init = true;

    if(!Is_Init)
    {

    }
    else
    {
        if(RecordFlags && data->CarState)
        {
            //if(!data->IsAddNoise)
            {
                if(SD.isInit)
                {
                    if(data->CarMode == LAutoBoot_Mode)
                    {
                        if(data->Element.Type == Cycle)
                        {
                            data->Element.Point = Cycle;

                            if(data->CycleState == CC_WaitIn || data->CycleState == CC_In || data->CycleState == CC_Confirm)
                            {
                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleIn.txt");
                            }
                            else
                            {
                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/Cycle.txt");
                            }

                        }
                        else if(data->Element.Type == RightAngle)
                        {
                            data->Element.Point = RightAngle;
                            SaveSensorDataAndAngle(data,"LAutoBoot/RightAngle/RightAngle.txt");
                        }
                        else if(data->Element.Type == Cross)
                        {
                            data->Element.Point = Cross;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Cross/Cross.txt");
                        }
                        else if(Is_LoseLine(data))
                        {
                            data->Element.Point = Lost;
                            SaveSensorDataAndAngle(data,"LAutoBoot/LoseLine/LoseLine.txt");
                        }
                        else if(Is_Straight(data))
                        {
                            data->Element.Point = Straight;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Straight/Straight.txt");
                        }
                        else if(Is_Corner(data))
                        {
                            data->Element.Point = Corner;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Corner/Corner.txt");
                        }
                        else
                        {
                            data->Element.Point = Other;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Other/Other.txt");
                        }
                    }
                    else if(data->CarMode == AI_Mode && (AICor(data)))
                    {
                        if(data->Element.Type == Cycle)
                        {
                            data->Element.Point = Cycle;
                            SaveSensorDataAndAngleAI(data,"AI/Cycle/CycleAI.txt");
                        }
                        else if(data->Element.Type == RightAngle)
                        {
                            data->Element.Point = RightAngle;
                            SaveSensorDataAndAngleAI(data,"AI/RightAngle/RightAngleAI.txt");
                        }
                        else if(data->Element.Type == Cross)
                        {
                            data->Element.Point = Cross;
                            SaveSensorDataAndAngleAI(data,"AI/Cross/CrossAI.txt");
                        }
                        else if(Is_LoseLine(data))
                        {
                            data->Element.Point = Lost;
                            SaveSensorDataAndAngleAI(data,"AI/LoseLine/LoseLineAI.txt");
                        }
                        else if(Is_Straight(data))
                        {
                            data->Element.Point = Straight;
                            SaveSensorDataAndAngleAI(data,"AI/Straight/StraightAI.txt");
                        }
                        else if(Is_Corner(data))
                        {
                            data->Element.Point = Corner;
                            SaveSensorDataAndAngleAI(data,"AI/Corner/CornerAI.txt");
                        }
                        else
                        {
                            data->Element.Point = Other;
                            SaveSensorDataAndAngle(data,"AI/Other/OtherAI.txt");
                        }
                    }
                }
                else
                {
                    Is_Init = false;
                    data->Error = FileSysInitError;
                }

            }

            RecordFlags = false;
        }
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

    //Servo.SetAngle(Servo.MaxAngle);

    //while(1);

    TIMx.Init(&TIM_Resources[2].TIMN);
    TIMx.Init(&TIM_Resources[3].TIMN);

    data_t *data = &Data[data_pointer];

    //Motor.SetPwmValue(Motor.Self,2000);

    //MotorSystemIdentification();

    //NNCU_Test();

    while(1)
    {
        SmartCarSysDataSave(data);

        ErrorMsg(data,data->Error);

//        data->UIEnable = false;
//
//        float dt = os.time.getTime(ms);
//
//        data->AIAngle = NeuralNetworkReasoning(data);
//
//        data->AIAngle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);
//
//        dt = os.time.getTime(ms) - dt;
//
//        Screen.Clear(Screen.Self,WHITE);
//        Screen.WriteXLine(Screen.Self,5,"Angle = %.3f",data->AIAngle);
//        Screen.WriteXLine(Screen.Self,6,"Time = %.3f",dt);
//
//        os.time.delay(0.5,s);


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

    data_t *data = &Data[data_pointer];

    while(1)
    {
        if(data->UIEnable)
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
    data_t *data = &Data[data_pointer];

    if(IMU.Is_Init)
        TIMx.Init(&TIM_Resources[5].TIMN);

    while(1)
    {
        if(IMU.Is_Init)
        {
            if(IMU.GetUpdateFlags(IMU.Self))
            {
                IMU.AttitudeUpdate(IMU.Self);
                IMU.SetUpdateFlags(IMU.Self,false);
            }
        }

        if(os.time.getnmsFlag(20))
        {
            if(data->UIEnable)
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

    data_t *data = &Data[data_pointer];

    if(data->UIEnable)
    {
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
}

void Core1_CheckStatus()
{
    static uint times = 0;

    data_t *data = &Data[data_pointer];

    if(data->UIEnable)
    {
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
}

void Core2_CheckStatus()
{
    static uint times = 0;

    data_t *data = &Data[data_pointer];

    if(data->UIEnable)
    {
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
}
