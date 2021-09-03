/*-------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【注   意】 中断服务函数 不知道名字可以去startup_MIMXRT1064.s里面找
---------------------------------------------------------*/
#include "include.h"
#include "fsl_sdmmc_host.h"

float t1 = 0.0;
bool Rampflag=false;
uint16_t Distance;

//电机实时速度
int LeftCurrentSpeed;
int RightCurrentSpeed;
int LeftTempSpeed;
int RightTempSpeed;
int SpeedCount;
int LEdata=0;
int REdata=0;

bool CameraProcessFlag = false;//图像处理
bool CameraShowFlag = false;//显示图像
bool MotorProcessFlag = false;//速度环处理
bool ScreenshowFlag = false;//屏幕显示

extern bool OutPark;
extern uint8_t OutParkstate;

void PIT_IRQHandler(void)
{
  float ts = 0.0;
  
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    ts = os.time.getTime(ms);
    //清除标志位
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    Distance=0;
    Distance=ADC_Read(ADC2,ADC2_CH4_J14);
    if(Distance>=1500 )
    {
      Rampflag=true;
    }
    CameraProcessFlag = true;
     
    t1 = os.time.getTime(ms)-ts;
  }
  
  //定时1ms
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    //清除标志位
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
    
    //累加速度
    if(MotorProcessFlag == false)
    {
        SpeedCount++;
//        LeftTempSpeed += abs(LMotor.GetSpeed(LMotor.Self));
//        RightTempSpeed += abs(RMotor.GetSpeed(RMotor.Self));
        LeftTempSpeed += LMotor.GetSpeed(LMotor.Self);
        RightTempSpeed += -RMotor.GetSpeed(RMotor.Self);
        //获取当前速度
        if(SpeedCount >5)
        {
            SpeedCount = 0;
            MotorProcessFlag = true;
            LeftCurrentSpeed = LeftTempSpeed;
            RightCurrentSpeed = RightTempSpeed;
            if(LCircleState==Circle_State_4 || LCircleState==Circle_State_3 || LCircleState==Circle_State_5 || InParkstate==1 || LCircleState==Circle_State_6 || TrackState==Track_Prejudge || TrackState==Track_ApriTag_Back)
            {
              EncoderCount+=RightCurrentSpeed;
            }
            if(RCircleState==Circle_State_4 || RCircleState==Circle_State_3 || RCircleState==Circle_State_5 || InParkstate==2 || RCircleState==Circle_State_6 || InParkstate==3)
            {
              EncoderCount+=LeftCurrentSpeed;
            }
            LeftTempSpeed = 0;
            RightTempSpeed = 0;
        }
    }
    
  }
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_2)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    //清除标志位
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
//    EsenerProcess();
    
    //用户自行添加代码
    
  }
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    //清除标志位
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
    
    //用户自行添加代码
    
  }
  __DSB();
}









