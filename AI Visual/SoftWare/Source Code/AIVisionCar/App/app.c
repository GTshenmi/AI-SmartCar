/*
 * app.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"


//车库信息
bool OutPark=false;
uint8_t OutParkstate=0;
   
   
float time;
int tmp=48;
uint8_t ti;
extern int LeftCurrentSpeed;
extern int RightCurrentSpeed;
extern int LeftSpeedSet;
extern int RightSpeedSet;
extern bool CameraProcessFlag;
extern bool CameraShowFlag;
extern bool MotorProcessFlag; 
extern bool ScreenshowFlag;
extern uint8_t ObliqueInCrossStartRow;
extern uint16_t Distance;
extern bool Rampflag;


void ConsoleDemo()
{
    
    Console.Init();
    while(1)
    {  
      Console.WriteLine("%d",1);
//      Console.ReadLine("%d",tmp);
      tmp=UART_GetChar(LPUART7);
      Screen.Clear(Screen.Self,WHITE);
      Screen.WriteXLine(Screen.Self,1,"Data = %d",tmp);
      os.time.delay(0.5,s);
//        if(MotorProcessFlag == true)
//        {
////            LMotor.SetPwmValue(LMotor.Self,2000);
////            RMotor.SetPwmValue(RMotor.Self,2000);
//            MotorControl(80,80);
////            MotorProcessFlag = false;
//            Console.WriteLine("Sample:%d,%d",LeftCurrentSpeed,RightCurrentSpeed); 
//            MotorProcessFlag = false;
//        }
        
        //os.time.delay(0.5,s);
    }
}


void ESensorDemo()
{
    uint16_t adc_val[4] = {0,0,0,0};
    
    for(int i = 0 ; i < 4 ; i++)
    {
        ESensor[i].Init(ESensor[i].Self);
    }
    while(1)
    {
        Screen.Clear(Screen.Self,WHITE);
        
        for(int i = 0 ; i < 4 ; i++)
        {
                        
            adc_val[i] = ESensor[i].Read(ESensor[i].Self);
            Screen.WriteXLine(Screen.Self,i,"ADC[%d] = %u",i,adc_val[i]);
        }
        os.time.delay(0.2,s);
    }
    
}

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    RLED.ON(RLED.Self);
//    for(int i = 2 ; i < 4 ; i++)
//    {
//        ESensor[i].Init(ESensor[i].Self);
//    }
//    ConsoleDemo();
//    ESensorDemo(); 

    if(PIN_Read(B11) == 1)
    {
        Screen.Clear(Screen.Self,WHITE);
        CameraShowFlag = false;
    }
    else
    {
        CameraShowFlag = true;
    }
    
    //650:中值；220：最右；1120：最左
    Servo2.SetPwmValue(Servo2.Self,650);

    //处理出库
    if(OutPark==false)
    {
      Servo1.SetPwmValue(Servo1.Self,650); 
      LMotor.Driver(LMotor.Self,3000);
      RMotor.Driver(RMotor.Self,3000);
      os.time.delay(0.4,s);
      if(InParktype==1)
      {
        Servo1.SetPwmValue(Servo1.Self,730); 
      }
      else if(InParktype==2)
      {
        Servo1.SetPwmValue(Servo1.Self,570); 
      }
      LMotor.Driver(LMotor.Self,5000);
      RMotor.Driver(RMotor.Self,5000);
      os.time.delay(0.3,s);
    }
    
    
    while(1)
    {
      if(CameraProcessFlag == true)
      {
          CameraProcessFlag = false;
//          ti++;
//          if(ti>=150)
//          {
//            Console.WriteLine("%d",1);
//          }
//          Console.WriteLine("%d",1);
//          tcount=UART_GetChar(LPUART7);
          float times = os.time.getTime(ms);
          if(Capture.GetState(Capture.Self) == Capture_Fin)
          {      
              Capture.Read(Capture.Self,1);
          }  
          GetUsedImage(&Capture.ImageCache[0],&ImageUsed);//获取图像
          Image_Binarization(&ImageUsed,&ImageBinary,Image_GetOSTU(&ImageUsed));
          My_ImageProcess(&ImageBinary);
//          if(LCircleState==Circle_State_1 || RCircleState==Circle_State_1)
//          if(LCircleState==Circle_State_1 || RCircleState==Circle_State_1 || RCircleState==Circle_State_4 || LCircleState==Circle_State_4)
           if(LCircleState==Circle_State_1 || RCircleState==Circle_State_1 || RCircleState==Circle_State_4 || LCircleState==Circle_State_4 || GetDiff==true || InParkstate==1 || TrackState==Track_ApriTag || TrackState==Track_Prejudge)

          {
            PIN_InitConfig(K12,PIN_MODE_OUTPUT,0);
          }
          else
          {
            PIN_InitConfig(K12,PIN_MODE_OUTPUT,1);
          }
          ServoControl();
          SpeedControl();
          ImageProcessFinished = 1;
          time = os.time.getTime(ms) - times;
          
          if(Capture.GetState(Capture.Self) == Capture_Fin)
          {      
              Capture.Read(Capture.Self,1);
          } 
      }
      if(MotorProcessFlag == true)
      {
          MotorProcessFlag = false;
          MotorControl(LeftSpeedSet,RightSpeedSet);
      } 
      
      if(ImageProcessFinished == 1 && CameraShowFlag == true )
      {
          Screen.SetFontColor(Screen.Self,RED);
          Capture.ClearFinFlag(Capture.Self); 
//          Screen.WriteXLine(Screen.Self,11,"LS = %d",DiffEnd_L);
//          Screen.WriteXLine(Screen.Self,12,"LM = %d",Blackrate);
//          Screen.WriteXLine(Screen.Self,13,"LSl = %.3f",Blackrate);
//          Screen.WriteXLine(Screen.Self,15,"LS = %d",ApriTagState);
//          Screen.WriteXLine(Screen.Self,16,"RM = %d",CircleDectMid_Right);
//          Screen.WriteXLine(Screen.Self,14,"RSl = %.3f",CircleRightSlope);
//          Screen.WriteXLine(Screen.Self,15,"f1 = %.3f",temp_slope1);
//          Screen.WriteXLine(Screen.Self,16,"END = %d",DiffSRow);
//          Screen.WriteXLine(Screen.Self,17,"Circle = %d",circlestate);
//          Screen.WriteXLine(Screen.Self,18,"Diff = %d",CircleType);
//          Screen.WriteXLine(Screen.Self,11,"count = %d",Left_Add_EndRow);
//          Screen.WriteXLine(Screen.Self,12,"width = %d",Right_Add_EndRow);
//          Screen.WriteXLine(Screen.Self,17,"SR = %.3f",DectShapeRate);
          Screen.WriteXLine(Screen.Self,14,"Num = %d",AprilDir);
          Screen.WriteXLine(Screen.Self,16,"Tag = %d",leftCol);
//          Screen.WriteXLine(Screen.Self,16,"TR = %.3f",DectTrackRate);
          Screen.WriteXLine(Screen.Self,17,"Dir = %d",rightCol);
//          Screen.WriteXLine(Screen.Self,17,"TagDir = %d",AprilDir);
//          Screen.WriteXLine(Screen.Self,13,"S = %.3f",lslope);
//          Screen.WriteXLine(Screen.Self,14,"E = %.3f",rslope);
//          Screen.WriteXLine(Screen.Self,15,"END = %.3f",temp_slope1);
//          Screen.WriteXLine(Screen.Self,16,"ECR = %.3f",temp_slope2);
//          Screen.DrawHline(Screen.Self,0,50,120,1,BLACK);
          Screen.DrawHline(Screen.Self,0,ScanEndRow,120,1,GREEN);
//          Screen.DrawHline(Screen.Self,0,40,120,1,RED);
          Screen.DrawHline(Screen.Self,0,UPPatchRow,120,1,RED);
          Screen.DrawHline(Screen.Self,0,EndPatchRow,120,1,RED);
//          Screen.DrawHline(Screen.Self,0,70,120,1,RED);
          Capture.Show(Capture.Self,&ImageBinary,1);
          for(int i=90;i>=0;i--)
          {
              Screen.SetPixel(Screen.Self,PreLeftCol[i],i,BLUE);
              Screen.SetPixel(Screen.Self,PreRightCol[i],i,RED);
              Screen.SetPixel(Screen.Self,PreMidCol[i],i,PURPLE);
          }   
          
          //os.time.delayms(2000);
          ImageProcessFinished = 0;
      }
    }
    
}
