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
void Normal_Tracking(data_t *data);

float ElementDetermine(void *argv)
{
    data_t *data = (data_t *)argv;

    if(data->v_difference >= 30.0)
    {
        SetValueWLock(data->Element,Type,RightAngle);
    }

    if((data->H_ESensorValue[1] > 2 * data->H_ESensorValue[0]) || (data->H_ESensorValue[1] >= 2 * data->H_ESensorValue[2]) &&  data->H_ESensorValue[1] >= 60.0)
    {
        SetValueWLock(data->Element,Type,Cross);
    }
}

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;

    if(data->TrackingState == LoseLine)
    {
        LoseLine_Handler(data);
    }
    else
    {
        switch(data->Element.Type)
        {
            case RightAngle:
                RightAngle_Handler(data);
                break;
            case Cross:
                Cross_Handler(data);
                

                break;
            
            default:
                break;
        }
    }

}

void Cross_Handler(data_t *data)
{
    cycle_state_t cycleState;

    switch(cycleState)
    {
        case CC_Wait:

            if(data->Element.Type == Cycle)
                cycleState = CC_Confirm;
            break;
        case CC_Confirm:    //确认是环岛
            
            
            Lock(data->Element);

            break;

        case CC_In:         //打角入环

            break;

        case CC_Tracking:   //环内正常寻迹

            break;
        case CC_Out:        //出环

            cycleState = CC_Wait;
            Unlock(data->Element);
            data->Element.Type = None;
            break;
        default:

            break;
    }
}

void Cycle_Handler(data_t *data)
{

}

void RightAngle_Handler(data_t *data)
{

}




void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    float weight[10] = {0.2,0.2,0.2,0.1,0.1,0.05,0.05,0.04,0.03,0.03};

    float *eSensorData = EQueue.Gets(&data->EQueue,0,NULL,0,7);

    float hESensorValue[3];
    float vESensorValue[2];

    hESensorValue[0] = eSensorData[1];
    hESensorValue[1] = eSensorData[3];
    hESensorValue[2] = eSensorData[5];

    vESensorValue[0] == eSensorData[0];
    vESensorValue[1] = eSensorData[6];


    

    float bias = 0.0;

    int lossLineCount = 0;

    switch(loseLineState)
    {
        case LL_Wait:
            if(data->TrackingState == LoseLine)
            {
                loseLineState = LL_Lose;
            }
            break;
        case LL_Lose:
            Lock(data->Element);

            for(int i = 0 ; i < 10 ;)       //取历史十次数据加权平均
            {
                float *_bias = EQueue.Gets(&data->EQueue,-i,NULL,7,8);
                float *_trackState = EQueue.Gets(&data->EQueue,-i,NULL,8,9);

                if(*_trackState == (float)LoseLine)
                {
                    lossLineCount++;
                }
                else
                { 
                    bias += *_bias;
                    i++;
                }
            }

            if(lossLineCount >= 10) //丢线超过10个点 
            {
                sint32_t rightcount = 0;
                sint32_t leftcount = 0;

                for(int i = 0 ; i < 10;)
                {
                    float *_bias = EQueue.Gets(&data->EQueue,-i,NULL,7,8);
                    float *_trackState = EQueue.Gets(&data->EQueue,-i,NULL,8,9);

                    if(*_trackState != (float)LoseLine)
                    { 
                        if(*_bias >= 0.0)
                        {
                            rightcount++;
                        }
                        else
                        {
                                leftcount--;
                        }
                    }
                }

                sint32_t angle = leftcount + rightcount;

                if(fabs(angle) <= 2)
                {
                    bias = 0.0;
                }
                else
                {
                    bias = fsign(angle);
                }
            }
            else
            {
                bias /= 10.0;
            }      

            break;

        case LL_SearchLine:
            data->Bias = bias * 100.0;

            if(vESensorValue[0] <= 20.0 && vESensorValue[1] <= 20.0 && (hESensorValue[0] >= 40.0 || hESensorValue[1] >= 40.0 || hESensorValue[2] >= 40.0))
            {
                loseLineState = LL_Searched; 
            }
            break;

        case LL_Searched:

            loseLineState = LL_Wait;
            data->TrackingState = Normal_Tracking;

            Unlock(data->Element);

            break;
    }
}

