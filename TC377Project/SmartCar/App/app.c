/*
 *
 * app.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 *  @TODO:
 *      1.8月15日之前提交参赛报名表.                DJ Submit(Maybe?).
 *      2.8月15日之前提交防疫与诚信承诺书.          Fin.UnSubmit.
 *      3.8月15日之前提交赛点申请表格.              未完成.
 *      4.8月15日之前提交车模技术检查表与技术文件.  Fin.UnSubmit.
 *      5.8月15日之前提交赛道审核文档.              未完成.
 *      6.8月25日之前提交技术报告.                  未完成.
 *
 *  @Problem:
 *      1.颠簸路段怎么算一个?
 *
 *
 *      7.比赛前两天提交文档(比赛流程/场地展示/比赛直播网站/高校组合信息).
 *      8.比赛当天提交车模检查表/合影/成绩表/录像等.
 *
 *
 */

#include <app.h>
#include "include.h"
#include "fuzzycontrol.h"

void SmartCarSysStateUpdate(void *argv);
void SmartCarSysDataReport(void *data);
void SmartCarSysDataSave(data_t *data);
void ErrorMsg(void *argv,uint error);
void MotorSystemIdentification(void);
//void MotorPIDRecording(data_t *data);
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
    data_t *data = &Data[data_pointer];

    TIMx.Init(&TIM_Resources[2].TIMN);

//    if(data->CarMode == AI_Mode)
//    {
//        TIM_Resources[3].TIMN.interrupt_interval = 5000;
//    }

    TIMx.Init(&TIM_Resources[3].TIMN);

    while(1)
    {
        SmartCarSysDataSave(data);

        ErrorMsg(data,data->Error);

        if(os.time.getnmsFlag(1000))
            Core0_CheckStatus();

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

        SmartCarSysStateUpdate(data);

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


        if(RecordFlags && data->CarState)
        {
            if(!data->IsAddNoise)
            {
                if(SD.isInit)
                {
                    //SaveSensorDataAI2();

                    if(data->Element.Type == Cycle || (data->CycleState != CC_Wait))
                    {
                        data->Element.Point = Cycle;
                    }
                    else if(data->Element.Type == RightAngle)
                    {
                        data->Element.Point = RightAngle;
                    }
                    else if(data->Element.Type == Cross)
                    {
                        data->Element.Point = Cross;
                    }
                    else if(Is_LoseLine(data))
                    {
                        data->Element.Point = Lost;
                    }

                    else if(Is_Straight(data) || data->Element.Type == Straight)
                    {
                        data->Element.Point = Straight;
                    }

                    else if(Is_Corner(data) || data->Element.Type == Corner)
                    {
                        data->Element.Point = Corner;
                    }
                    else
                    {
                        data->Element.Point = Other;
                    }

                    if((data->Element.Type == Cycle) || (data->CycleState != CC_Wait) || (data->Element.Point == Cycle) )
                        SaveSensorDataAndAngle(data,"LAutoBoot/AllCycle.txt");
                    else
                        SaveSensorDataAndAngle(data,"LAutoBoot/AllNoCycle.txt");



#if 0

                    if(data->CarMode == LAutoBoot_Mode)
                    {
                        if(data->CycleState != CC_Wait)
                        {
                            data->Element.Point = Cycle;
                            //SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/Cycle!.txt");

                            if(data->CycleState == CC_WaitIn || data->CycleState == CC_Confirm)
                            {
                                //SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleWaitIn.txt");
                            }
                            else if(data->CycleState == CC_In)
                            {
                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleIn.txt");
                            }
//                            else if(data->CycleState == CC_Tracking)
//                            {
//                                SaveSensorDataAndAngle(data,"LAutoBoot/Cycle/CycleTracking.txt");
//                            }
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
//                        else if(Is_LoseLine(data))
//                        {
//                            data->Element.Point = Lost;
//                            SaveSensorDataAndAngle(data,"LAutoBoot/LoseLine/LoseLine.txt");
//                        }

                        if(Is_Straight(data) || data->Element.Type == Straight)
                        {
                            data->Element.Point = Straight;
                            SaveSensorDataAndAngle(data,"LAutoBoot/Straight/Straight.txt");
                        }

                        else if(Is_Corner(data) || data->Element.Type == Corner)
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
                    else
                    {
                        SaveSensorDataAndAngleAI(data,"AI/Data.txt");
                    }
#endif
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

char *CycleStateDict[] =
{
        "Undef",
        "Exc",

        "Wait",
        "Config",
        "Confirm",
        "WaitIn",

        "In",
        "Tracking",
        "Out",
};

void SmartCarSysStateUpdate(void *argv)
{
    data_t *data = (data_t *)argv;

    uint32_t bits = DIPSwitch.Read(DIPSwitch.Self);


    if(bits & 0x80)
    {
        data->UIEnable = false;
        Screen.Clear(Screen.Self,WHITE);
        Screen.WriteXLine(Screen.Self,5,"CC:%s",CycleStateDict[data->CycleState]);
        os.time.delay(0.5,s);
    }
    else
    {
        //Screen.Clear(Screen.Self,WHITE);
        data->UIEnable = true;
    }
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
    data_t *data = &Data[data_pointer];

    //float dt = 0.002;

    data->UIEnable = false;

    Screen.SetFontColor(Screen.Self,BLUE);

    Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight * 2),"Input : Step Signal.");

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

    os.time.delay(1.0,s);

    StartRecord = true;

    Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight * 2),"Recording Response.");

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

    Screen.Clear(Screen.Self,WHITE);

    Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight * 2),"Recording Finished.");

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
