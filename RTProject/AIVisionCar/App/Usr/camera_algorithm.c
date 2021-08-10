#include "camera_algorithm.h"
#include "include.h"

#define ImageHight 90
#define ImageWidth 120

#define LeftMiss 0
#define RightMiss 120

extern bool OutPark;
extern uint8_t OutParkstate;
extern int LeftCurrentSpeed;
extern int RightCurrentSpeed;
extern uint16_t Distance;
uint16_t ttttcount;

extern bool Rampflag;
uint16_t RampCount;

uint8_t CalEndRow=60;
uint8_t Error;
uint8_t Car_Stop;

/*                 参数修改                */

uint8_t DiffState=2;//分岔路转向：      1:左转；2：右转; 0:停车识别

uint8_t OutDiffway=1;//出库方向         1：左出；2：右出

uint8_t InParktype=1;//入库方向         1：左库；2：右库

uint8_t AprilTagJudge=0;//是否识别AprilTag码方向    0：识别；1：不识别

uint8_t InParkcount=0;
















//车辆状态
uint8_t Carstate=0;//0：正常行驶状态  ;  1:分岔路停车阶段
track_t TrackState=Track_OK;



//电感信息
uint16_t temp_ldata;//电感原始数据
uint16_t temp_rdata;//电感原始数据
uint8_t Ecount;


//搜线信息
int PreLeftCol[90];//左边界
int PreRightCol[90];//右边界
uint8_t PreMidCol[90];//中线
int MidError[90];
uint8_t SerachPoint[90];//搜线起始点
uint8_t TrackWidth[90];//赛道宽度
uint8_t ScanRow;//搜线起点
uint8_t ScanEndRow;//搜线终点
uint8_t LastEndCol;
uint8_t FirstGetBod_Left;
uint8_t FirstGetBod_Right;
bool FindLeftCol[90];
bool FindRightCol[90];


//边界状态信息
uint8_t WrongTarckWidth;//赛道异常宽度
uint8_t WrongTarckWidthCount;//路宽异常计数
bool TarckWidthWrong=false;//赛道宽度异常
bool LeftColState[90];//生长方向正常为true
bool RightColState[90];//生长方向异常为false


//环岛信息
circle_t LCircleState=NoneCircle;//环岛状态，初始化为无环岛
circle_t RCircleState=NoneCircle;//环岛状态，初始化为无环岛
uint8_t CircleType;//环岛类型：0，无环岛；1，左环岛；2，右环岛
uint8_t CircleDectFlag_Left;//环岛检测开始_左边界：0，无；1，开始；2，中间；3.结束
uint8_t CircleDectFlag_Right;//环岛检测开始_右边界：0，无；1，开始；2，中间；3.结束
uint8_t CircleDectStart_Left;//搜环岛开始点
uint8_t CircleDectStart_Right;//搜环岛开始点
uint8_t CircleDectMid_Left;//搜环岛中间点
uint8_t CircleDectMid_Right;//搜环岛中间点
uint8_t CircleDectEnd_Left;//搜环岛结束点
uint8_t CircleDectEnd_Right;//搜环岛结束点
uint8_t CircleLeftCount;//计数
float CircleLeftSlope;//斜率
uint8_t CircleRightCount;//计数
uint8_t Circletmpnum;
float lslope;
float rslope;
float CircleRightSlope;//斜率
float cirtpslope1;
float cirtpslope2;
float temp_slope1;
float temp_slope2;
int EncoderCount;
uint16_t tcount;
uint8_t temp_count;
uint8_t temp_count1;
int temp1;
int temp2;
int temp3;
uint8_t CircleInCount;//环内计数
uint8_t CirclePatchStart;//环内补线开始点
uint8_t CirclePatchEnd;//环内补线结束点
float CircleSlope;//环岛补线斜率
uint16_t CircleIncount=0;//入环计数
uint8_t temp_circle[90];
uint8_t CircleOutRow;//出环岛补线点
uint8_t outcircletype;//0：不出；1：有线可补；2：无线可补
uint8_t circlestate;
uint8_t RightCircle_OK;
uint8_t RightCircle_OKcount;
uint8_t LeftCircle_OK;
uint8_t CricleE;//环岛终点
uint8_t CircleOut;
float CircleOutSlope;
bool circlejudge2;

//分岔路信息
bool NumberGet=false;
diff_t Diffstate=NoneDiff;//初始化为无分岔路
uint8_t DiffRoadCount;//分岔路判定计数
uint8_t DiffEndRow;//分岔路结束点
uint8_t DiffEnd_L;
uint8_t DiffEnd_R;
uint8_t DiffSRow;
uint8_t DiffSCol_L;//分岔路检测起点纵坐标
uint8_t DiffSCol_R;//分岔路检测起点纵坐标
uint8_t DiffECol_L;//分岔路检测左终点纵坐标
uint8_t DiffECol_R;//分岔路检测右终点纵坐标
float DiffLslope;
float DiffRslope;
uint8_t DiffCount=0;//分岔路计数
uint8_t Diftempcount;
bool DiffRoadJudge;//分岔路判断标志位
bool DiffFlag;//分岔路标志位
bool DIffF;//分岔路总标志
bool GetDiff=false;
uint8_t diff;

//ApriTag码信息
uint8_t ApriTagState;//ApriTag检测状态
uint8_t Blackrate;//黑块计数
uint8_t BlackPoint;//黑点计数
uint8_t LeftColPos;//左边界位置
uint8_t RightColPos;//右边界位置
uint8_t AprilStart;//黑色起始点
uint8_t AprilEnd;//黑色起始点
uint8_t BlackWidth;//中间黑块宽度
uint8_t AprilWidth;//识别赛道宽度
uint8_t StartLeftCol;
uint8_t StartRightCol;
uint8_t Aprilget=1;
bool AprilTagflag;
bool ApriTagGet=false;//检测到ApriTag
uint8_t NumJudge;
uint8_t ApriTagNum;
uint8_t AprilDir;
uint8_t WrongStartL;
uint8_t WrongStartR;
uint8_t WrongEndL;
uint8_t WrongEndR;


bool DectPark;
bool InParkflag;
bool InPark=false;
uint8_t InParkstate;



//赛道内异常信息
uint8_t TrackWrongStart;//赛道异常开始行
uint8_t TrackWrongEnd;//赛道异常结束行
uint8_t TrackEnd;//检查终止行
uint8_t TrackLCol[90];
uint8_t TrackRCol[90];
uint8_t LStart;
uint8_t RStart;

//补线信息
int Left_Add[90];//左补线
int Right_Add[90];//右补线
uint8_t LeftAdd_State;//补线状态
uint8_t RightAdd_State;
/*补线状态
0：无补线
1：有--无
2：无--有
*/
uint8_t LeftAddFlag_count;//补线标志计数
uint8_t RightAddFlag_count;//补线标志计数
uint8_t Left_Add_StartRow;//左边界补线开始行
uint8_t Right_Add_StartRow;//右边界补线开始行
uint8_t Left_Add_EndRow;//左边界补线结束行
uint8_t Right_Add_EndRow;//右边界补线结束行
float LeftRepairSlope;//左边界补线斜率
float RightRepairSlope;//右边界补线斜率
bool IF_LeftRequire_Add;//预判断左边界需要修补
bool IF_RightRequire_Add;//预判断右边界需要修补
bool LeftFirstRepairFin;//左边界初步补线完成
bool RightFirstRepairFin;//左边界初步补线完成
bool LeftSecRepairFin;//左边界初步补线完成
bool RightSecRepairFin;//左边界初步补线完成
bool LeftRequire_Add;//左边界需要修补
bool RightRequire_Add;//右边界需要修补



/*         整行搜索信息         */
uint8_t DectLeftCol;//搜索本行的左边界
uint8_t DectRightCol;//搜索本行的右边界
uint8_t DectLSP;//搜索本行的异常开始左边界
uint8_t DectRSP;//搜索本行的异常开始右边界
uint8_t DectUSP;//搜索本行的异常开始上边界
uint8_t DectDSP;//搜索本行的异常开始下边界
uint8_t DectBlackPoint;//搜索本行赛道内黑点数
uint8_t DectJumpRow;//搜索跳变点
uint8_t DectTrackWidth;//本行赛道宽度
uint8_t DectShapeWidth;//本行异常宽度
uint8_t DectTrackMid;//计算异常中点
uint8_t UPPatchRow;//上补线行
uint8_t DownPathRow;//下补线行
uint8_t TrackEndCount=1;
float DectTrackRate;
float DectShapeRate;
uint8_t UDnum;
uint16_t ttcount;
uint8_t SBthing;//搜索行的跳变沿数
//uint8_t Numcount;
uint8_t LeftCol;//左边界
uint8_t RightCol;//右边界
int JumpRow;//跳变沿
bool ApriTag=false;
uint16_t tttcount;

uint8_t leftCol;//左边界
uint8_t rightCol;//右边界
int jumpRow;//跳变沿
int GetOne=0;

uint8_t EndPatchRow;
uint8_t EndSearchRow;
float EndPatchlslope;
float EndPatchrslope;





/*
函数：My_ImageProcess
功能：图像处理
作者：戴进
日期：2021.5.30
参数：
imagein:Sobel二值化图像
imagein2:大津法二值化图像
*/
void My_ImageProcess(image_t *imagein)
{
  WrongStartL=0;
  WrongStartR=0;
  WrongEndL=0;
  WrongEndR=0;
  
  EndPatchlslope=0.0;
  EndPatchrslope=0.0;
  CircleOut=89;
  EndPatchRow=0;
  EndSearchRow=60;
  UDnum=0;
  LeftCol=0;
  RightCol=0;
  JumpRow=0;
  leftCol=0;
  rightCol=0;
  jumpRow=0;
  SBthing=0;
  //初始化补线信息
  ScanRow=89;
  ScanEndRow=0;
  LeftAdd_State=0;
  RightAdd_State=0;
  Left_Add_StartRow=90;
  Right_Add_StartRow=90;
  Left_Add_EndRow=0;
  Right_Add_EndRow=0;
  WrongTarckWidth=120;
  WrongTarckWidthCount=0;
  LeftAddFlag_count=0;
  RightAddFlag_count=0;
  FirstGetBod_Left=0;
  FirstGetBod_Right=0;
  TarckWidthWrong=false;
  IF_LeftRequire_Add=false;
  IF_RightRequire_Add=false;
  LeftRequire_Add=false;
  RightRequire_Add=false;
  LeftFirstRepairFin=false;
  RightFirstRepairFin=false;
  LeftSecRepairFin=false;
  RightSecRepairFin=false;
  
  //环岛信息初始化
  CircleType=0;
  CricleE=90;
  CircleDectFlag_Left=0;
  CircleDectFlag_Right=0;
  CircleDectStart_Left=0;
  CircleDectStart_Right=0;
  CircleDectMid_Left=0;
  CircleDectMid_Right=0;
  CircleDectEnd_Left=0;
  CircleDectEnd_Right=0;
//  LEdata=0;
//  REdata=0;
  temp_count=0;
  CircleLeftCount=0;
  CircleLeftSlope=0.0;
  CircleRightCount=0;
  CircleRightSlope=0.0;
  temp_slope1=0.0;
  temp_slope2=0.0;
  temp2=0;
  temp3=0;
  CircleInCount=0;
  CirclePatchStart=0;
  CirclePatchEnd=0;
  CircleSlope=0.0;
  temp_count1=0;
  CircleOutRow=0;
  outcircletype=0;
  RightCircle_OK=0;
  RightCircle_OKcount=0;
  lslope=0.0;
  rslope=0.0;
  cirtpslope1=0.0;
  cirtpslope2=0.0;
  
  //分岔路信息初始化
  DiffRoadCount=0;
  DiffEndRow=0;
  DiffEnd_L=0;
  DiffEnd_R=0;
  DiffSRow=0;
  DiffSCol_L=0;
  DiffSCol_R=0;
  DiffECol_L=0;
  DiffECol_R=0;
  DiffLslope=0.0;
  DiffRslope=0.0;
  DiffFlag=false;
  DiffRoadJudge=false;
  
  //AprilTag参数初始化
  Blackrate=0;
  BlackPoint=0;
  AprilStart=0;
  AprilEnd=0;
  AprilWidth=0;
  BlackWidth=0;
  StartLeftCol=0;
  StartRightCol=120;
  
  //赛道内异常信息
  TrackWrongStart=0;
  TrackWrongEnd=0;
  TrackEnd=0;
  
  
  //整行搜索信息初始化
  DectLeftCol=0;//搜索本行的左边界
  DectRightCol=120;//搜索本行的右边界
  DectLSP=0;//搜索本行的异常开始左边界
  DectRSP=120;//搜索本行的异常开始右边界
  DectUSP=0;
  DectDSP=0;
  UPPatchRow=0;
  DownPathRow=0;
  DectBlackPoint=0;//搜索本行赛道内黑点数
  DectJumpRow=0;//搜索跳变点
  SBthing=0;
  DectTrackWidth=0;
  DectShapeWidth=0;
  DectTrackRate=0.0;
  DectShapeRate=0.0;
//  Numcount1=0;
//  Numcount2=0;
  
  
  //初始化图像
  for(int i=0 ;i<90 ;i++)
  {
    PreLeftCol[i]=0;
    PreRightCol[i]=120;
//    PreMidCol[i]=60;
    SerachPoint[i]=60;
    Left_Add[i]=0;
    Right_Add[i]=120;
    LeftColState[i]=true;
    RightColState[i]=true;
    FindLeftCol[i]=false;
    FindRightCol[i]=false;
  }
  
  //特殊处理第一行数据，确定搜线中点
  SerachPoint[89]=FirstRowHandle(&ImageBinary);
 
  //处理坡道
  
  if(Rampflag==true && GetDiff==false)
  {
//    if(EncoderCount>=8000)
//    {
//      EncoderCount=0;
//      Rampflag=false;
//    }
    RampCount++;
    if(RampCount>=100)
    {
      RampCount=0;
      Rampflag=false;
    }
  }

  
  //获取环岛终点信息
  for(int i=89;i>=0;i--)
  {
    if(CricleE==90 && imagein->Array[i][60].binary==1)
    {
      CricleE=i;
      break;
    }
  }
  //列扫描
  for(int i=ScanRow;i>=ScanEndRow;i--)
  {
    //行扫描
    for(int j=SerachPoint[i];j>=0;j--)
    {
      if(imagein->Array[i][j].binary==1)
      {
        PreLeftCol[i]=j;
        FindLeftCol[i]=true;
        break;
      }
    }
    for(int j=SerachPoint[i];j<=120;j++)
    {
      if(imagein->Array[i][j].binary==1)
      {
        PreRightCol[i]=j;
        FindRightCol[i]=true;
        break;
      }
    }
    PreMidCol[i]=(int)((PreLeftCol[i]+PreRightCol[i])/2);
    TrackWidth[i]=PreRightCol[i]-PreLeftCol[i];//计算赛道宽度
    if(TrackWidth[i]>=TrackWidth[i+1] && TarckWidthWrong==false && i<70)
    {
      TarckWidthWrong=true;
      WrongTarckWidthCount++;
      WrongTarckWidth=TrackWidth[i+1];
    }
    if(TrackWidth[i]>=WrongTarckWidth && TarckWidthWrong==true)
    {
      WrongTarckWidthCount++;
    }
    
    /* 环岛判定条件记录 */
    if(CircleType==0)
    {
      if(CircleDectFlag_Left==0)//尚未开始检测环岛左边界
      {
        if(FindLeftCol[i]==true)//如果寻找到左边界
        {
          CircleDectFlag_Left=1;//开始搜索
          CircleDectStart_Left=i;
        }
      }
      else if(CircleDectFlag_Left==1)
      {
        if(PreLeftCol[i]>=PreLeftCol[i+1])
        {
          CircleLeftCount++;
        }
        else
        {
          if(CircleLeftCount>=10)
          {
            CircleDectMid_Left=i;
            CircleLeftSlope=((float)PreLeftCol[CircleDectMid_Left]-(float)PreLeftCol[CircleDectStart_Left])/(float)(CircleDectStart_Left-CircleDectMid_Left);
            lslope=CircleLeftSlope;
            if(CircleLeftSlope>=0.6 && CircleLeftSlope<=2.5&& FindRightCol[i]==true)
            {
              CircleDectFlag_Left=2;
              CircleLeftCount=0;
            }
          }
        }
      }
      else if(CircleDectFlag_Left==2)
      {
        if((PreLeftCol[i]<=PreLeftCol[i+1]) && FindLeftCol[i]==true)
        {
          CircleLeftCount++;
        }
        if(CircleLeftCount>=10 && FindLeftCol[i]==false)
        {
          CircleDectEnd_Left=i;
          CircleLeftSlope=((float)PreLeftCol[CircleDectEnd_Left]-(float)PreLeftCol[CircleDectMid_Left])/(float)(CircleDectMid_Left-CircleDectEnd_Left);
          rslope=CircleLeftSlope;
          if(CircleLeftSlope<=-1.5 && CircleLeftSlope>=-4.0 && FindRightCol[i]==true)
          {
            CircleDectFlag_Left=3;
          }
        }
      }
      else
      {
        for(int i=CircleDectEnd_Left;i<=CircleDectStart_Right;i++)
        {
          if(FindRightCol[i]==true)
          {
            temp_count++;
          }
          if(temp_count>=(CircleDectStart_Right-CircleDectEnd_Left-1))
          {
            CircleType=1;
            temp_count=0;
          }
        }
      }
      if(CircleDectFlag_Right==0)//尚未开始检测环岛右边界
      {
        if(FindRightCol[i]==true)
        {
          CircleDectFlag_Right=1;
          CircleDectStart_Right=i;
        }
      }
      else if(CircleDectFlag_Right==1)
      {
        if(PreRightCol[i]<=PreRightCol[i+1])
        {
          CircleRightCount++;
        }
        else
        {
          if(CircleRightCount>=10)
          {
            CircleDectMid_Right=i;
            CircleRightSlope=((float)PreRightCol[CircleDectMid_Right]-(float)PreRightCol[CircleDectStart_Right])/(float)(CircleDectStart_Right-CircleDectMid_Right);
            lslope=CircleRightSlope;
            CircleDectFlag_Right=2;
            if(CircleRightSlope<=-0.6 && CircleRightSlope>=-2.5 && FindLeftCol[i]==true)
            {
              CircleDectFlag_Right=2;
              CircleRightCount=0;
            }
          }
        }
      }
      else if(CircleDectFlag_Right==2)
      {
        if(PreRightCol[i]>=PreRightCol[i+1] && FindRightCol[i]==true)
        {
          CircleRightCount++;
        }
        if(CircleRightCount>=10 && FindRightCol[i]==false)
        {
          CircleDectEnd_Right=i;
          CircleRightSlope=((float)PreRightCol[CircleDectEnd_Right]-(float)PreRightCol[CircleDectMid_Right])/(float)(CircleDectMid_Right-CircleDectEnd_Right);
          rslope=CircleRightSlope;
          if(CircleRightSlope>=1.5 && CircleRightSlope<=4.0 && FindLeftCol[i]==true)
          {
            CircleDectFlag_Right=3;
          }
        }
      }
      else
      {
        for(int i=CircleDectEnd_Right;i<=CircleDectStart_Left;i++)
        {
          if(FindLeftCol[i]==true)
          {
            temp_count++;
          }
        }
        if(temp_count>=(CircleDectStart_Left-CircleDectEnd_Right-1))
        {
          CircleType=2;
          temp_count=0;
        }
      }
    }
    
/*         END         */
    
    
    /* 环岛判定 */
    switch(LCircleState)//左环
    {
      //没有环
      case NoneCircle:
      {
        ;
      }break;
      //环出现，有黑环
      case Circle_State_1:
      {
        circlestate=1;
        
        if(CircleDectStart_Left<=50 && CircleDectStart_Left>=10)
        {
          //判断丢线情况
          
          LCircleState=Circle_State_2;
        }
      }break;
      case Circle_State_2://黑环消失，开始补线
      {
        circlestate=2;
        CircleIncount=0;
        if(FindLeftCol[i]==true && CirclePatchEnd==0)
        {
          temp_count++;
          if(temp_count>=5)
          {
            CirclePatchStart=89;
            CirclePatchEnd=i;
          }
        }
        if(CirclePatchEnd>=60)
        {
          LCircleState=Circle_State_3;
          temp_count1=0;
        }
      }break;
      case Circle_State_3:
      {
        CircleIncount++;
        circlestate=3;
        if(CircleIncount>=70 && Right_Add_StartRow<=70 && Right_Add_StartRow!=0 && EncoderCount>=15000)
        {
          
          if(FindRightCol[i]==false)
          {
//            temp_count1++;
//            if(temp_count1>=5)
//            {
//              temp_count1=0;
              CircleIncount=0;
              EncoderCount=0;
              LCircleState=Circle_State_4;
//            }
          }
        }
      }break;
      case Circle_State_4:
      {
        circlestate=4;
//        EncoderCount+=-RMotor.GetSpeed(RMotor.Self);
        if(EncoderCount>=4500)
        {
          EncoderCount=0;
          LCircleState=Circle_State_5;
        }
        if(FindRightCol[70]==true)
        {
          CircleOutRow=Right_Add_StartRow;
          outcircletype=1;
        }
        else
        {
          if(i<70)
          {
            temp_count1++;
            if(temp_count1>=10)
            {
              temp_count1=0;
              outcircletype=2;
              CircleOutRow=89;
            }
          }
        }
      }break;
      case Circle_State_5:
      {
        circlestate=5;
        if(EncoderCount>=2500)
        {
          EncoderCount=0;
          LCircleState=Circle_State_6;
        }
        
      }break;
      case Circle_State_6:
      {
        circlestate=5;
        if(EncoderCount>=8000)
        {
          EncoderCount=0;
          LCircleState=NoneCircle;
          circlestate=0;
        }
      }break;
    }
    
    switch(RCircleState)//右环
    {
      //没有环
      case NoneCircle:
      {
        ;
      }break;
      //环出现，有黑环
      case Circle_State_1:
      {
        circlestate=1;
        if(CircleDectStart_Right<=50 && CircleDectStart_Right>=10)
        {
          RCircleState=Circle_State_2;
        }
      }break;
      case Circle_State_2://黑环消失，开始补线
      {
        circlestate=2;
        CircleIncount=0;
        if(FindRightCol[i]==true && CirclePatchEnd==0)
        {
          temp_count++;
          if(temp_count>=5)
          {
            CirclePatchStart=89;
            CirclePatchEnd=i;
          }
        }
        
        if(CirclePatchEnd>=60)
        {
          RCircleState=Circle_State_3;
          temp_count1=0;
        }
      }break;
      case Circle_State_3:
      {
        CircleIncount++;
        circlestate=3;
        if(CircleIncount>=70 && Left_Add_StartRow<=70 && Left_Add_StartRow!=0 && EncoderCount>=12000)
        {
          if(FindLeftCol[i]==false)
          {
//            temp_count1++;
//            if(temp_count1>=5)
//            {
//              temp_count1=0;
              CircleIncount=0;
              EncoderCount=0;
              RCircleState=Circle_State_4;
//            }
          }
        }
      }break;
      case Circle_State_4:
      {
        circlestate=4;
//        EncoderCount+=LMotor.GetSpeed(LMotor.Self);
        if(EncoderCount>=4800)
        {
          EncoderCount=0;
          RCircleState=Circle_State_5;
        }
        if(FindLeftCol[70]==true)
        {
          CircleOutRow=Left_Add_StartRow;
          outcircletype=1;
        }
        else
        {
          if(i<70)
          {
            temp_count1++;
            if(temp_count1>=10)
            {
              temp_count1=0;
              outcircletype=2;
              CircleOutRow=89;
            }
          }
        }
      }break;
      case Circle_State_5:
      {
        circlestate=5;
        if(EncoderCount>=2500)
        {
          EncoderCount=0;
          RCircleState=Circle_State_6;
        }
        
      }break;
      case Circle_State_6:
      {
        circlestate=5;
        if(EncoderCount>=8000)
        {
          EncoderCount=0;
          RCircleState=NoneCircle;
          circlestate=0;
        }
      }break;
    }
    
    /*          END            */
    
    
    /*          分叉判断条件记录           */
    if(i<=70)
    {
      if(FindLeftCol[i]==false && FindRightCol[i]==false)
      {
        DiffRoadCount++;
        if(DiffRoadCount>=5)
        {
          DiffRoadJudge=true;
        }
      }
    }
    
    
    /* 赛道状态记录 */
    if(FindLeftCol[i]==true && FirstGetBod_Left==0)
    {
      FirstGetBod_Left=i;
    }
    if(FindRightCol[i]==true && FirstGetBod_Right==0)
    {
      FirstGetBod_Right=i;
    }
    
    if(i>=70&&i<=79)
    {
      if(FindLeftCol[i]==false)
      {
        LeftAddFlag_count++;
      }
      if(FindRightCol[i]==false)
      {
        RightAddFlag_count++;
      }
    }
    
    //左边界状态记录
    if(i<70)//前十行开外才会开始判断
    {
      if(FindLeftCol[69]==true)
      {
        if(PreLeftCol[i]>=PreLeftCol[i+1])//左边线正常是向右上生长
        {
          LeftColState[i]=true;
        }
        //当左边界突然开始向左生长，出现的异常可能是弯道、十字、环岛、分叉路
        //出现异常就将该点记录为补线开始点,是否需要补线后面再判断
        else
        {
          if(IF_LeftRequire_Add==false)
          {
            IF_LeftRequire_Add=true;
            LeftAdd_State=1;
            Left_Add_StartRow=i+2;
          }
          LeftColState[i]=false;
        }
      }
      else
      {
        if(FindLeftCol[i]==false)
        {
          LeftAddFlag_count++;
          if(IF_LeftRequire_Add==false && LeftAddFlag_count>=10)//没有进入补线状态并且空白行大于阈值
          {
            IF_LeftRequire_Add=true;
            LeftAdd_State=2;
            Left_Add_StartRow=69;
          }
        }
      }
    }
    //右边界状态记录
    if(i<70)
    {
      if(FindRightCol[69]==true)
      {
        if(PreRightCol[i]<=PreRightCol[i+1])
        {
          RightColState[i]=true;
        }
        else
        {
          if(IF_RightRequire_Add==false)
          {
            IF_RightRequire_Add=true;
            RightAdd_State=1;
            Right_Add_StartRow=i+2;
          }
          RightColState[i]=false;
        }
      }
      else
      {
        if(FindRightCol[i]==false)
        {
          RightAddFlag_count++;
          if(IF_RightRequire_Add==false && RightAddFlag_count>=10)
          {
            IF_RightRequire_Add=true;
            RightAdd_State=2;
            Right_Add_StartRow=69;
          }
        }
      }
    }
    
    
/*               补线状态1                */    
    
    //补线状态1：两点法初步补线
    //补线信息线存入数组
    if((LeftAdd_State==1 || RightAdd_State==1))
    {
      if(LeftAdd_State==1)
      {
        if(IF_LeftRequire_Add==true && LeftFirstRepairFin==false)
        {
          LeftRepairSlope=(float)((PreLeftCol[Left_Add_StartRow+4]-PreLeftCol[Left_Add_StartRow+14])/10.0);
          PreLeftCol[i]=PreLeftCol[Left_Add_StartRow]+TrackRepair_TP1(LeftRepairSlope,i,Left_Add_StartRow);
          if(PreLeftCol[i]<=0)
          {
            PreLeftCol[i]=0;
          }
        }
      }
      if(RightAdd_State==1)
      {
        if(IF_RightRequire_Add==true && RightFirstRepairFin==false)
        {
          RightRepairSlope=(float)((int)(PreRightCol[Right_Add_StartRow+4]-PreRightCol[Right_Add_StartRow+14])/10.0);
          PreRightCol[i]=PreRightCol[Right_Add_StartRow]+TrackRepair_TP1(RightRepairSlope,i,Right_Add_StartRow);
          if(PreRightCol[i]>=120)
          {
            PreRightCol[i]=120;
          }
        }
      }
    }
    
    //补线状态1：结束初步补线，搜索补线终止行
    if((LeftAdd_State==1 || RightAdd_State==1))
    {
      if((IF_LeftRequire_Add==true || IF_RightRequire_Add==true) && WrongTarckWidthCount>=10)
      {
        if(LeftAdd_State==1 && RightAdd_State==1)
        {
          if(IF_LeftRequire_Add==true && IF_RightRequire_Add==true && LeftFirstRepairFin==false && RightFirstRepairFin==false)
          {
            if(imagein->Array[i][PreLeftCol[i]].binary==1 || TrackWidth[i]<(WrongTarckWidth-10))
            {
              Left_Add_EndRow=i-9;
              LeftFirstRepairFin=true;
            }
            if(imagein->Array[i][PreRightCol[i]].binary==1 || TrackWidth[i]<(WrongTarckWidth-10))
            {
              Right_Add_EndRow=i-9;
              RightFirstRepairFin=true;
            }
          }
        }
        if(LeftAdd_State==1 && RightAdd_State!=1)
        {
          if(IF_LeftRequire_Add==true && LeftFirstRepairFin==false)
          {
            if(imagein->Array[i][PreLeftCol[i]].binary==1 || TrackWidth[i]<(WrongTarckWidth-10))
            {
              Left_Add_EndRow=i-9;
              LeftFirstRepairFin=true;
            }
          }
        }
        if(LeftAdd_State!=1 && RightAdd_State==1)
        {
          if(IF_RightRequire_Add==true && RightFirstRepairFin==false || TrackWidth[i]<(WrongTarckWidth-10))
          {
            {
              Right_Add_EndRow=i-9;
              RightFirstRepairFin=true;
            }
          }
        }
      }
    }
/*                 END                */    
    
    
    
    
    
/*               补线状态2                */   
    if((LeftAdd_State==2 || RightAdd_State==2))
    {
      if(LeftAdd_State==2 && RightAdd_State==2)
      {
        if(IF_LeftRequire_Add==true && IF_RightRequire_Add==true)
        {
          if(LeftFirstRepairFin==false)
          {
            if(FindLeftCol[i]==true)
            {
              Left_Add_EndRow=i-3;
              LeftFirstRepairFin=true;
            }
          }
          if(RightFirstRepairFin==false)
          {
            if(FindRightCol[i]==true)
            {
              Right_Add_EndRow=i-3;
              RightFirstRepairFin=true;
            }
          }
        }
      }
      if(LeftAdd_State==2 && RightAdd_State!=2)
      {
        if(IF_LeftRequire_Add==true&& LeftFirstRepairFin==false)
        {
          if(FindLeftCol[i]==true)
          {
            Left_Add_EndRow=i-3;
            LeftFirstRepairFin=true;
          }
        }
      }
      if(LeftAdd_State!=2 && RightAdd_State==2)
      {
        if(IF_RightRequire_Add==true && RightFirstRepairFin==false)
        {
          if(FindRightCol[i]==true)
          {
            Right_Add_EndRow=i-3;
            RightFirstRepairFin=true;
          }
        }
      }
    }
      
    
/*                 END                */    
    
    
    
/*               重置搜线起点         */
    
    SerachPoint[i-1]=(int)((PreLeftCol[i]+PreRightCol[i])/2);
    if(LCircleState==Circle_State_2 && CirclePatchEnd<=70)
    {
      SerachPoint[i-1]=PreRightCol[i]-10;
    }
    if(RCircleState==Circle_State_2 && CirclePatchEnd<=70)
    {
      SerachPoint[i-1]=PreLeftCol[i]+10;
    }
    if(GetDiff==true)
    {
      if(DiffState==1)
      {
        SerachPoint[i-1]=PreLeftCol[i]+10;
      }
      if(DiffState==2)
      {
        SerachPoint[i-1]=PreRightCol[i]-10;
      }
    }
    if(InParkstate==1)
    {
        SerachPoint[i-1]=SerachPoint[89];
    }
    //检测终点行
    if(imagein->Array[i-1][SerachPoint[i-1]].binary==1)
    {
      if(InParkstate==1 || TrackState==Track_ApriTag)
      {
        
      }
      else
      {
        ScanEndRow=i;
      }
    }
  }
  
  
  
  /*        环岛判定       */
//  if(CircleType==1 && LCircleState==NoneCircle && RCircleState==NoneCircle && GetDiff==false)
//  {
//    temp_slope1=(float)((float)(PreRightCol[FirstGetBod_Right]-PreRightCol[CircleDectEnd_Left])/(float)(FirstGetBod_Right-CircleDectEnd_Left));
//    temp_slope2=(float)((float)(PreRightCol[CircleDectEnd_Left]-PreRightCol[CircleDectEnd_Left-4])/5.0);
//    if(temp_slope1<=1.0 && temp_slope2<=1.2 && CricleE<=20 && temp_slope1>=0.01 && temp_slope2>=0.01 && RightAdd_State!=1)
////    LEdata=ESensor[3].Read(ESensor[3].Self);
////    REdata=ESensor[2].Read(ESensor[2].Self);
////    if(LEdata>=3000 && REdata<=1000)
//    {
//      LCircleState=Circle_State_1;
//    }
//  }
//  if(CircleType==2 && LCircleState==NoneCircle && RCircleState==NoneCircle && GetDiff==false)
//  {
//    temp_slope1=(float)((float)(PreLeftCol[CircleDectEnd_Right]-PreLeftCol[FirstGetBod_Left])/(float)(FirstGetBod_Left-CircleDectEnd_Right));
//    temp_slope2=(float)((float)(PreLeftCol[CircleDectEnd_Right-4]-PreLeftCol[CircleDectEnd_Right])/5.0);
//    if(temp_slope1<=1.0 && temp_slope2<=1.2 && CricleE<=16 && temp_slope1>=0.01 && temp_slope2>=0.01 && LeftAdd_State!=1)
////    LEdata=ESensor[3].Read(ESensor[3].Self);
////    REdata=ESensor[2].Read(ESensor[2].Self);
////    if(REdata>=3000 && LEdata<=1000 )
//    {
//      RCircleState=Circle_State_1;
//    }
//  }
  //出环岛
  if(LCircleState==Circle_State_5 && FindLeftCol[70]==false)
  {
    for(int i=70;i>0;i--)
    {
      if(FindLeftCol[i]==true && FindRightCol[i]==true)
      {
        CircleOut=i;
        break;
      }
    }
  }
  else if(RCircleState==Circle_State_5 && FindRightCol[70]==false)
  {
    for(int i=70;i>0;i--)
    {
      if(FindLeftCol[i]==true && FindRightCol[i]==true)
      {
        CircleOut=i;
        break;
      }
    }
  }
  
  
  
  /*       分叉路判定      */
  if(DiffRoadJudge==true && LCircleState==NoneCircle && RCircleState==NoneCircle)
  {
    DiffRoadJudge=false;
    for(int i=89;i>=0;i--)
    {
      if(imagein->Array[i][SerachPoint[89]].binary==1 && DiffEndRow==0)
      {
        DiffEndRow=i;
        break;
      }
    }
    //终点行较低且曾存在大量空白行，开始判定是否为三岔路
    if(DiffEndRow>=35)
    {
      //搜索三岔路起点
      for(int i=DiffEndRow;i<90;i++)
      {
        for(int j=0;j<120;j++)
        {
          //左起搜索整行，扫描到空行为止
          if(imagein->Array[i][j].binary==1)//搜索到黑点就跳出次循环
          {
            break;
          }
          DiffSRow=i-1;
        }
        if(DiffSRow!=0)
        {
          break;
        }
      }
      if(DiffSRow!=0)
      {
        //搜索起点纵坐标
        for(int j=0;j<119;j++)
        {
          if(imagein->Array[DiffSRow][j].binary==1)
          {
            DiffSCol_L=j;
            break;
          }
        }
        for(int j=119;j>=0;j--)
        {
          if(imagein->Array[DiffSRow][j].binary==1)
          {
            DiffSCol_R=j;
            break;
          }
        }
        //搜索终点纵坐标
        for(int j=0;j<119;j++)
        {
          if(imagein->Array[DiffSRow-4][j].binary==1)
          {
            DiffECol_L=j;
            break;
          }
        }
        for(int j=119;j>=0;j--)
        {
          if(imagein->Array[DiffSRow-4][j].binary==1)
          {
            DiffECol_R=j;
            break;
          }
        }
        //计算斜率
        DiffLslope=(float)((float)(DiffECol_L-DiffSCol_L)/5.0);
        DiffRslope=(float)((float)(DiffECol_R-DiffSCol_R)/5.0);
        if(DiffCount==0)
        {
          if(DiffLslope<=-1.8 && DiffRslope>=1.8)
          {
            DiffFlag=true;
            if(DIffF==false)
            {
              DIffF=true;
            }
            if(Diffstate==NoneDiff)
            {
              Diffstate=InDiff;
            }
          }
        }
        else if(DiffCount==1)
        {
          if(DiffLslope<=-0.8 && DiffRslope>=0.8)
          {
            DiffFlag=true;
            if(DIffF==false)
            {
              DIffF=true;
            }
            if(Diffstate==NoneDiff)
            {
              Diffstate=InDiff;
            }
          }
        }

      }
    }
  }
  
  
  
  //分岔路状态机

    switch(Diffstate)
    {
      case NoneDiff:
      {
        diff=0;
      }
      break;
      case InDiff:
      {
        diff=1;
        GetDiff=true;
        if(FindRightCol[65]==false && DiffState==1)
        {
          Diffstate=InDiff_patch;
        }
        if(FindLeftCol[65]==false && DiffState==2)
        {
          Diffstate=InDiff_patch;
        }
        if(DiffState==0)
        {
          Car_Go=false;
          Diffstate=InDiff_Stop;
        }
      }
      break;
      case InDiff_patch:
      {
        diff=2;
        if(DiffState==1)//左转
        {
          if(FindRightCol[70]==true)
          {
            Diffstate=NoneDiff;
          }
        }
        if(DiffState==2)//右转
        {
          if(FindLeftCol[70]==true)
          {
            Diffstate=NoneDiff;
          }
        }
      }
      break;
      case InDiff_Stop:
      {
        Console.WriteLine("%d",1);
        ttttcount++;
        NumJudge=UART_GetChar(LPUART7);
        if(ttttcount<=200)//识别时间小于6秒判定，否则直接走
        {
          if(NumJudge==65)
          {
            Car_Go=true;
            Console.WriteLine("%d",5);
            DiffState=2;
            Diffstate=OutDiff_Go;
          }
          else if(NumJudge==66)
          {
            Car_Go=true;
            Console.WriteLine("%d",5);
            DiffState=1;
            Diffstate=OutDiff_Go;
          }
        }
        else
        {
          ttttcount=0;
          Car_Go=true;
          Console.WriteLine("%d",5);
          DiffState=1;
          Diffstate=OutDiff_Go;
        }
//        if(tcount>=100)
//        {
//          tcount=0;
//          Car_Go=true;
//          Diffstate=OutDiff_Go;
//        }
      }
      break;
      case OutDiff_Go:
      {
        tcount++;
        if(tcount>=20)
        {
          tcount=0;
          DiffCount++;
          GetDiff=false;
          Diffstate=NoneDiff;
        }
      }
      break;
   }
  if(GetDiff==true && Diffstate!=InDiff_Stop && Diffstate!=OutDiff_Go)
  {
    ttcount++;
    if(ttcount>=30)
    {
      ttcount=0;
      GetDiff=false;
      DiffCount++;
    }
  }
    if(DiffState==1 && DiffCount==2)
    {
      DiffCount=0;
      DiffState=2;
    }
    if(DiffState==2 && DiffCount==2)
    {
      DiffCount=0;
      DiffState=1;
    }
  
  //处理终点线
    if(InParkstate==0)
    {
      ApriTagDect(&ImageBinary,52);//搜索第52行，检测跳变沿
      if(InParkflag==true)
      {
        InParkstate=1;
        InParkcount++;
        if(InParkcount>=2)
        {
          InPark=true;
          InParkstate=2;
        }
      }
    }
    else if(InParkstate==1)
    {
      //搜索顶
      for(int i=30;i<=89;i++)
      {
        if(DectWrong2(&ImageBinary,i)==1)
        {
          UPPatchRow=i;
          break;
        }
      }
      for(int i=UPPatchRow-10;i>=0;i--)
      {
        if(FindLeftCol[i]==true && FindRightCol[i]==true)
        {
          EndPatchRow=i-3;
          break;
        }
//        EndSearchRow=(int)((leftCol+rightCol)/2);
      }
      //搜近处左右边点
      for(int i=0;i<119;i++)
      {
        if((imagein->Array[EndPatchRow][i].binary==1) && (imagein->Array[EndPatchRow][i+1].binary==0))
        {
          WrongStartL=i;
          break;
        }
        if((imagein->Array[EndPatchRow][i].binary==0) && (imagein->Array[EndPatchRow][i+1].binary==1))
        {
          WrongStartR=i;
          break;
        }
      }
      for(int i=0;i<119;i++)
      {
        if((imagein->Array[EndPatchRow-9][i].binary==1) && (imagein->Array[EndPatchRow-9][i+1].binary==0))
        {
          WrongEndL=i;
          break;
        }
        if((imagein->Array[EndPatchRow][i].binary==0) && (imagein->Array[EndPatchRow][i+1].binary==1))
        {
          WrongEndR=i;
          break;
        }
      }
      
      
      if(EncoderCount>=2500)
      {
        InParkstate=0;
        EncoderCount=0;
        InParkflag=false;
      }
    }
    else if(InParkstate==2)
    {
      if(EncoderCount>=1000)
      {
        EncoderCount=0;
        InParkstate=3;   
        Car_Stop=true;
      }
    }
  else if(InParkstate==3)
  {
    if(EncoderCount>=2000)
    {
      EncoderCount=0;
      
//      InParkstate=4;
      Car_Go=false;
    }
  }
  
  if(InParkstate==0 && Rampflag==false)
  {
    switch(TrackState)
    {
      //正常寻迹模式下检测异常
      case Track_OK:
      {
        //搜索第48行，预判赛道是否有异常
        DectRow(&ImageBinary,48);
        if(DectTrackRate<=0.5 && DectJumpRow>=6 && DectShapeRate>=0.4)
        {
          for(int i=89;i>=50;i--)
          {
            if(DectWrong1(&ImageBinary,i)==1)
            {
              DownPathRow=i;
              break;
            }
          }
          for(int i=50;i<=89;i++)
          {
            if(DectWrong2(&ImageBinary,i)==1)
            {
              UPPatchRow=i;
              break;
            }
          }
          UDnum=DownPathRow-UPPatchRow;
          if(UDnum>1)
          {
            TrackState=Track_ApriTag;
          }
        }
      }
      break;
      case Track_ApriTag:
      {
        Car_Go=false;
        for(int i=20;i<=89;i++)
        {
          if(DectWrong2(&ImageBinary,i)==1)
          {
            UPPatchRow=i;
            break;
          }
        }
        for(int i=UPPatchRow-10;i>=0;i--)
        {
          if(FindLeftCol[i]==true && FindRightCol[i]==true)
          {
            EndPatchRow=i;
            break;
          }
          EndSearchRow=(int)((leftCol+rightCol)/2);
        }
        if(AprilTagJudge==1)
        {
          Console.WriteLine("%d",2);
          ApriTagNum=UART_GetChar(LPUART7);
          if(ApriTagNum==65)
          {
            Car_Go=false;
            Console.WriteLine("%d",5);
            AprilDir=2;
            Servo2.SetPwmValue(Servo2.Self,220);
            TrackState=Track_ApriTag_Stop;
          }
          else if(ApriTagNum==66)
          {
            Car_Go=false;
            Console.WriteLine("%d",5);
            AprilDir=1;
            Servo2.SetPwmValue(Servo2.Self,1120);
            TrackState=Track_ApriTag_Stop;
          }
        }
        else
        {
          tttcount++;
          if(tttcount>=140)
          {
            tttcount=0;
            TrackState=Track_OK;
            Car_Go=true;
          }
        }
      }
      break;
      case Track_ApriTag_Stop:
      {
        
      }
      break;
    }
  }
  

  
  /*         END         */
  
  //第二次补线，判断元素修补边界
  
  
  
  
  /*           补线状态1                 */
  if(LeftAdd_State==1 || RightAdd_State==1)
  {
    if(LeftAdd_State==1 && RightAdd_State==1 && LeftFirstRepairFin==false && RightFirstRepairFin==false)
    {
      for(int i=Left_Add_StartRow;i>0;i--)
      {
        PreLeftCol[i]=0;
        for(int j=SerachPoint[i];j>=0;j--)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreLeftCol[i]=j;
            FindLeftCol[i]=true;
            break;
          }
        } 
      }
      for(int i=Right_Add_StartRow;i>0;i--)
      {
        PreRightCol[i]=120;
        for(int j=SerachPoint[i];j<=120;j++)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreRightCol[i]=j;
            FindRightCol[i]=true;
            break;
          }
        }
      }
    }
    if(LeftAdd_State==1 && RightAdd_State!=1 && (LeftFirstRepairFin==false || Left_Add_EndRow<10))
    {
      for(int i=Left_Add_StartRow;i>0;i--)
      {
        PreLeftCol[i]=0;
        for(int j=SerachPoint[i];j>=0;j--)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreLeftCol[i]=j;
            FindLeftCol[i]=true;
            break;
          }
        }
        
      }
    }
    if(LeftAdd_State!=1 && RightAdd_State==1 && (RightFirstRepairFin==false || Right_Add_EndRow<10))
    {
      for(int i=Right_Add_StartRow;i>0;i--)
      {
        PreRightCol[i]=120;
        for(int j=SerachPoint[i];j<=120;j++)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreRightCol[i]=j;
            FindRightCol[i]=true;
            break;
          }
        }
      }
    }
  }
  
  
/*           补线状态2              */
  if((LeftAdd_State==2 || RightAdd_State==2)&&DIffF==false)
  {
    if(LeftAdd_State==2 && RightAdd_State==2 && LeftFirstRepairFin==true && RightFirstRepairFin==true && Left_Add_EndRow>30 && Right_Add_EndRow>30)//补两边
    {
      LeftRepairSlope=(float)((float)(PreLeftCol[Left_Add_EndRow-9]-PreLeftCol[Left_Add_EndRow])/10.0);
      RightRepairSlope=(float)((float)(PreRightCol[Right_Add_EndRow-9]-PreRightCol[Right_Add_EndRow])/10.0);
      for(int i=Left_Add_EndRow;i<90;i++)
      {
        PreLeftCol[i]=PreLeftCol[Left_Add_EndRow]-TrackRepair_TP2(LeftRepairSlope,i,Left_Add_EndRow);
        if(PreLeftCol[i]<=0)
        {
          PreLeftCol[i]=0;
        }
      }
      for(int i=Right_Add_EndRow;i<90;i++)
      {
        PreRightCol[i]=PreRightCol[Right_Add_EndRow]-TrackRepair_TP2(RightRepairSlope,i,Right_Add_EndRow);
        if(PreRightCol[i]>=120)
        {
          PreRightCol[i]=120;
        }
      }
    }
    if(LeftAdd_State==2 && RightAdd_State!=2 && LeftFirstRepairFin==true && Left_Add_EndRow>30)//补左
    {
      LeftRepairSlope=(float)((float)(PreLeftCol[Left_Add_EndRow-5]-PreLeftCol[Left_Add_EndRow])/6.0);
      for(int i=Left_Add_EndRow;i<90;i++)
      {
        PreLeftCol[i]=PreLeftCol[Left_Add_EndRow]-TrackRepair_TP2(LeftRepairSlope,i,Left_Add_EndRow);
        if(PreLeftCol[i]<=0)
        {
          PreLeftCol[i]=0;
        }
      }
    }
    if(LeftAdd_State!=2 && RightAdd_State==2 && RightFirstRepairFin==true && Right_Add_EndRow>30)//补右
    {
      RightRepairSlope=(float)((float)(PreRightCol[Right_Add_EndRow-5]-PreRightCol[Right_Add_EndRow])/6.0);
      for(int i=Right_Add_EndRow;i<90;i++)
      {
        PreRightCol[i]=PreRightCol[Right_Add_EndRow]-TrackRepair_TP2(RightRepairSlope,i,Right_Add_EndRow);
        if(PreRightCol[i]>=120)
        {
          PreRightCol[i]=120;
        }
      }
    }
  }
  
/*            环岛补线          */
  if(LCircleState != NoneCircle && RCircleState == NoneCircle)//补左环
  {
    if(LCircleState==Circle_State_2)
    {
      CircleSlope=(float)((float)((float)PreLeftCol[CirclePatchEnd]-(float)PreRightCol[CirclePatchStart])/(float)((float)CirclePatchEnd-(float)CirclePatchStart));
      for(int i=CirclePatchEnd;i<90;i++)
      {
        PreLeftCol[i]=0;
        PreRightCol[i]=PreLeftCol[CirclePatchEnd-3]+TrackRepair_TP2(CircleSlope,i,CirclePatchEnd);
      }
      for(int i=CirclePatchEnd;i>=0;i--)
      {
        if(imagein->Array[i][0].binary==1)
        {
          ScanEndRow=i;
          break;
        }
      }
      for(int i=CirclePatchEnd;i>ScanEndRow;i--)
      {
        PreLeftCol[i]=0;
        for(int j=0;j<120;j++)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreRightCol[i]=j;
            break;
          }
        }
      }
    }
    if(LCircleState==Circle_State_4)
    {
      if(outcircletype==1)
      {
        RightRepairSlope=(float)((int)(PreRightCol[CircleOutRow+4]-PreRightCol[CircleOutRow+14])/10.0);
        for(int i=CircleOutRow;i>=0;i--)
        {
          PreRightCol[i]=PreRightCol[CircleOutRow]+TrackRepair_TP1(RightRepairSlope,i,CircleOutRow);
          if(PreRightCol[i]>=120)
          {
            PreRightCol[i]=120;
          }
        }
      }
      if(outcircletype==2)
      {
        RightRepairSlope=-1.5;
        for(int i=CircleOutRow;i>=0;i--)
        {
          PreRightCol[i]=PreRightCol[CircleOutRow]+TrackRepair_TP1(RightRepairSlope,i,CircleOutRow);
          if(PreRightCol[i]>=120)
          {
            PreRightCol[i]=120;
          }
        }
      }
      for(int i=89;i>=0;i--)
      {
        temp_circle[i]=PreRightCol[i];
      }
//      if((LEdata>=52 && REdata>=52) || FindLeftCol[60]==true)
//      {
//        LCircleState=Circle_State_5;
//      }
    }
    if(LCircleState==Circle_State_5)
    {
      CircleOutSlope=(float)((float)(PreLeftCol[CircleOut-5]-PreLeftCol[CircleOut])/6.0);
      for(int i=CircleOut;i<90;i++)
      {
        PreLeftCol[i]=PreLeftCol[CircleOut]-TrackRepair_TP2(CircleOutSlope,i,CircleOut);
        if(PreLeftCol[i]<=0)
        {
          PreLeftCol[i]=0;
        }
      }
    }
    
  }
  
  
  if(RCircleState != NoneCircle && LCircleState == NoneCircle)//补右环
  {
    if(RCircleState==Circle_State_2)
    {
      CircleSlope=(float)((float)((float)PreRightCol[CirclePatchEnd]-(float)PreLeftCol[CirclePatchStart])/(float)((float)CirclePatchEnd-(float)CirclePatchStart));
      for(int i=CirclePatchEnd;i<90;i++)
      {
        PreRightCol[i]=119;
        PreLeftCol[i]=PreRightCol[CirclePatchEnd-3]+TrackRepair_TP2(CircleSlope,i,CirclePatchEnd);
      }
      for(int i=CirclePatchEnd;i>=0;i--)
      {
        if(imagein->Array[i][119].binary==1)
        {
          ScanEndRow=i;
          break;
        }
      }
      for(int i=CirclePatchEnd;i>ScanEndRow;i--)
      {
        PreRightCol[i]=119;
        for(int j=119;j>=0;j--)
        {
          if(imagein->Array[i][j].binary==1)
          {
            PreLeftCol[i]=j;
            break;
          }
        }
      }
    }
    if(RCircleState==Circle_State_4)
    {
      if(outcircletype==1)
      {
        LeftRepairSlope=(float)((int)(PreLeftCol[CircleOutRow+4]-PreLeftCol[CircleOutRow+14])/10.0);
        for(int i=CircleOutRow;i>=0;i--)
        {
          PreLeftCol[i]=PreLeftCol[CircleOutRow]+TrackRepair_TP1(LeftRepairSlope,i,CircleOutRow);
          if(PreLeftCol[i]<=0)
          {
            PreLeftCol[i]=0;
          }
        }
      }
      if(outcircletype==2)
      {
        LeftRepairSlope=1.5;
        for(int i=CircleOutRow;i>=0;i--)
        {
          PreLeftCol[i]=PreLeftCol[CircleOutRow]+TrackRepair_TP1(LeftRepairSlope,i,CircleOutRow);
          if(PreLeftCol[i]<=0)
          {
            PreLeftCol[i]=0;
          }
        }
      }
      for(int i=89;i>=0;i--)
      {
        temp_circle[i]=PreLeftCol[i];
      }
    }
    if(RCircleState==Circle_State_5)
    {
      CircleOutSlope=(float)((float)(PreLeftCol[CircleOut-5]-PreLeftCol[CircleOut])/6.0);
      for(int i=CircleOut;i<90;i++)
      {
        PreLeftCol[i]=PreLeftCol[CircleOut]-TrackRepair_TP2(CircleOutSlope,i,CircleOut);
        if(PreLeftCol[i]<=0)
        {
          PreLeftCol[i]=0;
        }
      }
    }
  }
  
  
  //分岔路补线
  if(Diffstate==InDiff_patch)
  {
      if(DiffState==1)
      {
        RightRepairSlope=-1.5;
        for(int i=DiffSRow;i<90;i++)
        {
           PreRightCol[i]=DiffSCol_L-TrackRepair_TP2(RightRepairSlope,i,DiffSRow);
           if(PreRightCol[i]>120)
           {
             PreRightCol[i]=120;
           }
        }
      }
      if(DiffState==2)
      {
        LeftRepairSlope=1.5;
        for(int i=DiffSRow;i<90;i++)
        {
           PreLeftCol[i]=DiffSCol_R-TrackRepair_TP2(LeftRepairSlope,i,DiffSRow);
           {
             if(PreLeftCol[i]<0)
             {
               PreLeftCol[i]=0;
             }
           }
        }
      }
  }
  
  //终点和ApriTag补线
  if(InParkstate==1 || TrackState==Track_ApriTag)
  {
      EndPatchlslope=(float)((float)(PreLeftCol[EndPatchRow-9]-PreLeftCol[EndPatchRow])/10.0);
      EndPatchrslope=(float)((float)(PreRightCol[EndPatchRow-9]-PreRightCol[EndPatchRow])/10.0);
      for(int i=EndPatchRow;i<90;i++)
      {
        PreLeftCol[i]=PreLeftCol[EndPatchRow]-TrackRepair_TP2(EndPatchlslope,i,EndPatchRow);
        if(PreLeftCol[i]<=0)
        {
          PreLeftCol[i]=0;
        }
        else if(PreLeftCol[i]>=60)
        {
          PreLeftCol[i]=60;
        }
      }
      for(int i=EndPatchRow;i<90;i++)
      {
        PreRightCol[i]=PreRightCol[EndPatchRow]-TrackRepair_TP2(EndPatchrslope,i,EndPatchRow);
        if(PreRightCol[i]>=120)
        {
          PreRightCol[i]=120;
        }
        else if(PreRightCol[i]<=60)
        {
          PreRightCol[i]=60;
        }
      }
  }
  
  
  
//  //判定ApriTag
//  if(AprilTagflag==true && Aprilget==0)
//  {
//    ApriTagGet=true;
//    Car_Go=false;
//  }
  

  
  
/*            拟合中线          */
  for(int i=0;i<90;i++)
  {
    if(ScanEndRow>=89)
    {
      PreMidCol[i]=LastEndCol;
      MidError[i]=60-PreMidCol[i];
    }
    else
    {
      PreMidCol[i]=(int)((PreLeftCol[i]+PreRightCol[i])/2);
//      if((TrackState==Track_End || TrackState==Track_ApriTag) && i>=UPPatchRow-2 && i<=DownPathRow+2)
//      {
//        if(DownPathRow<88)
//        {
//          PreMidCol[i]=PreMidCol[DownPathRow+2];
//        }
//        else
//        {
//          PreMidCol[i]=PreMidCol[89];
//        }
//      }
      if(i<=ScanEndRow)
      {
        PreMidCol[i]=PreMidCol[ScanEndRow+1];
      }
      MidError[i]=60-PreMidCol[i];
    }
  }
  
  LastEndCol=PreMidCol[89];
  
  
}


/*
功能：图像第一行特殊处理
作者：戴进
日期：2021.6.3
参数：
imagein:大津法二值化图像
返回值：搜线中点
*/
uint8_t FirstRowHandle(image_t *imagein)
{
  uint8_t LeftCol=0;//左边界
  uint8_t RightCol=120;//右边界
  uint8_t res;
  bool FindLeft=false;
  bool FindRight=false;
  if(FindLeft==false)
  {
    for(int j=1;j<=119;j++)
    {
      if(imagein->Array[89][j].binary==0)
      {
        LeftCol=j;
        FindLeft=true;
        break;
      }
    }
  }
  if(FindRight==false)
  {
    for(int j=119;j>=1;j--)
    {
      if(imagein->Array[89][j].binary==0)
      {
        RightCol=j;
        FindRight=true;
        break;
      }
    }
  }
  res=(LeftCol+RightCol)/2;
  if(res>10 && res<110)
  {
    return (int)res;
  }
  else
  {
    return 60;
  }
}

/*
功能：两点法拟合补线(从下往上补)
作者：戴进
日期：2021.6.3
参数：
Slope:补线斜率
i:补线点
StartRow:补线基点
*/

int TrackRepair_TP1(float Slope,int i,int StartRow)
{
  int res;
  res=(int)(Slope*(StartRow-i));
  return res;
}

/*
功能：两点法拟合补线(从上往下补)
作者：戴进
日期：2021.6.3
参数：
Slope:补线斜率
i:补线点
StartRow:补线基点
*/
int TrackRepair_TP2(float Slope,int i,int StartRow)
{
  int res;
  res=(int)(Slope*(i-StartRow));
  return res;
}


/*
功能：计算中线误差
作者：戴进
日期：2021.6.4
*/
int ImageError()
{
  long Error1=0;
  long Error2=0;
  long RealError;
  //搜线搜索到的终点行低于计算误差的终点行，即存在部分误差空白
  //使用搜线终点行的误差替代
  for(int i=CalEndRow;i<CalEndRow+11;i++)
  {
    Error1 +=  MidError[i]*3 ;
  }
  for(int i=CalEndRow+12;i<CalEndRow+19;i++)
  {
    Error2 +=  MidError[i]*2;
  }
  RealError=(int)((float)(Error1+Error2)/14.0);
//  if(Diffstate!=NoneDiff)
//  {
//    RealError=RealError*3;
//  }
  return RealError;
}


/*
功能：电感数据处理
作者：戴进
日期：2021.6.5
*/
//void EsenerProcess()
//{
////  Ecount++;
//  temp_ldata=ESensor[3].Read(ESensor[3].Self);
//  temp_rdata=ESensor[2].Read(ESensor[2].Self);
//  LEdata=(int)(temp_ldata);
//  REdata=(int)(temp_rdata);
////  if(LEdata>=95)
////  {
////    LEdata=0;
////  }
////  if(REdata>=95)
////  {
////    REdata=0;
////  }
//  temp_ldata=0;
//  temp_rdata=0;
////  if(LEdata<=7 && REdata<=7)
////  {
////    Car_Go=false;
////  }
//}


/*
功能：识别ApriTag码
作者：戴进
日期：2021.7.4
*/

void ApriTagDect(image_t *imagein,uint8_t JudgeRow)
{
  int flag=0;//0:搜索上升沿；1：搜索下降沿
  bool GetLeft=false;
  bool GetRight=false;
  bool JudgeL=false;
  bool JudgeR=false;
  BlackPoint=0;
  Blackrate=0;
  //确定赛道范围
  for(int i=0;i<120;i++)
  {
    if(imagein->Array[89][i].binary==0)
    {
      StartLeftCol=i;
      break;
    }
  }
  for(int i=119;i>=0;i--)
  {
    if(imagein->Array[89][i].binary==0)
    {
      StartRightCol=i;
      break;
    }
  }
  //搜索左右边界
  if(imagein->Array[JudgeRow][0].binary==0)
  {
    LeftColPos=0;
    GetLeft=true;
  }
  if(imagein->Array[JudgeRow][119].binary==0)
  {
    RightColPos=119;
    GetRight=true;
  }
  if(GetLeft==false)
  {
    for(int i=1;i<120;i++)
    {
      if(imagein->Array[JudgeRow][i].binary==1 && imagein->Array[JudgeRow][i+1].binary==0)
      {
        LeftColPos=i;
        GetLeft=true;
        JudgeL=true;
        break;
      }
    }
  }
  if(GetRight==false)
  {
    for(int i=119;i>0;i--)
    {
      if(imagein->Array[JudgeRow][i].binary==0 && imagein->Array[JudgeRow][i+1].binary==1)
      {
        RightColPos=i;
        GetRight=true;
        JudgeR=true;
        break;
      }
    }
  }
  AprilWidth=RightColPos-LeftColPos;
  //搜索左右开始出现黑点的起始点异常点
  for(int i=LeftColPos+1;i<RightColPos;i++)
  {
    if(imagein->Array[JudgeRow][i].binary==1)
    {
      AprilStart=i;
      break;
    }
  }
  for(int i=RightColPos-1;i>LeftColPos;i-- )
  {
    if(imagein->Array[JudgeRow][i].binary==1)
    {
      AprilEnd=i;
      break;
    }
  }
  if(AprilStart!=0 && AprilEnd!=0)
  {
    BlackWidth=AprilEnd-AprilStart;
  }
  
  //在左右边界中搜索跳变沿
  for(int i=LeftColPos;i<RightColPos;i++)
  {
    if(imagein->Array[JudgeRow][i].binary==1)
    {
      BlackPoint++;
    }
    if(flag==0)//搜索上升沿
    {
      if(imagein->Array[JudgeRow][i].binary==0 && imagein->Array[JudgeRow][i+1].binary==1)
      {
        flag=1;
      }
    }
    else //搜索下降沿，搜素到一上升沿一下降沿后判定为一个完整的黑块
    {
      if(imagein->Array[JudgeRow][i].binary==1 && imagein->Array[JudgeRow][i+1].binary==0)
      {
        flag=0;
        Blackrate++;
      }
    }
  }
  if((JudgeL==true || JudgeR==true )&& Blackrate>=6 && BlackPoint>=20)
  {
    InParkflag=true;
  }
//  else if((JudgeL==true || JudgeR==true )&& Blackrate>=3 && Blackrate<=5)
//  {
//    if(BlackPoint>=5 && BlackPoint<=18 && AprilWidth<=75)
//      AprilTagflag=true;
//  }
}

/*
功能：搜索整行，记录数据
作者：戴进
日期：2021.7.12
！！！！！！！！！爆肝！！！！！！！！！！
*/
void DectRow(image_t *imagein,uint8_t SearchRow)
{
  
/*          搜赛道             */
  //寻找左边界：DectLeftCol
  for(int i=0;i<120;i++)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      DectLeftCol=i;
      break;
    }
  }
  //搜索右边界：DectRightCol
  for(int i=119;i>=0;i--)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      DectRightCol=i;
      break;
    }
  }
  //计算赛道宽度
  DectTrackWidth=DectRightCol-DectLeftCol;
/*          END         */
  
  
    
/*          搜异常宽度       */
  
  //搜左异常起始点:DectLSP
  for(int i=DectLeftCol;i<=DectRightCol;i++)
  {
    if(imagein->Array[SearchRow][i].binary==1)
    {
      DectLSP=i;
      break;
    }
  }
  //搜左异常起始点:DectRSP
  for(int i=DectRightCol;i>=DectLeftCol;i--)
  {
    if(imagein->Array[SearchRow][i].binary==1)
    {
      DectRSP=i;
      break;
    }
  }
  //计算异常宽度
  DectShapeWidth=DectRSP-DectLSP;
  //计算异常中点
  DectTrackMid=(int)((DectRSP+DectLSP)/2);
  //搜索异常上边界
  for(int i=SearchRow-15;i<=SearchRow+15;i++)
  {
    if(imagein->Array[i][DectTrackMid].binary==1)
    {
      DectUSP=i;
      break;
    }
  }
  //搜索下边界
  for(int i=SearchRow+15;i>=SearchRow-15;i--)
  {
    if(imagein->Array[i][DectTrackMid].binary==1)
    {
      DectDSP=i;
      break;
    }
  }
  //计算长宽比
  DectShapeRate=((float)(float)(DectDSP-DectUSP)/(float)DectShapeWidth);
  
  //计算异常宽和赛道宽比例
  DectTrackRate=(float)((float)DectShapeWidth/(float)DectTrackWidth);
  
  
  //搜索跳变沿和黑点：DectJumpRoDectBlackPointw，DectBlackPoint
  for(int i=DectLeftCol;i<DectRightCol;i++)
  {
    if(imagein->Array[SearchRow][i].binary==1)
    {
      DectBlackPoint++;
    }
    if((imagein->Array[SearchRow][i].binary==1)&&(imagein->Array[SearchRow][i+1].binary==0))
    {
      DectJumpRow++;
    }
    if((imagein->Array[SearchRow][i].binary==0)&&(imagein->Array[SearchRow][i+1].binary==1))
    {
      DectJumpRow++;
    }
  }
  
  
/*          END         */
  
}


/*
功能：搜索整行，返回跳变沿数量
作者：戴进
日期：2021.7.13
！！！！！！！！！爆肝！！！！！！！！！！
*/
int DectWrong1(image_t *imagein,uint8_t SearchRow)
{
//  uint8_t LeftCol;//左边界
//  uint8_t RightCol;//右边界
//  int JumpRow;//跳变沿
  
  //搜索左右边界
  for(int i=0;i<120;i++)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      LeftCol=i;
      break;
    }
  }
  for(int i=119;i>=0;i--)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      RightCol=i;
      break;
    }
  }
  
  //搜索赛道中的跳变沿
  for(int i=LeftCol;i<RightCol;i++)
  {
    if((imagein->Array[SearchRow][i].binary==1)&&(imagein->Array[SearchRow][i+1].binary==0))
    {
      JumpRow++;
    }
    if((imagein->Array[SearchRow][i].binary==1)&&(imagein->Array[SearchRow][i+1].binary==0))
    {
      JumpRow++;
    }
  }
  if(JumpRow>=10)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int DectWrong2(image_t *imagein,uint8_t SearchRow)
{
//  uint8_t LeftCol;//左边界
//  uint8_t RightCol;//右边界
//  int JumpRow;//跳变沿
  
  //搜索左右边界
  for(int i=0;i<120;i++)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      leftCol=i;
      break;
    }
  }
  for(int i=119;i>=0;i--)
  {
    if(imagein->Array[SearchRow][i].binary==0)
    {
      rightCol=i;
      break;
    }
  }
  
  //搜索赛道中的跳变沿
  for(int i=leftCol;i<rightCol;i++)
  {
    if((imagein->Array[SearchRow][i].binary==1)&&(imagein->Array[SearchRow][i+1].binary==0))
    {
      jumpRow++;
    }
    if((imagein->Array[SearchRow][i].binary==1)&&(imagein->Array[SearchRow][i+1].binary==0))
    {
      jumpRow++;
    }
  }
  if(jumpRow>=10)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}








