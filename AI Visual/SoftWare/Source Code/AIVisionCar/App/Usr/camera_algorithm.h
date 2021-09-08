#ifndef __CAMERA_ALGORITHM_H_
#define __CAMERA_ALGORITHM_H_

#include "sys.h"

typedef enum
{
  NoneCircle,//没识别到环岛
  Circle_State_1,//识别到环岛，黑环存在
  Circle_State_2,//黑环消失，开始打角补线
  Circle_State_3,//进入环中
  Circle_State_4,//出环，环岛边界开始丢线
  Circle_State_5,//一边线全丢光
  Circle_State_6,//环岛结束
}circle_t;

typedef enum
{
  NoneDiff,//没有分岔路
  InDiff,//分叉路中
  InDiff_patch,//分岔路中补线
  InDiff_Stop,//停车状态
  OutDiff_Go,//停车出分岔路
}diff_t;//分岔路状态

typedef enum
{
  Track_OK,//赛道正常
  Track_Prejudge,//预判赛道异常
  Track_ApriTag,//识别到ApriTag码
  Track_End,//识别到终点线
  Track_InPark_Stright,//进车库直行
  Track_InPark_Turn,//进车库打角
  Track_InPark_Stop,//入库停车
  Track_ApriTag_Stop,
  Track_ApriTag_Back,//倒车
  Track_ApriTag_Dect,//检测
}track_t;




void My_ImageProcess(image_t *imagein);
uint8_t FirstRowHandle(image_t *imagein);
int TrackRepair_TP1(float Slope,int i,int StartRow);
int TrackRepair_TP2(float Slope,int i,int StartRow);
int ImageError();
void EsenerProcess();
void ApriTagDect(image_t *imagein,uint8_t JudgeRow);
void DectRow(image_t *imagein,uint8_t SearchRow);
int DectWrong1(image_t *imagein,uint8_t SearchRow);
int DectWrong2(image_t *imagein,uint8_t SearchRow);
//void TrackWrongDect(image_t *imagein);

extern uint8_t LeftSeedRow;
extern uint8_t LeftSeedCol;
extern int PreLeftCol[90];
extern int PreRightCol[90];
extern uint8_t PreMidCol[90];
extern uint8_t TrackWidth[90];//赛道宽度
extern uint8_t SerachPoint[90];//搜线起始点
extern int MidError[90];
//extern uint16_t LEdata;
//extern uint16_t REdata;
extern uint8_t ScanEndRow;
extern uint8_t Left_Add_EndRow;//左边界补线开始行
extern uint8_t Right_Add_EndRow;//右边界补线开始行
extern uint8_t Left_Add_StartRow;//左边界补线开始行
extern uint8_t Right_Add_StartRow;//右边界补线开始行
extern uint8_t WrongTarckWidthCount;
extern uint8_t WrongTarckWidth;
extern float LeftRepairSlope;//左边界补线斜率
extern float RightRepairSlope;//右边界补线斜率
extern uint8_t CalEndRow;
extern float CircleLeftSlope;//斜率
extern float CircleRightSlope;//斜率float CircleLeftSlope;//斜率uint8_t CircleDectStart_Left;//搜环岛开始点
extern uint8_t CircleDectStart_Left;//搜环岛开始点
extern uint8_t CircleDectStart_Right;//搜环岛开始点
extern uint8_t CircleDectMid_Left;//搜环岛中间点
extern uint8_t CircleDectMid_Right;//搜环岛中间点
extern uint8_t CircleDectEnd_Left;//搜环岛结束点
extern uint8_t CircleDectEnd_Right;//搜环岛结束点
extern uint8_t CircleType;
extern int temp1;
extern float temp_slope1;
extern float temp_slope2;
extern uint8_t CirclePatchStart;//环内补线开始点
extern uint8_t CirclePatchEnd;//环内补线结束点
extern float CircleSlope;//环岛补线斜率
extern uint8_t outcircletype;
extern uint8_t circlestate;
extern circle_t LCircleState;//环岛状态，初始化为无环岛
extern circle_t RCircleState;
extern diff_t Diffstate;
extern uint8_t DiffState;//1:左转；2：右转
extern uint8_t diff;
extern uint8_t DiffEndRow;
extern uint8_t DiffEnd_L;
extern uint8_t DiffEnd_R;
extern uint8_t DiffSRow;
extern uint8_t DiffCount;
extern uint8_t DiffSCol_L;//分岔路检测起点纵坐标
extern uint8_t DiffSCol_R;//分岔路检测起点纵坐标
extern uint8_t DiffECol_L;//分岔路检测左终点纵坐标
extern uint8_t DiffECol_R;//分岔路检测右终点纵坐标
extern float DiffLslope;
extern float DiffRslope;
extern bool DiffFlag;
extern bool GetDiff;
extern bool DIffF;//分岔路总标志
extern uint8_t ApriTagState;
extern uint8_t Blackrate;
extern uint8_t BlackPoint;//黑点计数
extern bool ApriTagGet;
extern float lslope;
extern float rslope;
extern uint8_t LeftColPos;//左边界位置
extern uint8_t RightColPos;//右边界位置
extern uint8_t CricleE;//环岛终点
extern float cirtpslope1;
extern float cirtpslope2;
extern uint8_t Circletmpnum;
extern int EncoderCount;
extern uint8_t BlackWidth;//中间黑块宽度
extern uint8_t AprilWidth;//识别赛道宽度
extern uint8_t AprilStart;//黑色起始点
extern uint8_t AprilEnd;//黑色起始点
extern uint8_t StartLeftCol;
extern uint8_t StartRightCol;
extern uint8_t TrackWrongStart;//赛道异常开始行
extern uint8_t TrackWrongEnd;//赛道异常结束行
extern uint8_t TrackEnd;//检查终止行
extern bool InParkflag;
extern uint8_t InParkstate;
extern uint8_t InParkcount;
extern bool InPark;
extern uint8_t InParktype;//1：左库；2：右库
extern uint8_t OutDiffway;
  
extern track_t TrackState;
extern uint8_t DectLeftCol;//搜索本行的左边界
extern uint8_t DectRightCol;//搜索本行的右边界
extern uint8_t DectLSP;//搜索本行的异常开始左边界
extern uint8_t DectRSP;//搜索本行的异常开始右边界
extern uint8_t DectUSP;//搜索本行的异常开始上边界
extern uint8_t DectDSP;//搜索本行的异常开始下边界
extern uint8_t UPPatchRow;//上补线行
extern uint8_t DownPathRow;//下补线行
extern uint8_t DectBlackPoint;//搜索本行赛道内黑点数
extern uint8_t DectJumpRow;//搜索跳变点
extern uint8_t DectTrackWidth;//本行赛道宽度
extern uint8_t DectShapeWidth;//本行异常宽度
extern uint8_t DectTrackMid;//计算异常中点
extern float DectTrackRate;
extern float DectShapeRate;
extern uint8_t SBthing;//搜索行的跳变沿数
extern uint8_t LeftCol;//左边界
extern uint8_t RightCol;//右边界
extern int JumpRow;//跳变沿
extern uint8_t leftCol;//左边界
extern uint8_t rightCol;//右边界
extern int jumpRow;//跳变沿
extern bool ApriTag;
extern uint8_t UDnum;
extern uint8_t EndPatchRow;
extern uint8_t EndSearchRow;
extern float EndPatchlslope;
extern float EndPatchrslope;
extern uint16_t tcount;
extern uint16_t ttcount;
extern uint8_t NumJudge;
extern uint8_t ApriTagNum;
extern uint8_t AprilDir;
extern uint8_t CircleOut;
extern float CircleOutSlope;
extern uint8_t WrongStartL;
extern uint8_t WrongStartR;
extern uint8_t WrongEndL;
extern uint8_t WrongEndR;
extern uint8_t Car_Stop;




#endif
