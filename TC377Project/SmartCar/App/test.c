/*
 * test.c
 *
 *  Created on: 2021年3月9日
 *      Author: 936305695
 */
#include "include.h"


void NNCU_Test(void)
{

//    sint8_t ad_database[6][7]={ {-47,-77,-92,-123,-127,-87,-49},\
//            {-127,-112,-76,-40,17,-127,-29},\
//            {-71,-46,29,-40,-71,-63,-74},\
//            {-68,-54,-1,-53,-74,-112,-10},\
//            {-26,-17,-3,-85,-105,-25,-95},\
//            {16,-59,-87,-123,-127,18,-112}\
//    };
    //
    //sint8_t ad_data[7] ={0,0,0,0,0,0,0};
    //
    //float angle_suffer[7]={0,0,0,0,0,0,0};
    //
    //float model_output[6] ={121.0,0.0,101.0,121.0,-74.0,41.0};

    sint16_t ad_database[10][7]={

            {-47,-77,-92,-123,-127,-87,-49},

            {-127,-112,-76,-40,17,-127,-29},

            {-71,-46,29,-40,-71,-63,-74},

            {-68,-54,-1,-53,-74,-112,-10},

            {-26,-17,-3,-85,-105,-25,-95},

            {16,-59,-87,-123,-127,18,-112},

             {-54,-45,-7,-64,-82,5,-108},

             {-54,-43,0,-62,-85,3,-110},

             {19,-36,-54,-105,-112,-26,-107},

             {10,-30,-42,-97,-105,-13,-103},
    };

//    sint8_t ad_database[10][7]={
//
//            {-57,-34,11,-66 ,-91 ,-55,-85},\
//
//            {-128, -121, -98 , -63 , 12 ,-127 ,-47},\
//
//            {-83 , -74  ,-14 , -28 , -31 ,-117  , 13},\
//
//            {27 , -26 , -49 ,-105 ,-111 ,  10 ,-103},\
//
//            {-124, -107 , -59 , -14  , 30 ,-117 ,   1},\
//
//            { -40,  -26  ,  4 , -72 , -92 , -87  ,-48},\
//
//            {-128, -113 , -89 , -62 ,  -2,  -87 , -24},\
//
//            {-128 , -123 , -106 , -78 , -16 , -128 , -49},\
//
//            { -65, -102 ,-118, -128 ,-128 , -46, -121},\
//
//            {-103 , -84 ,  -11 , -11 , -31 , -98, -42},\
//    };

    void* RunModel(const void *in_buf);

    sint16_t ad_data[7] ={0,0,0,0,0,0,0};

    sint16_t angle_suffer[10] ={0,0,0,0,0,0,0};

    sint16_t model_output[10] = {121,0,101,121,-74,41,84,41,-105,-42};

    float systime = os.time.getTime(s);
    float dt=0;

    sint8_t i=0;

    for(i=0;i<9;i++)
    {
        for(int j=0;j<7;j++)
            ad_data[j] = ad_database[i][j];

        //angle_suffer[i] = *(sint16_t *)RunModel(ad_data);

        angle_suffer[i] = *(sint16_t *)NNForWardReasoning(TestModel,ad_data,4);

        angle_suffer[i] = angle_suffer[i] >> (14 - 10 - 1);

        //根据模型信息对转角进行位移
        //servo_value = temp >> (Model1_Info.quant_bits - Model1_Info.frac_bits - 1);

        //训练模型时，转角是被压缩到了-128至127， 压缩时使用的公式为*128/1651，所以这里需要*1651/128进行还原，然后再用于转角控制
        //angle = (int16_t)(servo_value * (int32_t)1651 / 128);
    }
    dt = os.time.getTime(s) - systime;

    Screen.SetFontColor(Screen.Self,RED);

    for(i=0;i<9;i++)
        Screen.WriteLine(Screen.Self,"angle[%d] = %d",i,angle_suffer[i]);

    Screen.WriteLine(Screen.Self," ");

    Screen.SetFontColor(Screen.Self,BLUE);

    for(i=0;i<9;i++)
        Screen.WriteLine(Screen.Self,"output[%d] = %d",i,model_output[i]);

    Screen.WriteLine(Screen.Self,"Time = %.3f s",dt);

    while(1);

}

void Queue_Test(queue_t *queue)
{
    //EQueue.Init(queue);

    float array[7];

    float time = 0.0;

    float input[7];

    while(1)
    {
        for(int i = 0 ; i < 47 ; i++)
        {

            for(int i = 0 ; i < 7 ; i++)
            {
                input[i] = time;
            }

            //EQueue.Put(queue,input);

            time += 1.0;

        }

        Queue.Print(queue);

        Console.WriteLine("\n\n\n");

        //EQueue.Get(queue,99,array);

        Console.WriteArray("float",array,7);

        Console.WriteLine("\n\n\n");

        //EQueue.Get(queue,101,array);

        Console.WriteArray("float",array,7);

        Console.WriteLine("\n\n\n");

        //EQueue.Get(queue,-102,array);


        Console.WriteArray("float",array,7);

        Console.WriteLine("\n\n\n");


        os.time.delay(5.0,s);
    }
}
void NN_Test(void)
{

//    sint8_t ad_database[6][7]={ {-47,-77,-92,-123,-127,-87,-49},\
//            {-127,-112,-76,-40,17,-127,-29},\
//            {-71,-46,29,-40,-71,-63,-74},\
//            {-68,-54,-1,-53,-74,-112,-10},\
//            {-26,-17,-3,-85,-105,-25,-95},\
//            {16,-59,-87,-123,-127,18,-112}\
//    };
    //
    //sint8_t ad_data[7] ={0,0,0,0,0,0,0};
    //
    //float angle_suffer[7]={0,0,0,0,0,0,0};
    //
    //float model_output[6] ={121.0,0.0,101.0,121.0,-74.0,41.0};

    sint8_t ad_database[10][7]={

            {-47,-77,-92,-123,-127,-87,-49},

            {-127,-112,-76,-40,17,-127,-29},

            {-71,-46,29,-40,-71,-63,-74},

            {-68,-54,-1,-53,-74,-112,-10},

            {-26,-17,-3,-85,-105,-25,-95},

            {16,-59,-87,-123,-127,18,-112},

             {-54,-45,-7,-64,-82,5,-108},

             {-54,-43,0,-62,-85,3,-110},

             {19,-36,-54,-105,-112,-26,-107},

             {10,-30,-42,-97,-105,-13,-103},
    };

//    sint8_t ad_database[10][7]={
//
//            {-57,-34,11,-66 ,-91 ,-55,-85},\
//
//            {-128, -121, -98 , -63 , 12 ,-127 ,-47},\
//
//            {-83 , -74  ,-14 , -28 , -31 ,-117  , 13},\
//
//            {27 , -26 , -49 ,-105 ,-111 ,  10 ,-103},\
//
//            {-124, -107 , -59 , -14  , 30 ,-117 ,   1},\
//
//            { -40,  -26  ,  4 , -72 , -92 , -87  ,-48},\
//
//            {-128, -113 , -89 , -62 ,  -2,  -87 , -24},\
//
//            {-128 , -123 , -106 , -78 , -16 , -128 , -49},\
//
//            { -65, -102 ,-118, -128 ,-128 , -46, -121},\
//
//            {-103 , -84 ,  -11 , -11 , -31 , -98, -42},\
//    };

    float ad_data[7] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    float angle_suffer[10]={0,0,0,0,0,0,0};

    float model_output[10] = { 121,0,101,121,-74, 41, 84, 41,-105,-42 };
//    float model_output[10] = {
//            1 , 3  , 95 ,-121 ,   1 ,-121, -121  ,  0  ,  0 ,  20
//    };

    NNInit(TestModel);

    float systime = os.time.getTime(s);
    float dt=0;

    sint8_t i=0;

    for(i=0;i<9;i++)
    {
        for(int j=0;j<7;j++)
            ad_data[j] = ad_database[i][j] * 1.0 / 128.0;

        angle_suffer[i] = *(float *)NNForWardReasoning(TestModel,ad_data,4);



        //根据模型信息对转角进行位移
        //servo_value = temp >> (Model1_Info.quant_bits - Model1_Info.frac_bits - 1);

        //训练模型时，转角是被压缩到了-128至127， 压缩时使用的公式为*128/1651，所以这里需要*1651/128进行还原，然后再用于转角控制
        //angle = (int16_t)(servo_value * (int32_t)1651 / 128);
    }
    dt = os.time.getTime(s) - systime;

    Screen.SetFontColor(Screen.Self,RED);

    for(i=0;i<9;i++)
        Screen.WriteLine(Screen.Self,"angle[%d] = %.3f",i,angle_suffer[i]);

    Screen.WriteLine(Screen.Self," ");

    Screen.SetFontColor(Screen.Self,BLUE);

    for(i=0;i<9;i++)
        Screen.WriteLine(Screen.Self,"output[%d] = %.3f",i,model_output[i]/128.0);

    Screen.WriteLine(Screen.Self,"Time = %.3f s",dt);

    while(1);

}


void SPIx_Test(spix_t *spi)
{
    SPIx.Init(spi);

    uint8_t rx,tx;

    tx = 0;

    while(1)
    {
        tx ++;

        SPIx.ReadWriteBytes(spi,&tx,&rx,1,0);

        Screen.ClearLines(Screen.Self,1,2,WHITE);

        Screen.WriteXLine(Screen.Self,1,"tx = 0x%x",tx);
        Screen.WriteXLine(Screen.Self,2,"rx = 0x%x",rx);

        os.time.delay(1.0,s);
    }
}
void MotorTest(struct motor_ctrl *self)
{

    sint16_t t = 0;
    sint16_t speed = 0;

    self->SetSpeedLimit(self,10000,-10000);
    Screen.WriteXLine(Screen.Self,1,"Starting Test Function.");

    Screen.WriteXLine(Screen.Self,1,"Wait.");
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);


    Screen.WriteXLine(Screen.Self,1,"Stop ");
    self->Stop(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Sleep");
    self->Sleep(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Break");
    self->Break(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Starting Test Speed.");
    Screen.Clear(Screen.Self,WHITE);

    self->Start(self);

    while(1)
    {
        t++;

        if(t >((self->MaxSpeed - self->MinSpeed) * 2))
            t = 0;

        if(t > (self->MaxSpeed - self->MinSpeed))
            speed = 2 * (self->MaxSpeed - self->MinSpeed) + self->MinSpeed - t;
        else
            speed = self->MinSpeed + t;

        Screen.WriteXLine(Screen.Self,1,"Speed = %d",speed);
        Screen.Fill(Screen.Self,0,Screen.Font.Hight,Screen.Width,Screen.Font.Hight * 2,WHITE);


        self->SetSpeed(self,speed);
        os.time.delayms(20);
    }
}

void ServoTest(struct servo_ctrl *self)
{
//    self->SetAngleLimit(self,90,-90);
//    Screen.WriteXLine(Screen.Self,1,"Starting Test Function.");
//    Screen.Clear(Screen.Self,WHITE);

    sint16_t t = 0;
    sint16_t angle = 0;

//    Screen.WriteXLine(Screen.Self,1,"Wait.");
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//    Screen.WriteXLine(Screen.Self,1,"Start");
//    self->Start(self);
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//
//    Screen.WriteXLine(Screen.Self,1,"Stop ");
//    self->Stop(self);
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//    Screen.WriteXLine(Screen.Self,1,"Start");
//    self->Start(self);
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//    Screen.WriteXLine(Screen.Self,1,"Sleep");
//    self->Sleep(self);
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//    Screen.WriteXLine(Screen.Self,1,"Break");
//    self->Break(self);
//    self->SetAngle(self,self->MaxAngle);
//    os.time.delayms(2000);
//
//    Screen.WriteXLine(Screen.Self,1,"Starting Test Angle.");
//    Screen.Clear(Screen.Self,WHITE);

    self->Start(self);

    while(1)
    {
        t += 10;

        if(t >((self->MaxAngle - self->MinAngle) * 2))
            t = 0;

        if(t > (self->MaxAngle - self->MinAngle))
            angle = 2 * (self->MaxAngle - self->MinAngle) + self->MinAngle - t;
        else
            angle = self->MinAngle + t;

        Screen.WriteXLine(Screen.Self,1,"Angle = %d",angle);
        Screen.Fill(Screen.Self,0,Screen.Font.Hight,Screen.Width,Screen.Font.Hight * 2,WHITE);


        self->SetAngle(self,angle);
        self->Update(self);

        os.time.delayms(200);
    }
}


