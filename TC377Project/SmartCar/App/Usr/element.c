/*
 * element.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *          This file is for the special element.
 */
#include <element.h>
#include "include.h"

void LoseLine_Handler(data_t *data);
void Cycle_Handler(data_t *data);
void Cross_Handler(data_t *data);
void RightAngle_Handler(data_t *data);
void Ramp_Handler(data_t *data);
void RustAngle_Handler(data_t *data);
void StopSituationDetect(data_t *data);


float ElementDetermine(void *argv)
{
    data_t *data = (data_t *)argv;

    static uint32_t loseLineCnt = 0;

    if(Is_Ramp(data))
    {
        data->Element.Type = Ramp;
    }

    if(Is_RightAngle(data))
    {
        data->Element.Type = RightAngle;
    }

    if(Is_Cycle(data))
    {
        data->Element.Type = Cycle;
    }

    if(Is_Cross(data) && data->Element.Type != Cycle && data->Element.Type != RightAngle)
    {
        data->Element.Type = Cross;
    }


    if(Is_LoseLine(data) && data->Element.Type != RightAngle && data->Element.Type != Cycle)
    {
        data->TrackingState = LoseLine;
        loseLineCnt++;

        //DebugBeepOn;
    }
    else
    {
        data->TrackingState = Normal_Tracking;
        loseLineCnt = 0;

        //DebugBeepOff;
    }

//    if(loseLineCnt >= 1500 && data->CarMode != DebugMode)
//    {
//        Motor.Break(Motor.Self);
//        Servo.Break(Servo.Self);
//    }

    return data->Element.Type * 1.0;

}

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;

    RightAngle_Handler(data);

    Cross_Handler(data);

    Cycle_Handler(data);

    LoseLine_Handler(data);

    Ramp_Handler(data);

    RustAngle_Handler(data);

    StopSituationDetect(data);
}

void StopSituationDetect(data_t *data) {
    //GPIO Resources[27]

    static _Bool firstExecute = 1;
    static float startTime = 0.0;
    static float startPos = 0.0;

    static int times = 2;

    switch(data->GameMode)
    {
        case OneLopMode:

            if (firstExecute) {
                GPIOx.Init(&GPIO_Resources[27].GPION);
                firstExecute = 0;
                startTime =os.time.getTime(s);
                startPos = data->x;
            }

            if ((!GPIOx.Read(&GPIO_Resources[27].GPION)) && ((os.time.getTimes() - startTime) >= 10 && ((data->x - startPos) >= 100.0))) {

                startTime =os.time.getTime(s);
                startPos = data->x;

                times--;
            }

            if(times <= 0)
            {
                Motor.Break(Motor.Self);
            }

            break;

        case ScoringMode:

            if((os.time.getTime(s) - data->GameStartTime) >= 60.0 * 3)
            {
                Motor.Break(Motor.Self);
                data->GameOverTime = os.time.getTime(s);
            }

            break;
    }
}

void Ramp_Handler(data_t *data)
{
    //data->Bias = 0.0;
}

void RustAngle_Handler(data_t *data)
{
    //data->Bias = Servo.GetMaxAngle(Servo.Self) * fsign(data->HESensor[0].Value - data->HESensor[1].Value);
}

void Cycle_Handler(data_t *data)
{
    static cycle_state_t cycleState = CC_Wait;
    static cycle_dir_t   cycleDir = CC_DirUndefined;

    static cycle_cnt cycleCnt = {0,0,0,0,0,0,0};

    static cycle_flag_t cycleFlag = {false,false,false,false,false,false};

    static cycle_config  cycleConfig = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    switch(cycleState)
    {
        case CC_Undefined:

            cycleState = CC_Exception_Handler;

            break;

        case CC_Exception_Handler:

            CycleClearConfig(&cycleConfig);

            CycleClearCnt(&cycleCnt);

            CycleClearFlag(&cycleFlag);

            BLED.OFF(BLED.Self);

            data->Element.Type = None;

            cycleCnt.Wait = 2000;

            cycleState = CC_Wait;

            break;

        case CC_Wait:

            cycleCnt.Wait--;

            if(cycleCnt.Wait <= 0)
                cycleCnt.Wait = 0;

            if(data->Element.Type == Cycle && cycleCnt.Wait <= 0)
            {
                cycleState = CC_Config;
            }

            break;

        case CC_Config:

            CycleClearConfig(&cycleConfig);
            CycleClearCnt(&cycleCnt);
            CycleClearFlag(&cycleFlag);

            cycleConfig.inCyclePointYaw = 0.0;

            cycleConfig.dYaw = 0.0;

            cycleConfig.waitInDistance = data->CycleWaitInDistance;
            cycleConfig.inDistance = data->CycleInDistance;

            cycleConfig.sum_l = data->HESensor[0].Value + data->VESensor[0].Value;

            cycleConfig.sum_r = data->HESensor[3].Value + data->VESensor[1].Value;

            if(cycleConfig.sum_l > cycleConfig.sum_r)//Judge the direction of cycle.
            {
                cycleDir = CC_DirLeft;
            }
            else
            {
                cycleDir = CC_DirRight;
            }

            if(cycleDir == CC_DirLeft) 
            {
                cycleConfig.bias = 100.0;
            }
            else
            {
                cycleConfig.bias = -100.0;
            }

            cycleConfig.isCyclePos = data->x;

            BLED.ON(BLED.Self);

            cycleState = CC_Confirm;

            break;

        case CC_Confirm:

            cycleCnt.Confirm--;

            if(Is_CycleConfirmed(data,&cycleCnt,&cycleFlag,cycleDir))//Confirm.
            {
                cycleConfig.inCyclePointYaw = data->attitude.yaw;

                //cycleConfig.isCyclePos = data->x;

                cycleState = CC_WaitIn;
            }

            //if(cycleCnt.Confirm <= -2000.0)//Misjudge.
            //    cycleState = CC_Undefined;

            if((data->x - cycleConfig.isCyclePos) >= 3 * cycleConfig.waitInDistance)//Misjudge.
            {
                cycleState = CC_Undefined;
            }

            break;

        case CC_WaitIn:

            cycleCnt.WaitIn--;

            if(Is_ArriveCycleInPoint(data,&cycleConfig))//Wait for in cycle point.
            {
                cycleState = CC_In;
            }

            if((data->x - cycleConfig.isCyclePos) >= 5 * cycleConfig.waitInDistance)//Misjudge.
            {
                cycleState = CC_Undefined;
            }

//            if(cycleCnt.WaitIn <= -2000.0)//Misjudge.
//            {
//                cycleState = CC_Undefined;
//            }

            break;


        case CC_In:

            cycleCnt.In--;

            cycleConfig.dYaw = data->attitude.yaw - cycleConfig.inCyclePointYaw;

            data->Bias = cycleConfig.bias;   //Give the servo a fixed angle within a certain distance,to enter cycle.

            if(Is_CycleIn(data,&cycleConfig))
            {
                cycleState = CC_Tracking;//in cycle success.
            }

            if((data->x - cycleConfig.isCyclePos) >= 5 * (cycleConfig.waitInDistance + cycleConfig.inDistance))//Misjudge.
            {
                cycleState = CC_Undefined;
            }

//            if(cycleCnt.In <= -2000)
//            {
//                cycleState = CC_Undefined;//in cycle fail.
//            }

            break;

        case CC_Tracking:

            cycleCnt.Tracking++;

            cycleConfig.dYaw = data->attitude.yaw - cycleConfig.inCyclePointYaw;

            data->Bias = ((data->h_difference + data->v_difference) / data->h_sum) * 100.0;

            data->Bias = data->Bias * 0.95 + fsign(cycleConfig.bias) * 100.0 * 0.05;

            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);

            if(Is_CycleOut(data,&cycleCnt,&cycleConfig)/*Normal*/ || cycleCnt.Tracking >= 6000 || (data->x - cycleConfig.isCyclePos) >= 2000.0/*Abnormal,force out*/)
            {
                cycleState = CC_Out;//out cycle.
            }

            break;

        case CC_Out:

            cycleCnt.Out++;

            cycleCnt.Wait = 2000;

            if(Is_CycleBackToStraight(data))/*Normal Out.*/
            {
                //back to straight.
                cycleState = CC_Wait;    
                data->Element.Type = None;

                BLED.OFF(BLED.Self);
            }

            if(cycleCnt.Out >= 500 || cycleCnt.Tracking >= 6000 || (data->x - cycleConfig.isCyclePos) >= 2000.0)//Force out of the cycle.
            {
                cycleState = CC_Undefined;
            }

            break;

        default:

            break;
    }

    data->CycleState = cycleState;
}

void RightAngle_Handler(data_t *data)
{
    static rightangle_state_t rightAngleState = RA_Wait;

    static rightangle_cnt rightAngleCnt = {0};

    static rightangle_config rightAngleConfig = {0};

    switch(rightAngleState)
    {
        case RA_Undefined:

            rightAngleState = RA_ExceptionHandler;

            break;

        case RA_ExceptionHandler:

            RightAngleClearCnt(&rightAngleCnt);
            RightAngleClearConfig(&rightAngleConfig);
            GLED.OFF(GLED.Self);
            rightAngleState = RA_Wait;

            break;

        case RA_Wait:

            if(data->Element.Type == RightAngle)
            {
                rightAngleState = RA_Config;

                goto RA_CONFIG;
            }

            break;

        case RA_Config:

            RA_CONFIG:

            RightAngleClearCnt(&rightAngleCnt);
            RightAngleClearConfig(&rightAngleConfig);

            rightAngleCnt.Tracking = 100;

            rightAngleConfig.bias = fsign(data->v_difference) * 100.0;//Determine the direction of the right angle.

            rightAngleState = RA_Confirm;

            goto RA_CONFIRM;

            break;

        case RA_Confirm:    //confirm and config some parameter.

            RA_CONFIRM:

            if(1)
            {
                GLED.ON(GLED.Self);

                rightAngleState = RA_Tracking;

                goto RA_TRACKING;
            }

            break;

        case RA_Tracking:

            RA_TRACKING:

            if((data->v_difference >= 15.0) && (data->v_sum >= 15.0))           //for continuous right angle,update bias.
                rightAngleConfig.bias = fsign(data->v_difference) * 100.0;

            data->Bias = rightAngleConfig.bias;//give the servo max angle by force.

            rightAngleCnt.Tracking--;

            if(Is_RightAngleOut(data,&rightAngleCnt,&rightAngleConfig))//if find line,hand over to the control algorithm to patrol the line.
            {
                rightAngleState = RA_Out;
            }

            if(rightAngleCnt.Tracking <= -1000) //force out of rightangle.
            {
                rightAngleState = RA_Out;
            }

            break;

        case RA_Out:

            if(Is_RightAngleBackToStraight(data))
            {
                GLED.OFF(GLED.Self);
                rightAngleState = RA_Wait;
                data->Element.Type = None;
            }

            break;

        default:

            break;
    }
}

void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    static loseline_config  loseLineConfig = {0};

    if(data->TrackingState == Normal_Tracking)
    {
        loseLineState = LL_Wait;
    }

    switch(loseLineState)
    {
        case LL_Undefined:

            loseLineState = LL_ExceptionHandler;

            break;

        case LL_ExceptionHandler:

            LoseLineClearConfig(&loseLineConfig);

            loseLineState = LL_Wait;

            break;

        case LL_Wait:

            if(data->TrackingState == LoseLine)
            {
                loseLineState = LL_Config;
            }

            break;

        case LL_Config:

            LoseLineClearConfig(&loseLineConfig);
            loseLineConfig.bias = data->B[8];
            loseLineState = LL_Confirm;

            break;

        case LL_Confirm:

            if(Is_LoseLine(data))
            {
                loseLineState = LL_SearchLine;
            }
            else
            {
                loseLineState = LL_Undefined;
            }

            break;

        case LL_BackSearchLine:

            break;


        case LL_SearchLine:

            data->Bias = loseLineConfig.bias;

            if(Is_SearchedLine(data))
            {
                loseLineState = LL_Searched;
            }

            break;

        case LL_Searched:

            loseLineState = LL_Wait;
            data->TrackingState = Normal_Tracking;

            break;

        default:

            break;
    }
}

void Cross_Handler(data_t *data)
{
    static cross_state_t crossState = CS_Wait;

    static cross_cnt     crossCnt = {0};

    static cross_config  crossConfig = {0};

    switch(crossState)
    {

        case CS_Undefined:

            crossState = CS_ExceptionHandler;

            break;

        case CS_ExceptionHandler:

            CrossClearCnt(&crossCnt);
            CrossClearConfig(&crossConfig);
            crossState = CS_Wait;

            break;

        case CS_Wait:

            if(data->Element.Type == Cross)
            {
                BEEP.ON(BEEP.Self);

                crossState = CS_Config;
            }

            break;

        case CS_Config:

            CrossClearCnt(&crossCnt);
            CrossClearConfig(&crossConfig);

            crossConfig.interval = 125;
            crossCnt.cnt = crossConfig.interval;

            crossState = CS_Confirm;
            break;

        case CS_Confirm:

            crossCnt.cnt --;

            if(crossCnt.cnt <= 0)
            {
                crossCnt.cnt = crossConfig.interval;

                crossState = CS_In;

                BEEP.Toggle(BEEP.Self);
            }


            break;

        case CS_In:
            crossCnt.cnt --;

            if(crossCnt.cnt <= 0)
            {
                crossCnt.cnt = crossConfig.interval;

                crossState = CS_Tracking;

                BEEP.Toggle(BEEP.Self);
            }

            break;

        case CS_Tracking:

            crossCnt.cnt --;


            if(crossCnt.cnt <= 0)
            {
                crossCnt.cnt = crossConfig.interval;

                crossState = CS_Out;

                BEEP.Toggle(BEEP.Self);
            }

            break;
        case CS_Out:

            crossCnt.cnt --;

            if(crossCnt.cnt <= 0)
            {
                crossCnt.cnt = crossConfig.interval;

                crossState = CS_Tracking;

                BEEP.Toggle(BEEP.Self);

                BEEP.OFF(BEEP.Self);

                crossState = CS_Wait;
                data->Element.Type = None;
            }


            break;

        default:

            break;
    }
}
