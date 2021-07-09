/*
 * init.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 *  @Brief:
 *          This file is for the initialization function.
 */

#include "init.h"
#include "include.h"

void UIParameterInit(void);

void BeepOffTimerCallBack(void *argc,unsigned short argv)
{
    BEEP.OFF(BEEP.Self);
    Console.WriteLine("BEEP OFF --- Time = %f",os.time.getTime(ms));

}


uint16_t line = 0;

void Core0_HardWareInit()
{
    sint8_t res = -1;
    uint32_t bits = 0;

    /*Init Debug Console.*/

    Console.Init();

    /*Init User Interface.*/

    Screen.Init(Screen.Self);

    for (int i = 0; i < 6; i++)
    {
        KEY[i].Init(KEY[i].Self);
    }

    DIPSwitch.Init(DIPSwitch.Self);

    bits = DIPSwitch.Read(DIPSwitch.Self);

    //bits = 0x00;

    data_pointer = CarMode;

    switch(((bits >> 2) & 0x00000003))
    {
        case 0x00:

            data_pointer = CarMode;

            Screen.WriteXLine(Screen.Self,line,"Mode:CarMode");

            break;
        case 0x01:

            data_pointer = LAutoBoot_Mode;

            Screen.WriteXLine(Screen.Self,line,"Mode:LAutoBoot");

            break;
        case 0x02:

            data_pointer = SAutoBoot_Mode;

            Screen.WriteXLine(Screen.Self,line,"Mode:SAutoBoot");

            break;
        case 0x03:

            data_pointer = AI_Mode;

            Screen.WriteXLine(Screen.Self,line,"Mode:AI");

            break;

        default:
            Screen.WriteXLine(Screen.Self,line,"Mode:ERR");

    }

    line += 2;

    UIParameterInit();

    UI_Init();

    /*Init LED And BEEP.*/

    Screen.WriteXLine(Screen.Self,line,"Init LED...........");

    GLED.Init(GLED.Self);
    BLED.Init(BLED.Self);

    Screen.WriteXLine(Screen.Self,line,"Init LED...........OK");

    Screen.WriteXLine(Screen.Self,line+=2,"Init BEEP..........");
    BEEP.Init(BEEP.Self);
    Screen.WriteXLine(Screen.Self,line,"Init BEEP..........OK");

    /*Init Motor.*/
    Screen.WriteXLine(Screen.Self,line+=2,"Init Motor.........");

    Motor.Init(Motor.Self);
    Motor.Connect(Motor.Self,MotorCtrlStrategy,&Data[data_pointer],sizeof(data_t));

    //Motor.SetSpeedLimit(Motor.Self,10000.0,-10000.0);

    Motor.SetSpeedLimit(Motor.Self,550.0,-550.0);

    //Motor.SetSpeedLimit(Motor.Self,550.0,-550.0);

    Motor.Start(Motor.Self);

    Screen.WriteXLine(Screen.Self,line,"Init Motor.........OK");

    /*Init Servo.*/

    Screen.WriteXLine(Screen.Self,line+=2,"Init Servo.........");

    Servo.Init(Servo.Self);
    Servo.Connect(Servo.Self,ServoCtrlStrategy,&Data[data_pointer],sizeof(data_t));

    //Servo.SetAngleLimit(Servo.Self,1250.0 - 750.0,250.0 - 750.0);

    /*950 690 430*/

    Servo.SetAngleLimit(Servo.Self,210.0,-210.0);

    Servo.SetPwmCentValue(Servo.Self,720);

    Servo.Start(Servo.Self);
    Servo.SetAngle(Servo.Self,0);
    //Servo.SetAngle(Servo.Self,Servo.MinAngle);

    Servo.Update(Servo.Self);

    Screen.WriteXLine(Screen.Self,line,"Init Servo.........OK");

    /*Init Sensor.*/

    Screen.WriteXLine(Screen.Self,line+=2,"Init ESensor.......");

    for(int i = 0;i<CData.MaxLADCDeviceNum;i++)
    {
        LESensor[i].Init(LESensor[i].Self);
        LESensor[i].EnableFilter(LESensor[i].Self,true);
//        LESensor[i].SetGain(LESensor[i].Self,1.0);
//        LESensor[i].EnableGain(LESensor[i].Self,false);
    }
    for(int i = 0;i<CData.MaxSADCDeviceNum;i++)
    {
        SESensor[i].Init(SESensor[i].Self);
        SESensor[i].EnableFilter(SESensor[i].Self,true);
//        SESensor[i].SetGain(SESensor[i].Self,1.0);
//        SESensor[i].EnableGain(SESensor[i].Self,false);
    }

    //DSensor.Init(DSensor.Self);

    Screen.WriteXLine(Screen.Self,line,"Init ESensor.......OK");

    /*Init SD Card And File System.*/
    Screen.WriteXLine(Screen.Self,line+=2,"Init File Sys....");

    if(bits & 0x01)
        res = (sint8_t)SD.init();

    if(res)
    {
        Screen.WriteXLine(Screen.Self,line++,"Init File Sys....Fail");
        Screen.WriteXLine(Screen.Self,line,"Error Code : %d",res);
    }
    else
        Screen.WriteXLine(Screen.Self,line,"Init File Sys......OK");

    /*Init Gyro*/

    res = -1;

    Screen.WriteXLine(Screen.Self,line+=2,"Init Gyro..........");

    if(bits & 0x02)
        res = IMU.Init(IMU.Self);

    if(res)
    {
        Screen.WriteXLine(Screen.Self,line++,"Init Gyro........Fail");
        Screen.WriteXLine(Screen.Self,line,"Error Code : %d",res);
    }
    else
        Screen.WriteXLine(Screen.Self,line,"Init Gyro..........OK");
}

void Core0_SoftWareInit()
{
    /*Init PID Controller.*/
    Data[data_pointer].S_PID = PID_Init(PositionalPID);
    Data[data_pointer].M_PID = PID_Init(IncrementalPID);

    /*Init Debug System.*/

    /*Init NerualNetWork.*/
    NeuralNetworkInit(&Data[data_pointer]);

    /*Init Parameter.*/
    ParameterInit(&Data[data_pointer]);

    Screen.WriteXLine(Screen.Self,line+=2,"System Init Finished.");

    os.time.delay(2.0,s);

    Screen.Clear(Screen.Self,WHITE);

    Screen.WriteXLine(Screen.Self,Screen.Hight/(Screen.Font.Hight*2),"  ---System Start---  ");

    os.time.delay(2.0,s);
    Screen.Clear(Screen.Self,WHITE);

    /* System Init Finished,BEEP ON */
    //Console.WriteLine("BEEP ON --- Time = %f",os.time.getTime(ms));

    //BEEP.ON(BEEP.Self);
    /*Set BEEP OFF 1 sec later*/
    //os.softtimer.start(1,SoftTimer_Mode_OneShot,1000000,0,BeepOffTimerCallBack,NULL,0);
}

void Core1_HardWareInit()
{

}
void Core1_SoftWareInit()
{

}

void Core2_HardWareInit()
{

}
void Core2_SoftWareInit()
{

}

void UIParameterInit(void)
{
#if EnableUiDataLink

    Data[data_pointer].SPwmValue = 750;

    UIData.Actual_Speed = &Data[data_pointer].Actual_Speed;
    UIData.Angle =        &Data[data_pointer].Angle;
    UIData.Bias =         &Data[data_pointer].Bias;
    UIData.MPID_Kd =      &Data[data_pointer].M_PID.Kd;
    UIData.MPID_Ki =      &Data[data_pointer].M_PID.Ki;
    UIData.MPID_Kp =      &Data[data_pointer].M_PID.Kp;
    UIData.MPID_Result =  &Data[data_pointer].M_PID.Result;
    UIData.M_PwmDuty =    (uint16_t *)&Motor.PwmValue;
    UIData.SPID_Kd =      &Data[data_pointer].S_PID.Kd;
    UIData.SPID_Ki =      &Data[data_pointer].S_PID.Ki;
    UIData.SPID_Kp =      &Data[data_pointer].S_PID.Kp;
    UIData.SPID_Result =  &Data[data_pointer].S_PID.Result;


    UIData.S_PwmDuty =    &Data[data_pointer].SPwmValue;

    //UIData.S_PwmDuty =    &Servo.PwmValue;


    UIData.DynamicKp = &Data[data_pointer].DynamicKp;


    UIData.Speed =        &Data[data_pointer].Speed;

    UIData.SADC =          Data[data_pointer].SESensor_SampleValue;
    UIData.LADC =          Data[data_pointer].LESensor_SampleValue;
    UIData.NLADC  =        Data[data_pointer].LESensor_NormalizedValue;
    UIData.NSADC =         Data[data_pointer].SESensor_NormalizedValue;
    UIData.Ke    =        Data[data_pointer].Ke;

    UIData.CarState =    &Data[data_pointer].CarState;
    UIData.CarMode =     (uint *)&Data[data_pointer].CarMode;
    UIData.AI_State =    (uint *)&Data[data_pointer].AI_State;
    UIData.ElementType = &Data[data_pointer].Element.Type;

//    UIData.MotorSysState[0] = (uint *)&MSU.State;
//    UIData.MotorSysState[1] = (uint *)&MDU.State;
//    UIData.MotorSysState[2] = (uint *)&MEU.State;
//    UIData.ServoSysState[0] = (uint *)&SSU.State;
//    UIData.ServoSysState[1] = (uint *)&SDU.State;
//    UIData.ServoSysState[2] = (uint *)&SEU.State;

    UIData.o_difference= &Data[data_pointer].o_difference;
    UIData.h_difference = &Data[data_pointer].h_difference;
    UIData.v_difference = &Data[data_pointer].v_difference;

    UIData.o_sum = &Data[data_pointer].o_sum;
    UIData.h_sum = &Data[data_pointer].h_sum;
    UIData.v_sum = &Data[data_pointer].v_sum;

    UIData.o_bias = &Data[data_pointer].o_bias;
    UIData.h_bias = &Data[data_pointer].h_bias;
    UIData.v_bias = &Data[data_pointer].v_bias;

    UIData.x = &Data[data_pointer].x;

    //UIData.ElementType = &Data[data_pointer].Element.Type;




    //UIData.NActual_Speed = &Data[data_pointer].Actual_Speed;
    //UIData.NAngle = NULL;
    //UIData.NSpeed = &Data[data_pointer].Speed;

#else

    Data[data_pointer].SPwmValue = 750;

    UIData.Actual_Speed = (float *)ReservedData.D;
    UIData.Angle =        (float *)ReservedData.D;
    UIData.Bias =         (float *)ReservedData.D;
    UIData.MPID_Kd =      (float *)ReservedData.D;
    UIData.MPID_Ki =      (float *)ReservedData.D;
    UIData.MPID_Kp =      (float *)ReservedData.D;
    UIData.MPID_Result =  (float *)ReservedData.D;
    UIData.SPID_Kd =      (float *)ReservedData.D;
    UIData.SPID_Ki =      (float *)ReservedData.D;
    UIData.SPID_Kp =      (float *)ReservedData.D;
    UIData.SPID_Result =  (float *)ReservedData.D;
    UIData.Speed =        (float *)ReservedData.D;
    UIData.NLADC  =       (float *)ReservedData.D;
    UIData.NSADC =        (float *)ReservedData.D;
    UIData.NActual_Speed =(float *)ReservedData.D;
    UIData.NAngle =       (float *)ReservedData.D;
    UIData.NSpeed =       (float *)ReservedData.D;

    UIData.S_PwmDuty =    (uint16_t *)ReservedData.D;
    UIData.M_PwmDuty =    (uint16_t *)ReservedData.D;
    UIData.SADC =         (uint16_t *)ReservedData.D;
    UIData.LADC =         (uint16_t *)ReservedData.D;

    UIData.CarState =    (uint *)ReservedData.D;
    UIData.CarMode =     (uint *)ReservedData.D;
    UIData.AI_State =    (uint *)ReservedData.D;
    UIData.ElementType = (uint *)ReservedData.D;

    UIData.MotorSysState[0] = (uint *)ReservedData.D;
    UIData.MotorSysState[1] = (uint *)ReservedData.D;
    UIData.MotorSysState[2] = (uint *)ReservedData.D;
    UIData.ServoSysState[0] = (uint *)ReservedData.D;
    UIData.ServoSysState[1] = (uint *)ReservedData.D;
    UIData.ServoSysState[2] = (uint *)ReservedData.D;

#endif
}

