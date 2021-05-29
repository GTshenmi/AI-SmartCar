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

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Servo Control and Data Process.
 * */
void Core0_Main()
{
    TIMx.Init(&TIM_Resources[2].TIMN);
    TIMx.Init(&TIM_Resources[3].TIMN);

    //data_t *data = &Data[data_pointer];

    while(1)
    {
        if(os.time.getnmsFlag(1000))
        {
            //GLED.Toggle(GLED.Self);

            Core0_CheckStatus();

//            GetESensorData(&Data[data_pointer]);
//
//            ESensorDataProcess(&Data[data_pointer]);
//
//            Queue.Puts(&data->EQueue,data->LESensor_NormalizedValue,0,7);
//            Queue.Puts(&data->BQueue,&data->Bias,0,1);
//
//            data->eSensorData = Queue.Gets(&data->EQueue,0,NULL,0,7);
//
//            float *bias = Queue.Gets(&data->BQueue,0,NULL,0,1);
//
//            Console.WriteLine("Raw Data:");
//            Console.WriteArray("float",data->LESensor_NormalizedValue,7);
//
//            Console.WriteLine("Queue Data:");
//            Console.WriteArray("float",data->eSensorData,7);
//
//            Console.WriteLine("Raw Bias:%f",data->Bias);
//            Console.WriteLine("Queue Bias:%f",*bias);


            //Console.WriteLine("h_bias = %f,v_bias = %f,bias = %f,weight = %f",data->h_bias,data->v_bias,data->Bias,fabs(data->h_bias)/fabs(data->v_bias));
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

    ADCx.Init(&ADC_Resources[15].ADCN);

    while(1)
    {

        //os.task.UiUpdate(&UIData,sizeof(UIData));
        Screen.Clear(Screen.Self,WHITE);
        uint16_t value = ADCx.Read(&ADC_Resources[15].ADCN);

        Screen.WriteXLine(Screen.Self,1,"%u",value);

//        for(int i = 0 ; i < 8 ;i++)
//        {
//            Screen.WriteXLine(Screen.Self,i,"AD[%d] = %.3f",i,Data[data_pointer].SESensor_NormalizedValue[i]);
//        }

        Core1_CheckStatus();

    }
}

/*
 * @Brief:CPU2 Main Func
 *  This Core is for Debug.
 * */
void Core2_Main()
{
    data_t *pdata = &Data[data_pointer];

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
            //BLED.Toggle(BLED.Self);
            Core2_CheckStatus();
        }

        SmartCarSysDataReport(pdata);
        SmartCarSysStateUpdate(pdata);

        os.time.delay(0.001,s);
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
