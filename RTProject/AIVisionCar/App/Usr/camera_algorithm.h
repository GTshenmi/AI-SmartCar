#ifndef __CAMERA_ALGORITHM_H_
#define __CAMERA_ALGORITHM_H_

#include "sys.h"

typedef enum
{
  NoneCircle,//ûʶ�𵽻���
  Circle_State_1,//ʶ�𵽻������ڻ�����
  Circle_State_2,//�ڻ���ʧ����ʼ��ǲ���
  Circle_State_3,//���뻷��
  Circle_State_4,//�����������߽翪ʼ����
  Circle_State_5,//һ����ȫ����
  Circle_State_6,//��������
}circle_t;

typedef enum
{
  NoneDiff,//û�зֲ�·
  InDiff,//�ֲ�·��
  InDiff_patch,//�ֲ�·�в���
  InDiff_Stop,//ͣ��״̬
  OutDiff_Go,//ͣ�����ֲ�·
}diff_t;//�ֲ�·״̬

typedef enum
{
  Track_OK,//��������
  Track_Prejudge,//Ԥ�������쳣
  Track_ApriTag,//ʶ��ApriTag��
  Track_End,//ʶ���յ���
  Track_InPark_Stright,//������ֱ��
  Track_InPark_Turn,//��������
  Track_InPark_Stop,//���ͣ��
  Track_ApriTag_Stop,
  Track_ApriTag_Back,//����
  Track_ApriTag_Dect,//���
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
extern uint8_t TrackWidth[90];//�������
extern uint8_t SerachPoint[90];//������ʼ��
extern int MidError[90];
//extern uint16_t LEdata;
//extern uint16_t REdata;
extern uint8_t ScanEndRow;
extern uint8_t Left_Add_EndRow;//��߽粹�߿�ʼ��
extern uint8_t Right_Add_EndRow;//�ұ߽粹�߿�ʼ��
extern uint8_t Left_Add_StartRow;//��߽粹�߿�ʼ��
extern uint8_t Right_Add_StartRow;//�ұ߽粹�߿�ʼ��
extern uint8_t WrongTarckWidthCount;
extern uint8_t WrongTarckWidth;
extern float LeftRepairSlope;//��߽粹��б��
extern float RightRepairSlope;//�ұ߽粹��б��
extern uint8_t CalEndRow;
extern float CircleLeftSlope;//б��
extern float CircleRightSlope;//б��float CircleLeftSlope;//б��uint8_t CircleDectStart_Left;//�ѻ�����ʼ��
extern uint8_t CircleDectStart_Left;//�ѻ�����ʼ��
extern uint8_t CircleDectStart_Right;//�ѻ�����ʼ��
extern uint8_t CircleDectMid_Left;//�ѻ����м��
extern uint8_t CircleDectMid_Right;//�ѻ����м��
extern uint8_t CircleDectEnd_Left;//�ѻ���������
extern uint8_t CircleDectEnd_Right;//�ѻ���������
extern uint8_t CircleType;
extern int temp1;
extern float temp_slope1;
extern float temp_slope2;
extern uint8_t CirclePatchStart;//���ڲ��߿�ʼ��
extern uint8_t CirclePatchEnd;//���ڲ��߽�����
extern float CircleSlope;//��������б��
extern uint8_t outcircletype;
extern uint8_t circlestate;
extern circle_t LCircleState;//����״̬����ʼ��Ϊ�޻���
extern circle_t RCircleState;
extern diff_t Diffstate;
extern uint8_t DiffState;//1:��ת��2����ת
extern uint8_t diff;
extern uint8_t DiffEndRow;
extern uint8_t DiffEnd_L;
extern uint8_t DiffEnd_R;
extern uint8_t DiffSRow;
extern uint8_t DiffCount;
extern uint8_t DiffSCol_L;//�ֲ�·������������
extern uint8_t DiffSCol_R;//�ֲ�·������������
extern uint8_t DiffECol_L;//�ֲ�·������յ�������
extern uint8_t DiffECol_R;//�ֲ�·������յ�������
extern float DiffLslope;
extern float DiffRslope;
extern bool DiffFlag;
extern bool GetDiff;
extern bool DIffF;//�ֲ�·�ܱ�־
extern uint8_t ApriTagState;
extern uint8_t Blackrate;
extern uint8_t BlackPoint;//�ڵ����
extern bool ApriTagGet;
extern float lslope;
extern float rslope;
extern uint8_t LeftColPos;//��߽�λ��
extern uint8_t RightColPos;//�ұ߽�λ��
extern uint8_t CricleE;//�����յ�
extern float cirtpslope1;
extern float cirtpslope2;
extern uint8_t Circletmpnum;
extern int EncoderCount;
extern uint8_t BlackWidth;//�м�ڿ���
extern uint8_t AprilWidth;//ʶ���������
extern uint8_t AprilStart;//��ɫ��ʼ��
extern uint8_t AprilEnd;//��ɫ��ʼ��
extern uint8_t StartLeftCol;
extern uint8_t StartRightCol;
extern uint8_t TrackWrongStart;//�����쳣��ʼ��
extern uint8_t TrackWrongEnd;//�����쳣������
extern uint8_t TrackEnd;//�����ֹ��
extern bool InParkflag;
extern uint8_t InParkstate;
extern uint8_t InParkcount;
extern bool InPark;
extern uint8_t InParktype;//1����⣻2���ҿ�
extern uint8_t OutDiffway;
  
extern track_t TrackState;
extern uint8_t DectLeftCol;//�������е���߽�
extern uint8_t DectRightCol;//�������е��ұ߽�
extern uint8_t DectLSP;//�������е��쳣��ʼ��߽�
extern uint8_t DectRSP;//�������е��쳣��ʼ�ұ߽�
extern uint8_t DectUSP;//�������е��쳣��ʼ�ϱ߽�
extern uint8_t DectDSP;//�������е��쳣��ʼ�±߽�
extern uint8_t UPPatchRow;//�ϲ�����
extern uint8_t DownPathRow;//�²�����
extern uint8_t DectBlackPoint;//�������������ںڵ���
extern uint8_t DectJumpRow;//���������
extern uint8_t DectTrackWidth;//�����������
extern uint8_t DectShapeWidth;//�����쳣���
extern uint8_t DectTrackMid;//�����쳣�е�
extern float DectTrackRate;
extern float DectShapeRate;
extern uint8_t SBthing;//�����е���������
extern uint8_t LeftCol;//��߽�
extern uint8_t RightCol;//�ұ߽�
extern int JumpRow;//������
extern uint8_t leftCol;//��߽�
extern uint8_t rightCol;//�ұ߽�
extern int jumpRow;//������
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