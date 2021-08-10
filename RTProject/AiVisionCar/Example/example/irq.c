/*-------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��ע   �⡿ �жϷ����� ��֪�����ֿ���ȥstartup_MIMXRT1064.s������
---------------------------------------------------------*/
#include "include.h"
#include "fsl_sdmmc_host.h"

float t1 = 0.0;
bool Rampflag=false;
uint16_t Distance;

//���ʵʱ�ٶ�
int LeftCurrentSpeed;
int RightCurrentSpeed;
int LeftTempSpeed;
int RightTempSpeed;
int SpeedCount;
int LEdata=0;
int REdata=0;

bool CameraProcessFlag = false;//ͼ����
bool CameraShowFlag = false;//��ʾͼ��
bool MotorProcessFlag = false;//�ٶȻ�����
bool ScreenshowFlag = false;//��Ļ��ʾ

extern bool OutPark;
extern uint8_t OutParkstate;

void PIT_IRQHandler(void)
{
  float ts = 0.0;
  
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    ts = os.time.getTime(ms);
    //�����־λ
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
  
  //��ʱ1ms
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    //�����־λ
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
    
    //�ۼ��ٶ�
    if(MotorProcessFlag == false)
    {
        SpeedCount++;
//        LeftTempSpeed += abs(LMotor.GetSpeed(LMotor.Self));
//        RightTempSpeed += abs(RMotor.GetSpeed(RMotor.Self));
        LeftTempSpeed += LMotor.GetSpeed(LMotor.Self);
        RightTempSpeed += -RMotor.GetSpeed(RMotor.Self);
        //��ȡ��ǰ�ٶ�
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
    //�����־λ
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
//    EsenerProcess();
    
    //�û�������Ӵ���
    
  }
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    //�����־λ
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
    
    //�û�������Ӵ���
    
  }
  __DSB();
}









