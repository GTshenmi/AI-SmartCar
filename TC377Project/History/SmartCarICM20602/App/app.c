/*
 * app.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"
#include "attitude.h"

void SmartCarSysStateUpdate(void *data);
void Core0_CheckStatus(void);
void Core1_CheckStatus(void);
void Core2_CheckStatus(void);

void KeyPressedCallBack(struct key *self,void *argv,uint16_t argc);

void SmartCarSysDataReport(void *data)
{
    data_t *pdata = (data_t *)data;

    //Console.WriteLine("MPID:%f,%f,%f",pdata->Speed,pdata->Actual_Speed,pdata->MPwmValue);
}


/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    TIMx.Init(&TIM_Resources[1].TIMN);
    TIMx.Init(&TIM_Resources[2].TIMN);
    TIMx.Init(&TIM_Resources[3].TIMN);

    //NNCU_Test();

    //NN_Test();

    //data_t *data = &Data[data_pointer];

    //sint16_t angle = (sint16_t)data->Angle;

    ICM20602_Init();

    while(1)
    {
        axis_t acc,gyro;
        attitude_t attitude;
        GLED.Toggle(GLED.Self);

//        SaveParameterSD(data->LADC_Value,data->SADC_Value,&angle);



        ICM20602_ReadAcc(&acc.x,&acc.y,&acc.z);
        ICM20602_ReadGyro(&gyro.x,&gyro.y,&gyro.z);
        printf("a:%7.3f,%7.3f,%7.3f g:%7.3f,%7.3f,%7.3f ",acc.x,acc.y,acc.z,gyro.x,gyro.y,gyro.z);
        AttitudeUpdate(&gyro,&acc,NULL,&attitude);
        printf("attitude x:%8.3f,y:%8.3f,z:%8.3f\n",attitude.roll,attitude.yaw,attitude.pitch);

        Core0_CheckStatus();

        os.time.delay(10,ms);
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
    BEEP.ON(BEEP.Self);

    BEEP.OFF(BEEP.Self);

    uint32_t times = 0;

    data_t *pdata = &Data[data_pointer];

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

        SmartCarSysDataReport(pdata);
        SmartCarSysStateUpdate(pdata);

        os.time.delay(0.02,s);
    }
}



void SmartCarSysStateUpdate(void *data)
{
    data_t *pdata = (data_t *)data;

    if(pdata->CarState == true)
    {
        Motor.Start(Motor.Self);
        Servo.Start(Servo.Self);
    }
    else
    {
        Motor.Stop(Motor.Self);
        Servo.Stop(Servo.Self);
    }

    if(pdata->ReportMotorData)
    {

    }

    if(pdata->ReportSensorData)
    {

    }

    if(pdata->ReportServoData)
    {

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

#if defined(Debug)
    Console.WriteLine("Core2 Running...");
#endif
}