/*
 * element.c
 *
 *  Created on: 2020骞�12鏈�6鏃�
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

    if(loseLineCnt >= 1500 && data->CarMode != DebugMode)
    {
        Motor.Break(Motor.Self);
        Servo.Break(Servo.Self);
    }

    return data->Element.Type * 1.0;

}

/*
 * @Brief:鐗规畩鍏冪礌澶勭悊鎺ュ彛鍑芥暟
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;

    RightAngle_Handler(data);

    Cross_Handler(data);

    Cycle_Handler(data);

    LoseLine_Handler(data);

    Ramp_Handler(data);
}

void StopSituationDetect(void) {
    //GPIO Resources[27]
    static _Bool firstExecute = 1;
    if (firstExecute) {
        GPIOx.Init(&GPIO_Resources[27].GPION);
        firstExecute = 0;
    }

    if (!GPIOx.Read(&GPIO_Resources[27].GPION) && os.time.getTimes() >= 10) {
        Motor.Stop(Motor.Self);
    }
}

/*鍧￠亾澶勭悊*/
void Ramp_Handler(data_t *data)
{

}

void RustAngle_Handler(data_t *data)
{
    data->Bias = Servo.GetMaxAngle(Servo.Self);
}

void Cycle_Handler(data_t *data)
{
    static cycle_state_t cycleState = CC_Wait;
    static cycle_dir_t   cycleDir = CC_DirUndefined;

    static float bias = 0.0;

    static sint32_t cycleWaitCnt = 0;

    static sint32_t cycleWaitInCnt = 0;

    static sint32_t cycleInCnt = 0;

    static sint32_t cycleOutCnt;

    static sint32_t cycleFlagCnt = 0;

    static sint32_t exceptionHandlerCnt = 0;

    static bool isMidESensorMaxValue = false;
    static bool isLeftOSensorFall = false;
    static bool isRightOSensorFall = false;
    static bool isMidHSensorFall = false;

    static bool isLeftHSensorFall = false;
    static bool isRightHSensorFall = false;

    static float bias_ra = 0.0;

    static float bias_slant = 0.0;

    static float isCyclePos = 0.0;

    static float pos = 0.0;

    static float inPos = 0.0;

    static bool Is_SlantIn = false;

    if((FindMinIndex(data->H_ESensorValue,3) == 1) && (data->H_ESensorValue[1] <= 90.0) && data->Element.Type == Cycle)
    {
        //data->Element.Exception.CC == CC_SlantIn;

        bias_slant = fsign(data->v_difference) * 100.0;

        Is_SlantIn = true;

    }

//    if(data->Element.Exception.EXT != NoException && data->Element.Type == Cycle)
//    {
//        data->Element.Exception.EXT = NoException;
//
//        data->Element.Exception.CC = CC_SlantIn;
//
//        cycleState = CC_Undefined;
//    }

    switch(cycleState)
    {
        case CC_Undefined:

            cycleState = CC_Exception_Handler;

            if((data->Element.Exception.CC == CC_SlantIn))
            {
                if(data->Element.Exception.EXT == RA_To_CC)
                {
                    bias_ra = data->Element.Exception.Info[RightAngle][Cycle];
                }

                else
                {
                    bias = bias_slant;

                    cycleState = CC_In;

                    cycleInCnt = 360;
                }
                exceptionHandlerCnt = 50;
            }

            break;

        case CC_Exception_Handler:

            exceptionHandlerCnt --;

            data->Bias = bias_ra;

            isCyclePos = 0.0;

            pos = 0.0;

            inPos = 0.0;

            if((data->Element.Exception.CC == CC_SlantIn))
            {
                if(Is_CCNormal(data,exceptionHandlerCnt))
                {
                    cycleState = CC_In;

                    cycleInCnt = 360;

                    //bias = data->v_bias * 1.2;

                    bias = -bias_ra;

                    //bias = 100.0;

                    data->Element.Type =Cycle;

                    //BEEP.OFF(BEEP.Self);


                }
            }

            if(data->Element.Exception.CC == CC_MisJudge)
            {
                //Console.WriteLine("[Warning]:CC_MisJudge(%f)",data->Element.Exception.Info[Cycle][Cycle]);
                cycleState = CC_Wait;
            }

            if(data->Element.Exception.CC == CC_Err)
            {
                //Console.WriteLine("[Error]:CC_Err(%f)",data->Element.Exception.Info[Cycle][Cycle]);
                cycleState = CC_Wait;
            }

            break;

        case CC_Wait:

            cycleWaitCnt--;

            if(cycleWaitCnt <= 0)
                cycleWaitCnt = 0;

//            if(FindMinIndex(data->H_ESensorValue,3) == 1)
//            {
//                cycleState = CC_Undefined;
//
//                data->Element.Exception.CC = CC_SlantIn;
//            }

            if(data->Element.Type == Cycle && cycleWaitCnt <= 0)
            {
                cycleState = CC_Confirm;

                isCyclePos = data->x;
            }

            break;

        case CC_Confirm:

            if(1)//Is_Cycle(data))
            {
                cycleState = CC_WaitIn;

                float sum_l,sum_r;

                if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
                {
                    cycleInCnt = 100;
                    cycleOutCnt = 0;

                    sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0];// + data->O_ESensorValue[0];

                    sum_r = data->H_ESensorValue[2] + data->V_ESensorValue[1];// + data->O_ESensorValue[1];
                }
                else
                {
                    cycleInCnt = 100;

                    cycleOutCnt = 0; /*have problem. is zero?*/

                    sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0];

                    sum_r = data->H_ESensorValue[3] + data->V_ESensorValue[1];
                }

                BLED.ON(BLED.Self);

                if(sum_l > sum_r)
                {
                    cycleDir = CC_DirLeft;
                }
                else
                {
                    cycleDir = CC_DirRight;
                }

                if(cycleDir == CC_DirLeft)
                {
                    bias = 100.0;
                    cycleWaitInCnt = 220;
                }
                else
                {
                    bias = -100.0;
                    cycleWaitInCnt = 220;
                }
            }
            else
            {
//                if(FindMinIndex(data->H_ESensorValue,3) == 1)
//                {
//                    cycleState = CC_Undefined;
//
//                    data->Element.Exception.CC = CC_SlantIn;
//                }
//                else
                {
                    cycleState = CC_Undefined;
                    data->Element.Exception.CC = CC_MisJudge;
                    data->Element.Exception.Info[Cycle][Cycle] = CC_Confirm;
                }
            }

            break;

        case CC_WaitIn:

            cycleWaitInCnt--;

            pos = data->x;

            if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
            {
                if(data->Ke[2] >= 5.0 || data->Ke[4] >= 5.0)
                {
                    isMidESensorMaxValue = true;
                }

               if(isMidESensorMaxValue)
               {
                   if(data->Ke[2] < 0.0)
                   {
                       isLeftOSensorFall = true;

                       cycleFlagCnt++;
                   }

                   if(data->Ke[4] < 0.0)
                   {
                       isRightOSensorFall = true;
                       cycleFlagCnt++;
                   }

                   if(data->Ke[3] <= 0.0)
                   {
                       isMidHSensorFall = true;

                       cycleFlagCnt++;
                   }

                   if(data->Ke[1] <= 0.0)
                   {
                       isLeftHSensorFall = true;

                       cycleFlagCnt++;

                   }

                   if(data->Ke[5] <= 0.0)
                   {
                       isRightHSensorFall = true;

                       cycleFlagCnt++;
                   }

                   if((cycleFlagCnt>=2) && ((pos - isCyclePos) >= 90.0))
                   {
                           cycleState = CC_In;
                           inPos = data->x;
                   }

              }
            }
            else
            {

               if(data->Ke[2] >= 5.0 || data->Ke[5] >= 5.0)
               {
                     isMidESensorMaxValue = true;
               }

               if(isMidESensorMaxValue)
               {
                   if(data->Ke[2] < 0.0)
                   {
                       isLeftOSensorFall = true;

                       cycleFlagCnt++;
                   }

                   if(data->Ke[5] < 0.0)
                   {
                       isRightOSensorFall = true;
                       cycleFlagCnt++;
                   }

                   if(data->Ke[4] <= 0.0)
                   {
                       isMidHSensorFall = true;

                       cycleFlagCnt++;
                   }

                   if(data->Ke[3] <= 0.0)
                   {
                       isLeftHSensorFall = true;

                       cycleFlagCnt++;

                   }

                   if((cycleFlagCnt>=2) && ((pos - isCyclePos) >= 50.0))
                   {
                           cycleState = CC_In;
                           inPos = data->x;
                   }
               }

               if(cycleWaitInCnt <= -2000)
               {
                   cycleState = CC_Undefined;
                   data->Element.Exception.CC = CC_MisJudge;
                   data->Element.Exception.Info[Cycle][Cycle] = CC_WaitIn;
               }
            }

            break;


        case CC_In:

            data->Bias = bias;

            cycleInCnt--;

            pos = data->x;

            if(data->CarMode == AI_Mode || data->CarMode == LAutoBoot_Mode)
            {
                if(data->h_bias <= 20.0 && cycleInCnt <= 0 && ((pos - inPos) >= 25.0))
                {
                    isLeftOSensorFall = false;
                    isRightOSensorFall = false;
                    isMidHSensorFall = false;
                    isRightHSensorFall = false;
                    isLeftHSensorFall = false;
                    isMidESensorMaxValue = false;

                    //pos = 0.0;

                    inPos = 0.0;

                    isCyclePos = 0.0;

                    cycleWaitInCnt = 0;

                    cycleInCnt = 0;

                    cycleFlagCnt = 0;

                    cycleState = CC_Tracking;
                }
            }
            else
            {
                if(data->h_bias <= 35.0 && cycleInCnt <= 0 && ((pos - inPos) >= 20.0))
                {
                    isLeftOSensorFall = false;
                    isRightOSensorFall = false;
                    isMidHSensorFall = false;
                    isRightHSensorFall = false;
                    isLeftHSensorFall = false;
                    isMidESensorMaxValue = false;

                    //pos = 0.0;

                    inPos = 0.0;

                    isCyclePos = 0.0;

                    cycleWaitInCnt = 0;

                    cycleInCnt = 0;

                    cycleFlagCnt = 0;

                    cycleState = CC_Tracking;
                }
            }

            if(cycleInCnt <= -2000)
            {
                cycleState = CC_Undefined;
                data->Element.Exception.CC = CC_Err;
                data->Element.Exception.Info[Cycle][Cycle] = CC_In;
            }

            break;

        case CC_Tracking:

            cycleOutCnt++;

            data->Bias = ((data->h_difference + data->v_difference) / data->h_sum) * 100.0;

            //data->Bias = data->Bias * 1.05 * 0.95 + fsign(bias) * 100.0 * 0.05;

            data->Bias = data->Bias * 0.95 + fsign(bias) * 100.0 * 0.05;

            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);

            if(Is_CycleOut(data,cycleOutCnt) || cycleOutCnt >= 6000 || (data->x - pos) >= 2000.0)
            {
                cycleState = CC_Out;
            }

            break;

        case CC_Out:

            if(Is_CycleBackToStraight(data) || cycleOutCnt >= 6000 || (data->x - pos) >= 2000.0)
            {
                cycleState = CC_Wait;
                data->Element.Type = None;

                cycleWaitCnt = 500;

                BLED.OFF(BLED.Self);
            }

            if(cycleOutCnt >= 6000)
            {
                cycleState = CC_Undefined;
                data->Element.Exception.CC = CC_Err;
                data->Element.Exception.Info[Cycle][Cycle] = CC_Out;
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

    static sint32_t rightAngleCount = 0;

    static float lastBias = 0.0;

    static float bias = 0.0;

    switch(rightAngleState)
    {
        case RA_Wait:

            if(data->Element.Type == RightAngle)
            {
                rightAngleState = RA_Confirm;

                goto RA_CONFIRM;
            }

            break;

        case RA_Confirm:

            RA_CONFIRM:

            if(1)
            {
                rightAngleCount = 100;

                GLED.ON(GLED.Self);

                bias = fsign(data->v_difference) * 100.0;

                rightAngleState = RA_Tracking;

                /*Changed.*/
                if(data->CarMode != AI_Mode && data->CarMode != SAutoBoot_Mode)
                {
                    data->Is_AdjustAngle = true;

                    data->Angle = - fsign(bias) * Servo.MaxAngle;

                    Servo.SetAngle(Servo.Self,data->Angle);
                    Servo.Update(Servo.Self);
                }
                else
                {
                    data->Bias = bias;
                }

                goto RA_TRACKING;
            }

            break;

        case RA_Tracking:

            RA_TRACKING:

            if((data->v_difference >= 15.0) && (data->v_sum >= 15.0))
                bias = fsign(data->v_difference) * 100.0;

            data->Bias = bias;

            /*Changed.*/
            if(data->CarMode != AI_Mode && data->CarMode != SAutoBoot_Mode)
            {
                data->Angle = - fsign(bias) * Servo.MaxAngle;

                Servo.SetAngle(Servo.Self,data->Angle);
                Servo.Update(Servo.Self);
            }

            rightAngleCount--;

            if(Is_RightAngleOut(data,rightAngleCount) || rightAngleCount <= -1000)
            {
                data->Is_AdjustAngle = false;

                data->Is_AdjustSpeed = false;

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

            if(Is_RAToCCException(data))
            {
                data->Element.Exception.EXT = RA_To_CC;

                data->Element.Exception.Info[RightAngle][Cycle] = bias;

                //BEEP.ON(BEEP.Self);
            }
            else
            {
                data->Element.Exception.EXT = NoException;

                data->Element.Exception.Info[RightAngle][Cycle] = bias;
            }

            break;

        case RA_Undefined:

            break;

        default:
            break;
    }

    lastBias = data->Bias;
}

void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    static float bias = 0.0;

    if(data->TrackingState == Normal_Tracking)
    {
        loseLineState = LL_Wait;
    }

    switch(loseLineState)
    {
        case LL_Wait:

            if(data->TrackingState == LoseLine)
            {
                loseLineState = LL_Lose;
            }

            break;

        case LL_Lose:

//            for(int i = 0 ; i <= 5 ;i++)
//            {
//                HESensorData = Queue.Gets(&data->HESensorQueue,-i,NULL,0,4);
//
//                if(HESensorData[0] >= 5.0 || HESensorData[3] >= 5.0)
//                {
//                    bias = *Queue.Gets(&data->HBiasQueue,-i,NULL,0,1);
//
//                    loseLineState = LL_SearchLine;
//
//                    break;
//                }
//            }

            bias = *Queue.Gets(&data->RawBiasQueue,-1,NULL,0,1);

            loseLineState = LL_SearchLine;

//            if(loseLineState == LL_Lose)
//            {
//                loseLineState = LL_BackSearchLine;
//            }



            break;

        case LL_BackSearchLine:

            //DebugBeepOff;
            break;


        case LL_SearchLine:

            data->Bias = bias;

            if(Is_SearchedLine(data))
            {
                loseLineState = LL_Searched;
                //data->Is_AdjustSpeed = false;
            }

            break;

        case LL_Searched:

            loseLineState = LL_Wait;
            data->TrackingState = Normal_Tracking;

            //DebugBeepOff;

            break;

        case LL_Undefined:default:

            break;
    }
}

void Cross_Handler(data_t *data)
{
    static cross_state_t crossState = CS_Wait;

    static uint32_t crossCnt = 0;

    uint32_t interval = 125;

    switch(crossState)
    {
        case CS_Wait:

            if(data->Element.Type == Cross)
            {
                crossCnt = interval;

                BEEP.ON(BEEP.Self);

                crossState = CS_Confirm;
            }

            break;

        case CS_Confirm:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_In;

                BEEP.Toggle(BEEP.Self);
            }


            break;

        case CS_In:
            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_Tracking;

                BEEP.Toggle(BEEP.Self);
            }

            break;

        case CS_Tracking:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_Out;

                BEEP.Toggle(BEEP.Self);
            }

            break;
        case CS_Out:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

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



///*
// * element.c
// *
// *  Created on: 2020年12月6日
// *      Author: 936305695
// *  @Brief:
// *          This file is for the special element.
// */
//#include <element.h>
//#include "include.h"
//
//void LoseLine_Handler(data_t *data);
//void Cycle_Handler(data_t *data);
//void Cross_Handler(data_t *data);
//void RightAngle_Handler(data_t *data);
//void Ramp_Handler(data_t *data);
//void Parking_Handler(data_t *data);
//
//float ElementDetermine(void *argv)
//{
//    data_t *data = (data_t *)argv;
//
//    static uint32_t loseLineCnt = 0;
//
//    if(Is_Ramp(data))
//    {
//        data->Element.Type = Ramp;
//    }
//
//    if(Is_RightAngle(data))
//    {
//        data->Element.Type = RightAngle;
//    }
//
//    if(Is_Cycle(data))
//    {
//        data->Element.Type = Cycle;
//    }
//
//    if(Is_Cross(data))
//    {
//        data->Element.Type = Cross;
//    }
//
//    if(Is_LoseLine(data) && data->Element.Type != RightAngle && data->Element.Type != Cycle)
//    {
//        data->TrackingState = LoseLine;
//        loseLineCnt++;
//
//        //DebugBeepOn;
//    }
//    else
//    {
//        data->TrackingState = Normal_Tracking;
//        loseLineCnt = 0;
//
//        //DebugBeepOff;
//    }
//
////    if(loseLineCnt >= 1500)
////    {
////        Motor.Break(Motor.Self);
////        Servo.Break(Servo.Self);
////    }
////    else
////    {
////
////    }
//
////    if(Element == data->Element.Type)
////    {
////        Element = data->Element.Type;
////    }
////    else
////    {
////        lastElement = Element;
////        Element = data->Element.Type;
////        data->LastElement.Type = data->Element.Type;
////    }
//
//
//
//
//    //data->LastElement = data->Element;
//
////    else
////    {
////        Motor.Start(Motor.Self);
////    }
//
//    return data->Element.Type * 1.0;
//
//}
//
///*
// * @Brief:特殊元素处理接口函数
// * */
//void SpecialElementHandler(void *argv)
//{
//    data_t *data = (data_t *)argv;
//
//    RightAngle_Handler(data);
//
//    Cross_Handler(data);
//
//    Cycle_Handler(data);
//
//    LoseLine_Handler(data);
//
//    Ramp_Handler(data);
//
//    if(Is_Dst(data))
//    {
//        Parking_Handler(data);
//    }
//}
//
//void Parking_Handler(data_t *data)
//{
//
//}
///*坡道处理*/
//void Ramp_Handler(data_t *data)
//{
//
//}
//
//void RustAngle_Handler(data_t *data)
//{
//    data->Bias = Servo.GetMaxAngle(Servo.Self);
//}
//
//void Cycle_Handler(data_t *data)
//{
//    static cycle_state_t cycleState = CC_Wait;
//    static cycle_dir_t   cycleDir = CC_DirUndefined;
//
//    static float bias = 0.0;
//
//    static sint32_t cycleWaitCnt = 0;
//
//    static sint32_t cycleWaitInCnt = 0;
//
//    static sint32_t cycleInCnt = 0;
//
//    static sint32_t cycleTrackingCnt;
//
//    static sint32_t cycleFlagCnt = 0;
//
//    static sint32_t exceptionHandlerCnt = 0;
//
//    static bool isMidESensorMaxValue = false;
//    static bool isLeftOSensorFall = false;
//    static bool isRightOSensorFall = false;
//    static bool isMidHSensorFall = false;
//
//    static bool isLeftHSensorFall = false;
//    static bool isRightHSensorFall = false;
//
//    switch(cycleState)
//    {
//        case CC_Undefined:
//
//            cycleState = CC_Exception_Handler;
//
//            BLED.Toggle(BLED.Self);
//
//            break;
//
//        case CC_Exception_Handler:
//
//            exceptionHandlerCnt --;
//
//            BLED.Toggle(BLED.Self);
//
//            if(data->Element.Exception.CC == CC_MisJudge)
//            {
//                //Console.WriteLine("[Warning]:CC_MisJudge(%f)",data->Element.Exception.Info[Cycle][Cycle]);
//                cycleState = CC_Wait;
//            }
//
//            if(data->Element.Exception.CC == CC_Err)
//            {
//                //Console.WriteLine("[Error]:CC_Err(%f)",data->Element.Exception.Info[Cycle][Cycle]);
//                cycleState = CC_Wait;
//            }
//
//            cycleState = CC_Wait;
//
//            break;
//
//        case CC_Wait:
//
//            cycleWaitCnt--;
//
//            BLED.OFF(BLED.Self);
//
//            if(cycleWaitCnt <= 0)
//                cycleWaitCnt = 0;
//
//            if(data->Element.Type == Cycle && cycleWaitCnt <= 0)
//            {
//                cycleState = CC_Confirm;
//            }
//
//            break;
//
//        case CC_Confirm:
//
//            if(Is_Cycle(data))
//            {
//                cycleWaitInCnt = 220;
//                cycleState = CC_WaitIn;
//
//                float sum_l,sum_r;
//
//                //cycleInCnt = 120;
//
//                //s 90 80  50  35
//                //t 80 100 320 500
//
//                sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0];
//
//                sum_r = data->H_ESensorValue[2] + data->V_ESensorValue[1];
//
//                if(sum_l > sum_r)
//                {
//                    cycleDir = CC_DirLeft;
//                }
//                else
//                {
//                    cycleDir = CC_DirRight;
//                }
//            }
//            else
//            {
//                cycleState = CC_Undefined;
//                data->Element.Exception.CC = CC_MisJudge;
//                data->Element.Exception.Info[Cycle][Cycle] = CC_Confirm;
//            }
//
//            break;
//
//        case CC_WaitIn:
//
//            cycleWaitInCnt--;
//
//            if(data->Ke[2] >= 5.0 || data->Ke[4] >= 5.0)
//            {
//                isMidESensorMaxValue = true;
//            }
//
//            if(isMidESensorMaxValue)
//            {
//
//               if(data->Ke[2] < 0.0)
//               {
//                   isLeftOSensorFall = true;
//
//                   cycleFlagCnt++;
//               }
//
//               if(data->Ke[4] < 0.0)
//               {
//                   isRightOSensorFall = true;
//                   cycleFlagCnt++;
//               }
//
//               if(data->Ke[3] <= 0.0)
//               {
//                   isMidHSensorFall = true;
//
//                   cycleFlagCnt++;
//               }
//
//               if(data->Ke[1] <= 0.0)
//               {
//                   isLeftHSensorFall = true;
//
//                   cycleFlagCnt++;
//
//               }
//
//               if(data->Ke[5] <= 0.0)
//               {
//                   isRightHSensorFall = true;
//
//                   cycleFlagCnt++;
//               }
//
//               if((cycleFlagCnt>=2) && (cycleWaitInCnt <= 0))
//               {
//                   cycleState = CC_WaitIn;
//
////                   float sum_l,sum_r;
////
////                   //cycleInCnt = 120;
////
////                   //s 90 80  50  35
////                   //t 80 100 320 500
////
////                   sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0];
////
////                   sum_r = data->H_ESensorValue[2] + data->V_ESensorValue[1];
////
////                   if(sum_l > sum_r)
////                   {
////                       cycleDir = CC_DirLeft;
////                   }
////                   else
////                   {
////                       cycleDir = CC_DirRight;
////                   }
//
//                   if(cycleDir == CC_DirLeft)
//                   {
//                       bias = 100.0;
//                       cycleInCnt = 100;
//                       //cycleInCnt = (sint32_t)CycleInCntFunc(data->H_ESensorValue[0]) - ConstrainFloat(data->O_ESensorValue[0] * 0.5,0.0,25.0);
//                   }
//                   else
//                   {
//                       bias = -100.0;
//                       cycleInCnt = 100;
//                       //cycleInCnt = (sint32_t)CycleInCntFunc(data->H_ESensorValue[2]) - ConstrainFloat(data->O_ESensorValue[1] * 0.5,0.0,25.0);
//                   }
//
//                   BLED.ON(BLED.Self);
//
//                   cycleState = CC_In;
//               }
//
//           }
//
//           if(cycleWaitInCnt <= -2000)
//           {
//               cycleState = CC_Undefined;
//               data->Element.Exception.CC = CC_MisJudge;
//               data->Element.Exception.Info[Cycle][Cycle] = CC_WaitIn;
//           }
//
//           break;
//
//
//        case CC_In:
//
//            data->Bias = bias;
//
//            cycleInCnt--;
//
//            if(data->h_bias <= 20.0 && cycleInCnt <= 0)
//            {
//                cycleTrackingCnt = 0;
//
//                isLeftOSensorFall = false;
//                isRightOSensorFall = false;
//                isMidHSensorFall = false;
//                isRightHSensorFall = false;
//                isLeftHSensorFall = false;
//                isMidESensorMaxValue = false;
//
//                cycleWaitInCnt = 0;
//
//                cycleInCnt = 0;
//
//                cycleFlagCnt = 0;
//
//                cycleState = CC_Tracking;
//            }
//
//            if(cycleInCnt <= -2000)
//            {
//                cycleState = CC_Undefined;
//                data->Element.Exception.CC = CC_Err;
//                data->Element.Exception.Info[Cycle][Cycle] = CC_In;
//            }
//
//            break;
//
//        case CC_Tracking:
//
//            cycleTrackingCnt++;
//
//            data->Is_AdjustSpeed = true;
//
//            //data->Speed = 2000;
//
//            data->Bias = ((data->h_difference + data->v_difference) / data->h_sum) * 100.0;
//
//            data->Bias = data->Bias * 0.95 + fsign(bias) * 100.0 * 0.05;
//
//            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);
//
//            if(Is_CycleOut(data,cycleTrackingCnt) || cycleTrackingCnt >= 6000)
//            {
//                data->Is_AdjustSpeed = false;
//
//                cycleState = CC_Out;
//            }
//
//            break;
//
//        case CC_Out:
//
//            if(Is_CycleBackToStraight(data) || cycleTrackingCnt >= 6000)
//            {
//                cycleState = CC_Wait;
//                data->Element.Type = None;
//
//                cycleWaitCnt = 500;
//
//                BLED.OFF(BLED.Self);
//            }
//
//            if(cycleTrackingCnt >= 6000)
//            {
//                cycleState = CC_Undefined;
//                data->Element.Exception.CC = CC_Err;
//                data->Element.Exception.Info[Cycle][Cycle] = CC_Out;
//            }
//
//            break;
//
//        default:
//
//            break;
//    }
//}
//
//void RightAngle_Handler(data_t *data)
//{
//    static rightangle_state_t rightAngleState = RA_Wait;
//
//    static sint32_t rightAngleCount = 0;
//
//    static float lastBias = 0.0;
//
//    static float bias = 0.0;
//
//    switch(rightAngleState)
//    {
//        case RA_Wait:
//
//            if(data->Element.Type == RightAngle)
//            {
//                rightAngleState = RA_Confirm;
//
//                goto RA_CONFIRM;
//            }
//
//            break;
//
//        case RA_Confirm:
//
//            RA_CONFIRM:
//
//            if(1)
//            {
//                rightAngleCount = 100;
//
//                GLED.ON(GLED.Self);
//
//                bias = fsign(data->v_difference) * 100.0;
//
//                rightAngleState = RA_Tracking;
//
//                /*Changed.*/
//                if(data->CarMode != AI_Mode)
//                {
//                    data->Is_AdjustAngle = true;
//
//                    data->Angle = - fsign(bias) * Servo.MaxAngle;
//
//                    Servo.SetAngle(Servo.Self,data->Angle);
//                    Servo.Update(Servo.Self);
//                }
//                else
//                {
//                    data->Bias = bias;
//                }
//
//
//                goto RA_TRACKING;
//            }
//
//            break;
//
//        case RA_Tracking:
//
//            RA_TRACKING:
//
//            if((data->v_difference >= 15.0) && (data->v_sum >= 15.0))
//                bias = fsign(data->v_difference) * 100.0;
//
//            data->Bias = bias;
//
//            /*Changed.*/
//            if(data->CarMode != AI_Mode)
//            {
//                data->Angle = - fsign(bias) * Servo.MaxAngle;
//
//                Servo.SetAngle(Servo.Self,data->Angle);
//                Servo.Update(Servo.Self);
//            }
//
//            rightAngleCount--;
//
//            if(Is_RightAngleOut(data,rightAngleCount) || rightAngleCount <= -1000)
//            {
//
//                data->Is_AdjustAngle = false;
//
//                data->Is_AdjustSpeed = false;
//
//                rightAngleState = RA_Out;
//            }
//
//            break;
//
//        case RA_Out:
//
//            if(Is_RightAngleBackToStraight(data))
//            {
//                GLED.OFF(GLED.Self);
//                rightAngleState = RA_Wait;
//                data->Element.Type = None;
//            }
//
//            if(Is_RAToCCException(data))
//            {
//                data->Element.Exception.EXT = RA_To_CC;
//
//                data->Element.Exception.Info[RightAngle][Cycle] = bias;
//
//                //BEEP.ON(BEEP.Self);
//            }
//            else
//            {
//                data->Element.Exception.EXT = NoException;
//
//                data->Element.Exception.Info[RightAngle][Cycle] = bias;
//            }
//
//            break;
//
//        case RA_Undefined:
//
//            break;
//
//        default:
//            break;
//    }
//
//    lastBias = data->Bias;
//}
//
//void LoseLine_Handler(data_t *data)
//{
//    static loseline_state_t loseLineState = LL_Wait;
//
//    static float bias = 0.0;
//
//    if(data->TrackingState == Normal_Tracking)
//    {
//        loseLineState = LL_Wait;
//    }
//
//    switch(loseLineState)
//    {
//        case LL_Wait:
//
//            if(data->TrackingState == LoseLine)
//            {
//                loseLineState = LL_Lose;
//            }
//
//            break;
//
//        case LL_Lose:
//
////            for(int i = 0 ; i <= 5 ;i++)
////            {
////                HESensorData = Queue.Gets(&data->HESensorQueue,-i,NULL,0,4);
////
////                if(HESensorData[0] >= 5.0 || HESensorData[3] >= 5.0)
////                {
////                    bias = *Queue.Gets(&data->HBiasQueue,-i,NULL,0,1);
////
////                    loseLineState = LL_SearchLine;
////
////                    break;
////                }
////            }
//
//            bias = *Queue.Gets(&data->RawBiasQueue,-1,NULL,0,1);
//
//            loseLineState = LL_SearchLine;
//
////            if(loseLineState == LL_Lose)
////            {
////                loseLineState = LL_BackSearchLine;
////            }
//
//            break;
//
//        case LL_BackSearchLine:
//
//            //DebugBeepOff;
//            break;
//
//        case LL_SearchLine:
//
//            data->Bias = bias;
//
//            if(Is_SearchedLine(data))
//            {
//                loseLineState = LL_Searched;
//                //data->Is_AdjustSpeed = false;
//            }
//
//            break;
//
//        case LL_Searched:
//
//            loseLineState = LL_Wait;
//            data->TrackingState = Normal_Tracking;
//
//            //DebugBeepOff;
//
//            break;
//
//        case LL_Undefined:default:
//
//            break;
//    }
//}
//
//void Cross_Handler(data_t *data)
//{
//    static cross_state_t crossState = CS_Wait;
//
//    switch(crossState)
//    {
//        case CS_Wait:
//
//            break;
//
//        case CS_Confirm:
//
//            break;
//
//        case CS_In:
//
//            break;
//
//        case CS_Tracking:
//
//            break;
//        case CS_Out:
//
//            crossState = CS_Wait;
//            data->Element.Type = None;
//
//            break;
//
//        default:
//
//            break;
//    }
//}
