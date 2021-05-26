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


/*
 *       Condition A  Condition B       Y
 *           0            0             0
 *           0            1             1
 *           1            0             1
 *           1            1             0
 *
 *
 *           Y = A'B + AB' = A ^ B
 * */
//#define RightAngleJudgeCondition ((data->V_ESensorValue[0] >= 30.0) ^ (data->V_ESensorValue[1] >= 30.0)) //Raw

//#define RightAngleJudgeCondition (((data->V_ESensorValue[0] >= 30.0) ^ (data->V_ESensorValue[1] >= 30.0)) && (fabs(data->v_bias) >= 15.0))

#define RightAngleJudgeCondition ((fabs(data->v_difference) >= 30.0 && fabs(data->o_difference) <= 30.0))

#define RightAngleOutCondition   (rightAngleCount <= 0) && (data->V_ESensorValue[0] <=20.0 && data->V_ESensorValue[1] <= 20.0) && (data->H_ESensorValue[0] >=20.0 || data->H_ESensorValue[1] >=20.0 || data->H_ESensorValue[2] >= 20.0)

#define CrossJudgeCondition      ((fabs(data->o_difference >= 65.0)) && (fabs((data->v_sum >= 80.0))) && (fabs(data->o_sum >= 100.0)))
#define CrossOutJudgeCondition   ((fabs(data->o_difference >= 65.0) && fabs(data->v_sum >= 80.0)


#define CycleJudgeCondition      0//((data->H_ESensorValue[1] > 2 * data->H_ESensorValue[0]) || (data->H_ESensorValue[1] >= 2 * data->H_ESensorValue[2])) &&  (data->H_ESensorValue[1] >= 70.0)

#define LoseLineSearchedLineCondition (data->V_ESensorValue[0] <= 20.0 && data->V_ESensorValue[1] <= 20.0 && (data->H_ESensorValue[0] >= 40.0 || data->H_ESensorValue[1] >= 40.0 || data->H_ESensorValue[2] >= 40.0))
//#define LoseLineSearchedLineCondition (data->TrackingState == Normal_Tracking)

void LoseLine_Handler(data_t *data);
void Cycle_Handler(data_t *data);
void Cross_Handler(data_t *data);
void RightAngle_Handler(data_t *data);


float ElementDetermine(void *argv)
{
    data_t *data = (data_t *)argv;

//    if(CrossJudgeCondition)             //十字
//    {
//        data->Element.Type = Cross;
//    }

    if(RightAngleJudgeCondition && data->Element.Type != Cross)
    {
        data->Element.Type = RightAngle;
    }

    if(CycleJudgeCondition)
    {
        data->Element.Type = Cycle;
    }

    return data->Element.Type * 1.0;

}

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;


    switch(data->Element.Type)
    {
        case RightAngle:
            RightAngle_Handler(data);
            break;
        case Cross:
            //Cross_Handler(data);
            break;
        case Cycle:
            Cycle_Handler(data);
            break;
        default:
            break;
    }

    LoseLine_Handler(data);

    if(data->TrackingState == LoseLine)
    {
        DebugBeepOn;

    }
    else
    {
        DebugBeepOff;
    }
}

void Cross_Handler(data_t *data) //十字
{
    static cross_state_t crossState = CS_Wait;

    static incross_attitude_t inCrossAttitude = CS_UndefinedIn;

    static cross_info_t   crossInfo = CS_UndefinedInfo;

    static sint32_t crossInCnt = 0;

    static sint32_t crossCount = 0;

    static sint32_t crossTurnCount = 0;


    switch(crossState)
    {
        case CS_Wait:

            if(data->Element.Type == Cross)
                crossState = CS_Confirm;
            break;

        case CS_Confirm:    //确认是十字
            
            if(CrossJudgeCondition)
            {
                if(data->o_difference >= 50.0)        //入左十字
                {
                    crossInfo = CS_Left;
                }
                else if(data->o_difference <= -50.0)   //入右十字
                {
                    crossInfo = CS_Right;
                }
                else    //偏离过大 or 判断错误
                {
                    crossInfo = CS_UndefinedIn;
                }

                if(data->h_bias >= 30.0)  //右斜入十字
                {
                    inCrossAttitude = CS_RightIn;
                }
                else if(data->h_bias <= -30.0)  //左斜入十字
                {
                    inCrossAttitude = CS_LeftIn;
                }
                else    //中入十字
                {
                    inCrossAttitude = CS_MidIn;
                }

                GLED.ON(GLED.Self);

            }

            crossState = CS_In;

            break;

        case CS_In: //调整姿态

            crossInCnt = 50;

            if(crossInCnt <= 0)
            {
                crossState = CS_Tracking;
                crossCount = 300;
                crossInCnt = 0;
            }


            break;

        case CS_Tracking:

            crossCount--;

            if((fabs(data->v_difference) >= 20.0) && (fabs(data->o_difference)>= 20.0))
                data->Bias = fsign(data->v_difference) * 100.0;
            else
                data->Bias = data->h_bias;

            if(CrossJudgeCondition && (crossCount <= 0))
            {
                crossState = CS_Out;
                crossTurnCount = 0;
            }


//            if(crossInfo == CS_Right)
//            {
//                if((data->O_ESensorValue[0] >= 50.0) || (data->O_ESensorValue[1] >= 50.0))
//                {
//                    crossState = CS_Out;
//                }
//            }
//            else
//            {
//
//                if((data->O_ESensorValue[0] >= 50.0) || (data->O_ESensorValue[1] >= 50.0))
//                {
//                    crossState = CS_Out;
//                }
//            }

            break;
        case CS_Out:

            crossState = CS_Wait;
            data->Element.Type = None;

            break;

        default:

            break;
    }
}

void Cycle_Handler(data_t *data)
{
    static cycle_state_t cycleState = CC_Undefined;

    switch(cycleState)
    {
        case CC_Wait:

            if(data->Element.Type == Cycle)
                cycleState = CS_Confirm;
            break;

        case CC_Confirm:

            if(0)
            {

            }

            break;

        case CC_In:



            break;

        case CC_Tracking:

            break;
        case CS_Out:

            cycleState = CC_Wait;
            Unlock(data->Element);
            data->Element.Type = None;

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

    switch(rightAngleState)
    {
        case RA_Wait:

            if(data->Element.Type == RightAngle)
            {
                rightAngleState = RA_Confirm;
            }
            break;

        case RA_Confirm:

            if(RightAngleJudgeCondition)
            {
                rightAngleState = RA_Tracking;

                rightAngleCount = 50;

                GLED.ON(GLED.Self);
                //DebugBeepOn;
            }

            break;

        case RA_Tracking:

            data->Bias = fsign(data->v_difference) * 100.0;

            rightAngleCount--;

            if(RightAngleOutCondition)
            {
                rightAngleState = RA_Out;
            }

            break;

        case RA_Out:

            //DebugBeepOff;
            GLED.OFF(GLED.Self);
            rightAngleState = RA_Wait;
            data->Element.Type = None;

            break;

        case RA_Undefined:default:
            break;
    }

    lastBias = data->Bias;
}


//void LoseLine_Handler(data_t *data)
//{
//    static loseline_state_t loseLineState = LL_Wait;
//
//    int lossLineCount = 0;
//
//    static float lastBias = 0.0;
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
//        case LL_Lose:
//
//            loseLineState = LL_SearchLine;
//
//            break;
//
//        case LL_SearchLine:
//
//            if(data->Element.Type == Cross)
//            {
//
//            }
//
//            if(data->Element.Type == RightAngle)
//            {
//                data->Bias = lastBias;
//            }
//
//            if(data->V_ESensorValue[0] <= 20.0 && data->V_ESensorValue[1] <= 20.0 && (data->H_ESensorValue[0] >= 40.0 || data->H_ESensorValue[1] >= 40.0 || data->H_ESensorValue[2] >= 40.0))
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
//            break;
//
//        case LL_Undefined:default:
//
//            break;
//    }
//
//    lastBias = data->Bias;
//
//
//}


void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    static float bias = 0.0;

    int index = 0;

//    if(data->TrackingState == Normal_Tracking)
//    {
//        loseLineState = LL_Wait;
//        //bias = 0.0;
//    }

    switch(loseLineState)
    {
        case LL_Wait:
            if(data->TrackingState == LoseLine)
            {
                loseLineState = LL_Lose;
            }
            break;
        case LL_Lose:

            //DebugBeepOn;

            for(index = 1 ; index < 10 ; index++)
            {
                float *_eType = Queue.Gets(&data->ElementTypeQueue,-index,NULL,0,1);
                float *_trackingState = Queue.Gets(&data->TrackingQueue,-index,NULL,0,1);

                if(FIs_Equal(*_eType,data->Element.Type * 1.0) && (!FIs_Equal(*_trackingState,LoseLine * 1.0)))
                {
                    float *_bias = NULL;

                    if(data->Element.Type == None)
                        _bias = Queue.Gets(&data->RawBiasQueue,-index,NULL,0,1);
                    else
                        _bias = Queue.Gets(&data->ElementBiasQueue,-index,NULL,0,1);
                    bias = *_bias;

                    BLED.ON(BLED.Self);

                    break;
                }
                else
                {
                    if(!FIs_Equal(*_eType,data->Element.Type * 1.0))
                    {
                        break;
                    }
                }
            }

            if(index >= 10)
            {
                //data->NeedToBack = true;
            }
            else
            {
                //bias /= index;
            }

            if(data->NeedToBack)
            {
                data->Bias = 0.0;
                //BLED.ON(BLED.Self);

            }
            else
            {
                loseLineState = LL_SearchLine;
            }



            break;

        case LL_SearchLine:

            if(data->NeedToBack)
            {

            }
            else
            {
                data->Bias = fsign(bias) * 100.0;
            }

            if(LoseLineSearchedLineCondition)
            {
                loseLineState = LL_Searched;
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

