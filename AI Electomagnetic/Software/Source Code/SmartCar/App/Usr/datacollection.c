/*
 * datacollection.c
 *
 *  Created on: 2021Äê7ÔÂ21ÈÕ
 *      Author: 936305695
 */
#include "datacollection.h"
#include "include.h"


uint SaveMotorSystemInfo(float *input,float *output,uint32_t len)
{
    char buffer[100];
    char *bufferPointer = buffer;
    float time = 0.0;

    for(int i = 0 ; i < len ; i++)
    {
        bufferPointer = buffer;

        time = i * 0.002;

        bufferPointer += sprintf(bufferPointer,"%f %f %f\n",time,input[i],output[i]);

        //SD.fastWrite("MotorSystem.txt",buffer);
        
        os.file.fastWrite("MotorSystem.txt",buffer);
    }

    return 0;
}

char *ElementDict[] =\
{
    "None",\
    "RightAngle",\
    "Cross",\
    "Cycle",\
    "Ramp",\
    "Lost",\
    "Straight",\
    "Corner",\
    "Other",\
    "Parameter",
};

uint SaveSensorDataAndAngleAI(void *argv,char *path){

    data_t *data = (data_t *)argv;

    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",data->SESensor_NormalizedValue[i]);
    }

    float anglecor = data->CorAngle / Servo.MaxAngle;

    bufferPointer += sprintf(bufferPointer,"%f ",anglecor);

    bufferPointer += sprintf(bufferPointer,"%f ",data->Speed);

    bufferPointer += sprintf(bufferPointer,ElementDict[data->Element.Point]);

    bufferPointer += sprintf(bufferPointer," AI E \n");

    line++;

    uint res = 0;

    float dt = os.time.getTime(s);

    //res = SD.fastWrite(path,buffer);
    
    res = os.file.fastWrite(path,buffer);

    dt = os.time.getTime(s) - dt;

    data->time = dt;

    return res;
}

uint SaveSensorDataAndAngleAI2(void *argv,char *path){

    data_t *data = (data_t *)argv;

    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",data->SESensor_NormalizedValue[i]);
    }

    for(int i = 0 ; i < 5 ; i++)
    {
        float angle = data->A[9 - i] / Servo.MaxAngle;
        bufferPointer += sprintf(bufferPointer,"%f ",angle);
    }

    bufferPointer += sprintf(bufferPointer,"%f ",data->Speed);

    bufferPointer += sprintf(bufferPointer,ElementDict[data->Element.Point]);

    bufferPointer += sprintf(bufferPointer," AI E \n");

    line++;

    uint res = 0;

    float dt = os.time.getTime(s);

    res = os.file.fastWrite(path,buffer);

    dt = os.time.getTime(s) - dt;

    data->time = dt;

    return res;
}

uint SaveSensorDataAndAngle(void *argv,char *path){

    data_t *data = (data_t *)argv;
    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",data->SESensor_NormalizedValue[i]);
    }

    float angle =   ((data->Angle)/ Servo.MaxAngle);

    bufferPointer += sprintf(bufferPointer,"%f ",angle);

    bufferPointer += sprintf(bufferPointer,"%f ",data->Speed);

    bufferPointer += sprintf(bufferPointer,ElementDict[data->Element.Point]);

    bufferPointer += sprintf(bufferPointer," LAutoBoot_AllNoCycle E \n");

    line++;

    uint res = 0;

    //res = SD.fastWrite(path,buffer);
    
    res = os.file.fastWrite(path,buffer);

    return res;

}


uint SaveParameterSD(void *argv,float *LADC_Value,float *SADC_Value,float *Angle){

    data_t *data = (data_t *)argv;
    char buffer[100];
    char* bufferPointer = buffer;

    static uint32_t line = 0;

    bufferPointer += sprintf(bufferPointer,"S %lu S ",line);

    for(uint8_t i = 0;i < MAX_SESENSOR_NUM;i++){
        bufferPointer += sprintf(bufferPointer,"%f ",SADC_Value[i]);
    }

    bufferPointer += sprintf(bufferPointer,"%f ",*Angle);

    bufferPointer += sprintf(bufferPointer,"%f E\n",data->Speed);

    line++;

    //return SD.fastWrite("Parameter.txt",buffer);
    
    return os.file.fastWrite("Parameter.txt",buffer);

}

typedef enum{
    NormalTrackingStraight,
    DeviateStraight,
    ReturnStraight,
}how_to_name_this_datatype;

void HowToNameThisFunc(void *argv)
{

    /*------------------dt1----------------*//*------------------dt2----------------*//*------------------dt3----------------*/
    /*---------------Running---------------*//*-----------------Sleep---------------*//*----------------WakeUp---------------*/

    data_t *data = (data_t *)argv;

    static how_to_name_this_datatype trackingstate;

    static float setTime = 0.0;

    static float deviateTime = 0.0;

    static float normalTime = 0.0;

    static float angle = 0.0;

    if(data->Element.Type == RightAngle)
    {
        trackingstate = NormalTrackingStraight;
    }


    switch(trackingstate)
    {
        case NormalTrackingStraight:

            normalTime = random(2.5,3.5) * 50.0;

            setTime = normalTime;

            trackingstate = DeviateStraight;

         break;

        case DeviateStraight:

            if(setTime <= 0.0)
            {
                data->IsAddNoise = true;
                //DebugBeepOn;

                float sign = (random(0.0,20.0) - 10) * 100.0;
                //uint8_t time = os.time.getTimeus() % 10000 / 100;
                deviateTime = random(0.15,0.25) * 50.0;//time/100.0f*0.7f;//random(0.1,0.7) * 50.0;
                setTime = deviateTime;

                deviateTime = fabs(deviateTime);

                angle = fsign(sign) * Servo.MaxAngle;
                //Servo.Sleep(Servo.Self);

                trackingstate = ReturnStraight;
            }

            break;

        case ReturnStraight:

            if(setTime <= 0.0)
            {

                data->IsAddNoise = false;

                if((data->h_bias <= 20.0 && data->v_sum <= 10.0))
                {
                    trackingstate = NormalTrackingStraight;
                    //DebugBeepOff;
                }

            }
            else
            {
                Servo.SetAngle(Servo.Self,angle);
                Servo.Update(Servo.Self);
            }
            break;
    }

    if(setTime >= -1.0)
        setTime -= 1.0;

}

void HowToNameThisFunc2(void *argv)
{

    /*------------------dt1----------------*//*------------------dt2----------------*//*------------------dt3----------------*/
    /*---------------Running---------------*//*-----------------Sleep---------------*//*----------------WakeUp---------------*/

    data_t *data = (data_t *)argv;

    static how_to_name_this_datatype trackingstate;

    static float setTime = 0.0;

    static float deviateTime = 0.0;

    static float normalTime = 0.0;

    static float bias = 0.0;

    if(data->Element.Type == Cycle)
    {
        trackingstate = NormalTrackingStraight;
        data->IsAddNoise = false;
        BEEP.OFF(BEEP.Self);
    }

    switch(trackingstate)
    {
        case NormalTrackingStraight:

            normalTime = random(1.5,2.5) * 500.0;

            setTime = normalTime;

            trackingstate = DeviateStraight;

         break;

        case DeviateStraight:

            if(setTime <= 0.0)
            {
                data->IsAddNoise = true;
                //DebugBeepOn;
                BEEP.ON(BEEP.Self);

                //float sign = (random(0.0,20.0) - 10) * 100.0;

                deviateTime = random(2.0,3.0) * 500.0;//time/100.0f*0.7f;//random(0.1,0.7) * 50.0;

                deviateTime = fabs(deviateTime);

                setTime = deviateTime;

                //bias = fsign(sign) * random(0,35.0);

                bias  = random(-45.0,45.0);

                //Servo.Sleep(Servo.Self);

                trackingstate = ReturnStraight;
            }

            break;

        case ReturnStraight:

            if(setTime <= 0.0)
            {
                BEEP.OFF(BEEP.Self);
                data->IsAddNoise = false;

                if((data->h_bias <= 20.0 && data->v_sum <= 10.0))
                {
                    trackingstate = NormalTrackingStraight;
                }
            }
            else
            {
                data->Bias += bias;

//                Servo.SetAngle(Servo.Self,angle);
//                Servo.Update(Servo.Self);
            }
            break;
    }

    if(setTime >= -1.0)
        setTime -= 1.0;
}
