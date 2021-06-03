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

    if(Is_RightAngle(data))
    {
        data->Element.Type = RightAngle;
    }

    if(Is_Cross(data))     
    {
        data->Element.Type = Cross;
    }

    if(Is_Cycle(data))
    {
        data->Element.Type = Cycle;
        
    }

    if(Is_LoseLine(data))
    {
        data->TrackingState = LoseLine;
    }
    else
    {
        data->TrackingState = Normal_Tracking;
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
            Cross_Handler(data);
            break;
        case Cycle:
            Cycle_Handler(data);
            break;
        default:
            break;
    }

    LoseLine_Handler(data);
}

void Cycle_Handler(data_t *data)
{
    static cycle_state_t cycleState = CC_Undefined;
    static cycle_dir_t   cycleDir = CC_DirUndefined;

    static float bias = 0.0;

    static uint32_t cycleCnt;

    switch(cycleState)
    {
        case CC_Wait:

            if(data->Element.Type == Cycle)
                cycleState = CS_Confirm;
            break;

        case CC_Confirm:

            if(Is_Cycle(data))
            {
                if(data->H_ESensorValue[0] >= data->H_ESensorValue[2])
                {
                    cycleDir = CC_DirLeft;
                    bias =  100.0;
                }
                else
                {
                    cycleDir = CC_DirRight;
                    bias = -100.0;
                }
                
                cycleState = CC_In;
            }

            break;

        case CC_In:

            data->Bias = bias;

            if(data->h_bias <= 10.0)
            {
                cycleState = CC_Tracking;
            }
            
            break;
        case CC_Tracking:
            
            cycleCnt++;

            if(data->v_difference >= 30.0)
            {
                data->Bias = data->h_bias * 0.3 + data->v_bias * 0.7;
            }
            else
            {
                data->Bias = data->h_bias;
            }
            
            if(Is_CycleOut(data) || cycleCnt >= 6000)
            {
                cycleState = CC_Out;
            }

            break;

        case CC_Out:

            data->Bias = -bias;

            if(Is_CycleBackToStraight(data) || cycleCnt >= 6000)
            {
                cycleState = CC_Wait;
                data->Element.Type = None;
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
            }

            break;

        case RA_Confirm:

            if(Is_RightAngle(data))
            {

                rightAngleCount = 50;

                GLED.ON(GLED.Self);    

                bias = fsign(data->v_difference) * 100.0;

                rightAngleState = RA_Tracking;
            }

            break;

        case RA_Tracking:

            data->Bias = bias;

            rightAngleCount--;

            if(Is_RightAngleOut(data,rightAngleCount) || rightAngleCount <= -1000) 
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

        case RA_Undefined:default:
            break;
    }

    lastBias = data->Bias;
}


void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    static float bias = 0.0;

    int index = 0;

    bool isFindBias = false;

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
                float *_eSensorValue = Queue.Gets(&data->ESensorQueue,-index,NULL,0,7);

                if(FIs_Equal(*_eType,data->Element.Type * 1.0) && (!FIs_Equal(*_trackingState,LoseLine * 1.0)))
                {
                    float *_bias = NULL;

                    if(data->Element.Type == None)
                        _bias = Queue.Gets(&data->RawBiasQueue,-index,NULL,0,1);
                    else
                        _bias = Queue.Gets(&data->ElementBiasQueue,-index,NULL,0,1);

                    bias = *_bias;

                    BLED.ON(BLED.Self);

                    if(Is_LLBiasVaild(_eSensorValue,bias))
                    {
                        isFindBias = true;
                        break;
                    }
                }
                else
                {
                    if(!FIs_Equal(*_eType,data->Element.Type * 1.0))
                    {
                        break;
                    }
                }
            }

            if(isFindBias)
            {
                loseLineState = LL_SearchLine;
            }
            else
            {
                bias = 0.0;
                loseLineState = LL_BackSearchLine;

                data->Is_AdjustSpeed = true;
            }

            break;

        case LL_BackSearchLine:

            data->Bias = bias;

            data->Speed = -3200;

            if(Is_SearchedLine(data))
            {
                loseLineState = LL_Lose;
                data->Is_AdjustSpeed = false;
            }

            break;
        
        
        case LL_SearchLine:

            data->Bias = fsign(bias) * 100.0;

            data->Is_AdjustSpeed = false;

            if(Is_SearchedLine(data))
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
