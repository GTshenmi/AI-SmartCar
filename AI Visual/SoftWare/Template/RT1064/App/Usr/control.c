#include <control.h>
#include "include.h"

//#define SERVO_MINVALUE 665
//#define SERVO_MIDVALUE 740 
//#define SERVO_MAXVALUE 820

#define SERVO_MINVALUE 570
#define SERVO_MIDVALUE 650 
#define SERVO_MAXVALUE 730


extern bool OutPark;
extern uint8_t OutParkstate;

/*  方向环变量  */
int FarRow=55;
int LowRow=60;
long ServoError = 0;//中线偏差
long ServoLastError = 0;//上次中线偏差
int ServoAngle = 0;
int CurrentServoAngle = 0;
int LastServoAngle = 0;
int ServoPWM = 740;
float CalulateAngle[5];
float ServoSlope = 0.0;
float Servo_J = 0.00007;
float Servo_Kp = 0.45;
float Servo_KpMax = 0.9;
float Servo_KpAll = 0.0;
float Servo_KpInc = 0.0;
float Servo_K = 1.05;
float Servo_Kd = 0.9;
float Servo_KdInc = 0.0;
float Servo_KdAll = 0.0;
float Servo_KdMax = 0.2;
float Different_P = 0.004;
float Drop_P = 0.000047;
extern bool Rampflag; 

/*   速度变量   */
bool Car_Go=true;
int BasicSpeed = 80;         //基础速度
int SpecialDrop = 0;
int LeftSpeedSet;       //设置的左电机速度
int RightSpeedSet;      //设置的右电机速度
float DropSpeed;        //减速量
float LeftPWMSet;         //左电机PWM
uint32_t LeftPWM;
float RightPWMSet;        //右电机PWM
uint32_t RightPWM;


/*  速度环变量  */
float Motor_Kp = 180.0;
float Motor_Ki = 18.0;
float Motor_Kd = 0.0;

extern bool MotorProcessFlag;
extern int LeftCurrentSpeed;        //实时左电机速度
extern int RightCurrentSpeed;       //实时右电机速度
int LCurrentSpeed;              //本次左电机速度
int RCurrentSpeed;              //本次右电机速度
int LeftLastSpeed;              //上次左电机速度
int RightLastSpeed;             //上次右电机速度  
int LeftPreSpeed;               //上上次电机速度
int RightPreSpeed;              //上上次电机速度
int LeftCurrentError;           //本次左电机误差
int RightCurrentError;          //本次右电机误差
int LeftLastError;              //上次左电机误差
int RightLastError;             //上次右电机误差
int LeftPreError;              //上上次左电机误差
int RightLPreError;             //上上次右电机误差

void ServoControl(void)
{
    ServoLastError = ServoError;//上次中线偏差
    ServoError = ImageError();//获取中线偏差
    if(abs(ServoError) >= 80)
    {
        if(ServoError > 0)
        {
            ServoError = 80;
        }
        else
        {
            ServoError = -80;
        }
    }
  if(abs(ServoError) >=20)
   {
        Servo_KpInc = ServoError * ServoError * Servo_J;//动态PID值
   }
   else
   {
        Servo_KpInc = 0;
    }
    Servo_KdInc = Servo_KpInc*Servo_K;
    //Servo_KdAll = Servo_Kd + Servo_KdInc;
    Servo_KdAll = Servo_Kd;
    Servo_KpAll = Servo_Kp + Servo_KpInc;//PID总值
    
    if(Servo_KpAll > Servo_KpMax)
    {
        Servo_KpAll = Servo_KpMax;
    }
    if(Servo_KdAll > Servo_KdMax)
    {
        Servo_KdAll = Servo_KdMax;
    }
    LastServoAngle = ServoAngle;
    CurrentServoAngle = (int)(Servo_KpAll * (float)ServoError + Servo_KdAll * (float)(ServoError - ServoLastError));//计算打角值;
    ServoAngle = CurrentServoAngle;
    ServoPWM = ServoAngle + SERVO_MIDVALUE;//计算打角PWM值
    
    //特殊状态的打角处理
    if(InParkstate==3)//入库打角
    {
      if(InParktype==1)
      {
        ServoPWM=SERVO_MAXVALUE;
      }
      if(InParktype==2)
      {
        ServoPWM=SERVO_MINVALUE;
      }
    }
    else if(Diffstate==OutDiff_Go)
    {
      if(DiffState==1)
      {
        ServoPWM=SERVO_MAXVALUE;
      }
      if(DiffState==2)
      {
        ServoPWM=SERVO_MINVALUE;
      }
    }
    if(GetDiff==true && DiffState==0)//入库打角
    {
      ServoPWM=SERVO_MIDVALUE;
    }
//    else if(InParkstate==1)
//    {
//      ServoPWM=650;
//    }
    
    
    //打角限幅
    if(ServoPWM > SERVO_MAXVALUE)
    {
        ServoPWM = SERVO_MAXVALUE;
    }
    if(ServoPWM < SERVO_MINVALUE)
    {
        ServoPWM = SERVO_MINVALUE;
    }
    Servo1.SetPwmValue(Servo1.Self,ServoPWM); //打角
}

void MotorControl(int LSpeed,int RSpeed)
{
//    if(MotorProcessFlag == true)
//    {
        //刷新
        MotorProcessFlag = false;
        
        //左电机参数
        LeftPreSpeed = LeftLastSpeed;
        LeftLastSpeed = LCurrentSpeed;
        LCurrentSpeed = LeftCurrentSpeed;
        
        //右电机参数
        RightPreSpeed = RightLastSpeed;
        RightLastSpeed = RCurrentSpeed;
        RCurrentSpeed = RightCurrentSpeed;
        
        /*计算误差*/
        
        LeftPreError = LeftLastError;
        RightLPreError = RightLastError;
        
        LeftLastError = LeftCurrentError;
        RightLastError = RightCurrentError;
        
        LeftCurrentError = (LSpeed - LCurrentSpeed);
        RightCurrentError =(RSpeed - RCurrentSpeed);
        
        
        //计算PWM
        LeftPWMSet += (float)(Motor_Kp * (float)(LeftCurrentError - LeftLastError) + Motor_Ki * LeftCurrentError + Motor_Kd * (float)(LeftCurrentError - 2*LeftLastError + LeftPreError));
        RightPWMSet += (float)(Motor_Kp * (float)(RightCurrentError - RightLastError) + Motor_Ki * RightCurrentError + Motor_Kd * (float)(RightCurrentError - 2*RightLastError + RightLPreError));
        
        
        if(LeftPWMSet > 10000)
            LeftPWMSet = 10000;
        if(RightPWMSet > 10000)
            RightPWMSet = 10000;
        if(LeftPWMSet < -10000)
            LeftPWMSet = -10000;
        if(RightPWMSet < -10000)
            RightPWMSet = -10000;
        
        LeftPWM = (uint32_t)LeftPWMSet;
        RightPWM = (uint32_t)RightPWMSet;
        
        LMotor.SetPwmValue(LMotor.Self,LeftPWM);
        RMotor.SetPwmValue(RMotor.Self,RightPWM);
        if( TrackState==Track_ApriTag)
        {
          if(abs(LeftCurrentError)>=10 && LCurrentSpeed>=0)
          {
            LMotor.Driver(LMotor.Self,-13000);
          }
          if(abs(RightCurrentError)>=10 && RCurrentSpeed>=0)
          {
            RMotor.Driver(RMotor.Self,-13000);
          }
        } 
        if(Car_Stop==true)
        {
          if(abs(LeftCurrentError)>=30 && LCurrentSpeed>=0)
          {
            LMotor.Driver(LMotor.Self,-13000);
          }
          if(abs(RightCurrentError)>=30 && RCurrentSpeed>=0)
          {
            RMotor.Driver(RMotor.Self,-13000);
          }
        } 
        else if(Diffstate==InDiff_Stop)
        {
          if(abs(LeftCurrentError)>=5 && LCurrentSpeed>=0)
          {
            LMotor.Driver(LMotor.Self,-13000);
          }
          if(abs(RightCurrentError)>=5 && RCurrentSpeed>=0)
          {
            RMotor.Driver(RMotor.Self,-13000);
          }
        }
}

void SpeedControl(void)
{
  if(Car_Go)
  {
    if(GetDiff==true)
    {
      if(DiffState==1)
      {
        CalEndRow=48;
      }
      else if(DiffState==2)
      {
        CalEndRow=48;
      }
      BasicSpeed=60;
    }
    else if(DiffCount==1)
    {
      BasicSpeed=70;
    }
    else
    {
      CalEndRow=LowRow;
      BasicSpeed=72;
    }
    if(abs(ServoAngle) >= 30)
    {
        
        if(abs(ServoAngle) >= 40)
        {
              //DropSpeed = 10;
            DropSpeed = abs(ServoError) * abs(ServoError) * Drop_P * BasicSpeed;
            if(DropSpeed > 40)
            {
                DropSpeed = 40;
            }
        }
        if(ServoAngle<=0)
        {
          LeftSpeedSet = (int)(BasicSpeed - DropSpeed + Different_P * abs(ServoAngle) * (BasicSpeed-DropSpeed));
          RightSpeedSet = (int)(BasicSpeed - DropSpeed); 
        }
        else
        {
          LeftSpeedSet=(int)(BasicSpeed - DropSpeed);
          RightSpeedSet=(int)(BasicSpeed - DropSpeed + Different_P * abs(ServoAngle) * (BasicSpeed-DropSpeed));
        }
        if(LeftSpeedSet >=120 )
          LeftSpeedSet = 120;
        if(LeftSpeedSet <= 20)
          LeftSpeedSet = 20;
        if(RightSpeedSet >=120 )
          RightSpeedSet = 120;
        if(RightSpeedSet <= 20)
          RightSpeedSet = 20;
    }
    else
    {
        CalEndRow=FarRow;
        LeftSpeedSet = (int)(BasicSpeed - SpecialDrop);
        RightSpeedSet = (int)(BasicSpeed - SpecialDrop);
    }
  }
  else
  {
    if(TrackState==Track_ApriTag_Back)
    {
      LeftSpeedSet=-20;
      RightSpeedSet=-20;
    }
    LeftSpeedSet=0;
    RightSpeedSet=0;
  }
}


void EmergencyStop()
{
    
}










  
    
    
      
      
          
          
            
        
          
          
            
