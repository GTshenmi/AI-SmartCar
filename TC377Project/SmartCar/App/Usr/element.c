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


float ElementDetermine(void *argv)
{
    data_t *data = (data_t *)argv;

    //data->Element.Type = None;

    if(Is_RightAngle(data))
    {
        data->Element.Type = RightAngle;

        //data->Is_AdjustSpeed = true;

        //data->Speed = 1500;
    }

    if(Is_Cycle(data))
    {
        data->Element.Type = Cycle;

    }

    if(Is_Cross(data))
    {
        data->Element.Type = Cross;
    }

    if(Is_LoseLine(data) && data->Element.Type != RightAngle && data->Element.Type != Cycle)
    {
        data->TrackingState = LoseLine;

        //DebugBeepOn;
    }
    else
    {
        data->TrackingState = Normal_Tracking;

        DebugBeepOff;
    }

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

    static bool isMidESensorMaxValue = false;
    static bool isLeftOSensorFall = false;
    static bool isRightOSensorFall = false;
    static bool isMidHSensorFall = false;

    switch(cycleState)
    {
        case CC_Wait:

            cycleWaitCnt--;

            if(cycleWaitCnt <= 0)
                cycleWaitCnt = 0;

            if(data->Element.Type == Cycle && cycleWaitCnt <= 0)
            {
                cycleState = CC_Confirm;
            }

            break;

        case CC_Confirm:

            if(Is_Cycle(data))
            {
                float sum_l,sum_r;

                if(data->CarMode == LAutoBoot_Mode)
                {
                    cycleInCnt = 100;
                    cycleOutCnt = 0; /*have problem. is zero?*/

                    sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0] + data->O_ESensorValue[0];

                    sum_r = data->H_ESensorValue[2] + data->V_ESensorValue[1] + data->O_ESensorValue[1];
                }
                else
                {
                    cycleInCnt = 270;

                    cycleOutCnt = 0; /*have problem. is zero?*/

                    sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0] + data->O_ESensorValue[0];

                    sum_r = data->H_ESensorValue[3] + data->V_ESensorValue[1] + data->O_ESensorValue[1];
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

                cycleState = CC_WaitIn;
            }

            break;

        case CC_WaitIn:

            cycleWaitInCnt--;

            if(data->CarMode == LAutoBoot_Mode)
            {
                if(data->Ke[2] >= 10.0 || data->Ke[4] >= 10.0)
                {
                    isMidESensorMaxValue = true;
                }


               if(isMidESensorMaxValue)
               {

                   if(data->Ke[2] < 0.0)
                   {
                       isLeftOSensorFall = true;
                   }

                   if(data->Ke[4] < 0.0)               //右入环提前入弯
                   {
                       isRightOSensorFall = true;
                   }

                   if(data->Ke[3] <= 0)
                   {
                       isMidHSensorFall = true;
                   }

                   if(isLeftOSensorFall && isRightOSensorFall && isMidHSensorFall && (cycleWaitInCnt <= 0))

                   //if((isLeftOSensorFall || isRightOSensorFall || isMidHSensorFall) && (cycleWaitInCnt <= 0))
                   {
                       cycleState = CC_In;
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
                   }

                   if(data->Ke[5] < 0.0)               //右入环提前入弯
                   {
                       isRightOSensorFall = true;
                   }

                   if(data->Ke[4] <= 0)
                   {
                       isMidHSensorFall = true;
                   }

                   if((isLeftOSensorFall || isRightOSensorFall || isMidHSensorFall) && (cycleWaitInCnt <= 0))
                   {
                       cycleState = CC_In;
                   }
               }
            }

            break;


        case CC_In:

            data->Bias = bias;

            cycleInCnt--;

            if(data->h_bias <= 20.0 && cycleInCnt <= 0)
            {
                cycleState = CC_Tracking;
            }

            break;

        case CC_Tracking:
            
            cycleOutCnt++;
            
            //if(data->Element.Type != RightAngle)
            //    data->Bias = data->o_bias;

            data->Bias = data->Bias * 1.2;

            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);

            if(Is_CycleOut(data,cycleOutCnt) || cycleOutCnt >= 6000)
            {
                cycleState = CC_Out;
            }

            break;

        case CC_Out:

            if(Is_CycleBackToStraight(data) || cycleOutCnt >= 6000)
            {
                cycleState = CC_Wait;
                data->Element.Type = None;

                cycleWaitCnt = 500;

                BLED.OFF(BLED.Self);
            }
            
            break;

        default:

            break;
    }
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

            //if(Is_RightAngle(data))
            {
                rightAngleCount = 100;

                GLED.ON(GLED.Self);    

                bias = fsign(data->v_difference) * 100.0;

                rightAngleState = RA_Tracking;

                data->Is_AdjustAngle = true;
                //data->Is_AdjustSpeed = true;

                data->Angle = - fsign(bias) * Servo.MaxAngle;

                Servo.SetAngle(Servo.Self,data->Angle);
                Servo.Update(Servo.Self);

                goto RA_TRACKING;
            }

            break;

        case RA_Tracking:

            RA_TRACKING:

            data->Bias = bias;
            data->Angle = - fsign(bias) * Servo.MaxAngle;

            Servo.SetAngle(Servo.Self,data->Angle);
            Servo.Update(Servo.Self);

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

            break;

        case RA_Undefined:default:
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

                //DebugBeepOn;
            }

            break;

        case LL_Lose:

//            for(int i = 0 ; i <= 10 ;i++)
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

            DebugBeepOff;
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

            DebugBeepOff;

            break;

        case LL_Undefined:default:

            break;
    }
}

//void LoseLine_Handler(data_t *data)
//{
//    static loseline_state_t loseLineState = LL_Wait;
//
//    static float bias = 0.0;
//
//    int index = 0;
//
//    bool isFindBias = false;
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
//            //DebugBeepOn;
//
//            for(index = 1 ; index < 10 ; index++)
//            {
//                float *_eType = Queue.Gets(&data->ElementTypeQueue,-index,NULL,0,1);
//                float *_trackingState = Queue.Gets(&data->TrackingQueue,-index,NULL,0,1);
//                float *_eSensorValue = Queue.Gets(&data->ESensorQueue,-index,NULL,0,7);
//
//                if(FIs_Equal(*_eType,data->Element.Type * 1.0) && (!FIs_Equal(*_trackingState,LoseLine * 1.0)))
//                {
//                    float *_bias = NULL;
//
//                    if(data->Element.Type == None)
//                        _bias = Queue.Gets(&data->RawBiasQueue,-index,NULL,0,1);
//                    else
//                        _bias = Queue.Gets(&data->ElementBiasQueue,-index,NULL,0,1);
//
//                    bias = *_bias;
//
//                    //BLED.ON(BLED.Self);
//
//                    if(Is_LLBiasVaild(_eSensorValue,bias))
//                    {
//                        isFindBias = true;
//                        break;
//                    }
//                }
//                else
//                {
//                    if(!FIs_Equal(*_eType,data->Element.Type * 1.0))
//                    {
//                        break;
//                    }
//                }
//            }
//
//            if(isFindBias)
//            {
//                loseLineState = LL_SearchLine;
//            }
//            else
//            {
//                bias = 0.0;
//                loseLineState = LL_BackSearchLine;
//
//                data->Is_AdjustSpeed = true;
//            }
//
//            break;
//
//        case LL_BackSearchLine:
//
//            data->Bias = bias;
//
//            data->Speed = -3200;
//
//            if(Is_SearchedLine(data))
//            {
//                loseLineState = LL_Lose;
//                data->Is_AdjustSpeed = false;
//            }
//
//            break;
//
//
//        case LL_SearchLine:
//
//            data->Bias = fsign(bias) * 100.0;
//
//            data->Is_AdjustSpeed = false;
//
//            if(Is_SearchedLine(data))
//            {
//                loseLineState = LL_Searched;
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



void Cross_Handler(data_t *data)
{
    static cross_state_t crossState = CS_Wait;

    static incross_attitude_t inCrossAttitude = CS_UndefinedIn;

    static cross_info_t   crossInfo = CS_UndefinedInfo;

    switch(crossState)
    {
        case CS_Wait:

            if(data->Element.Type == Cross)
                crossState = CS_Confirm;
            break;

        case CS_Confirm:

            if(Is_Cross(data))
            {
                if(data->o_difference >= 50.0)
                {
                    crossInfo = CS_Left;
                }
                else if(data->o_difference <= -50.0)
                {
                    crossInfo = CS_Right;
                }
                else
                {
                    crossInfo = CS_UndefinedIn;
                }

                if(data->h_bias >= 30.0)
                {
                    inCrossAttitude = CS_RightIn;
                }
                else if(data->h_bias <= -30.0)
                {
                    inCrossAttitude = CS_LeftIn;
                }
                else
                {
                    inCrossAttitude = CS_MidIn;
                }

                GLED.ON(GLED.Self);

            }

            crossState = CS_In;

            break;

        case CS_In:

            break;

        case CS_Tracking:

            break;
        case CS_Out:

            crossState = CS_Wait;
            data->Element.Type = None;

            break;

        default:

            break;
    }
}
