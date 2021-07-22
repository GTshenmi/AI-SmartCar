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
void SmartCarSysDataReport(void *data);
void SmartCarSysDataSave(data_t *data);
void ErrorMsg(void *argv,uint error);
void MotorSystemIdentification(void);
void Core0_CheckStatus(void);
void Core1_CheckStatus(void);
void Core2_CheckStatus(void);

void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc);

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Servo Control and Dadta Process.
 * */
void Core0_Main()
{
    TIMx.Init(&TIM_Resources[2].TIMN);
    TIMx.Init(&TIM_Resources[3].TIMN);

    axis_t acc,gyro,mag;

    data_t *data = &Data[data_pointer];

    while(1)
    {
        SmartCarSysDataSave(data);

        ErrorMsg(data,data->Error);

        if(os.time.getnmsFlag(500))
        {
            IMU.Read(IMU.Self,&acc,&gyro,&mag);

            Console.WriteLine("acc:%f,%f,%f",acc.x,acc.y,acc.z);
            Console.WriteLine("gyro:%f,%f,%f",gyro.x,gyro.y,gyro.z);
            Console.WriteLine("mag:%f,%f,%f",mag.x,mag.y,mag.z);
        }

        /*Attitude Update*/


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

        ErrorMsg(data,data->Error);

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

//    if(IMU.Is_Init)
//        TIMx.Init(&TIM_Resources[5].TIMN);

    while(1)
    {
//        if(IMU.Is_Init)
//        {
//            if(IMU.GetUpdateFlags(IMU.Self))
//            {
//                IMU.AttitudeUpdate(IMU.Self);
//                IMU.SetUpdateFlags(IMU.Self,false);
//            }
//        }

        ErrorMsg(data,data->Error);

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
    }
}

void SmartCarSysDataReport(void *data)
{

}
inline bool AICor(data_t *data)
{
    return fabs(data->NNOutput - data->CorAngle) >= 10.0;
}

void ErrorMsg(void *argv,uint error)
{
    data_t *data = (data_t *)argv;

    static bool is_handled = false;

    if(!is_handled)
    {
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

                is_handled = true;

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

                is_handled = true;

                break;

            default:

                break;
        }
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

        //data->Element.Type = Cycle;
        //data->Element.Type = RightAngle;

        data->Element.Type = Straight;



        //data->Is_AdjustAngle = true;

        if(RecordFlags && data->CarState)
        {
            //if(!data->IsAddNoise)
            {
                if(SD.isInit)
                {
                    if(data->CarMode == LAutoBoot_Mode)
                    {
                        //SaveSensorDataAndAngle(data,"LAutoBoot/RightAngle/RightAngleM.txt");
                        //if(data->CycleState != CC_Wait)
//                        {
//                            data->Element.Point = Cycle;
//                            //SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/Cycle!.txt");
//
//                            if(data->CycleState == CC_WaitIn || data->CycleState == CC_Confirm)
//                            {
//                                //SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleWaitIn.txt");
//                            }
//                            else if(data->CycleState == CC_In)
//                            {
//                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleIn.txt");
//                            }
////                            else if(data->CycleState == CC_Tracking)
////                            {
////                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleTracking.txt");
////                            }
//                            else
//                            {
//                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/Cycle.txt");
//                            }

//                        }
//                        else if(data->Element.Type == RightAngle)
//                        {
//                            data->Element.Point = RightAngle;
//                            SaveSensorDataAndAngle(data,"LAutoBoot/RightAngle/RightAngle.txt");
//                        }
//                        else if(data->Element.Type == Cross)
//                        {
//                            data->Element.Point = Cross;
//                            SaveSensorDataAndAngle(data,"LAutoBoot/Cross/Cross.txt");
//                        }
////                        else if(Is_LoseLine(data))
////                        {
////                            data->Element.Point = Lost;
////                            SaveSensorDataAndAngle(data,"LAutoBoot/LoseLine/LoseLine.txt");
////                        }

                        if(Is_Straight(data) || data->Element.Type == Straight)
                        {
                            data->Element.Point = Straight;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Straight/Straight.txt");
                        }

//                        else if(Is_Corner(data) || data->Element.Type == Corner)
//                        {
//                            data->Element.Point = Corner;
//                            SaveSensorDataAndAngle(data,"LAutoBoot/Corner/Corner.txt");
//                        }
//                        else
//                        {
//                            data->Element.Point = Other;
//                            SaveSensorDataAndAngle(data,"LAutoBoot/Other/Other.txt");
//                        }
                    }
                    else //if(data->CarMode == AI_Mode)
                    {
                        SaveSensorDataAndAngleAI(data,"AI/Data.txt");
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

bool StartRecord = false;
bool FinRecord = false;

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
