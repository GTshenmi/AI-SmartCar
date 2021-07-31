/*
 * adrc.c
 *
 *  Created on: 2021��7��31��
 *      Author: 936305695
 */
#include "adrc.h"
#include "include.h"

ADRC_TypeDef ADRC_Pitch_Controller;
ADRC_TypeDef ADRC_Roll_Controller;
const float ADRC_Unit[4][16]=
{
  /*TD����΢����   �Ľ�����TD,h0=N*h      ����״̬�۲���ESO           �Ŷ�����     ���������*/
  /*  r     h      N                  beta_01   beta_02    beta_03     b0     beta_0  beta_1     beta_2     N1     C    alpha1  alpha2  zeta       b*/
  {1000000 ,0.002 , 5,               300,      10000,      100000,      0.5,    0.85,   3.3,      0.5,       2,    5,    0.9,   1.2,    0.03,    0.1},
  {1000000 ,0.005 , 5,               300,      10000,      100000,      0.5,    0.85,   1.5,      0.0003,       2,    5,    0.9,   1.2,    0.03,    0.1},
  {300000  ,0.005 , 3,               300,      4000,      10000,     100,   0.2,    1.2,      0.0010,       5,      5,    0.8,   1.5,    0.03,    0.05},
};

float Constrain_Float(float amt, float low, float high){
  return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

//Butter_Parameter ADRC_Div_LPF_Parameter={
  //200---20hz
//  1,    -1.14298050254,   0.4128015980962,
//  0.06745527388907,   0.1349105477781,  0.06745527388907
//};


//float ADRC_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
//{
////  /* ���ٶȼ�Butterworth�˲� */
////  /* ��ȡ����x(n) */
////  Buffer->Input_Butter[2]=curr_inputer;
////  /* Butterworth�˲� */
////  Buffer->Output_Butter[2]=
////    Parameter->b[0] * Buffer->Input_Butter[2]
////      +Parameter->b[1] * Buffer->Input_Butter[1]
////    +Parameter->b[2] * Buffer->Input_Butter[0]
////          -Parameter->a[1] * Buffer->Output_Butter[1]
////            -Parameter->a[2] * Buffer->Output_Butter[0];
////  /* x(n) ���б��� */
////  Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
////  Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
////  /* y(n) ���б��� */
////  Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
////  Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
////  return (Buffer->Output_Butter[2]);
//}




int16_t Sign_ADRC(float Input)
{
  int16_t output=0;
  if(Input>1E-6) output=1;
  else if(Input<-1E-6) output=-1;
  else output=0;
  return output;
}
int16_t Fsg_ADRC(float x,float d)
{
  int16_t output=0;
  output=(Sign_ADRC(x+d)-Sign_ADRC(x-d))/2;
  return output;
}
void ADRC_Init(ADRC_TypeDef *fhan_Input1)
{
  fhan_Input1->r=ADRC_Unit[0][0];
  fhan_Input1->h=ADRC_Unit[0][1];
  fhan_Input1->N0=(uint16)(ADRC_Unit[0][2]);
  fhan_Input1->beta_01=ADRC_Unit[0][3];
  fhan_Input1->beta_02=ADRC_Unit[0][4];
  fhan_Input1->beta_03=ADRC_Unit[0][5];
  fhan_Input1->b0=ADRC_Unit[0][6];
  fhan_Input1->beta_0=ADRC_Unit[0][7];
  fhan_Input1->beta_1=ADRC_Unit[0][8];
  fhan_Input1->beta_2=ADRC_Unit[0][9];
  fhan_Input1->N1=(uint16)(ADRC_Unit[0][10]);
  fhan_Input1->c=ADRC_Unit[0][11];

  fhan_Input1->alpha1=ADRC_Unit[0][12];
  fhan_Input1->alpha2=ADRC_Unit[0][13];
  fhan_Input1->zeta=ADRC_Unit[0][14];
  fhan_Input1->b=ADRC_Unit[0][15];

//  fhan_Input2->r=ADRC_Unit[1][0];
//  fhan_Input2->h=ADRC_Unit[1][1];
//  fhan_Input2->N0=(uint16)(ADRC_Unit[1][2]);
//  fhan_Input2->beta_01=ADRC_Unit[1][3];
//  fhan_Input2->beta_02=ADRC_Unit[1][4];
//  fhan_Input2->beta_03=ADRC_Unit[1][5];
//  fhan_Input2->b0=ADRC_Unit[1][6];
//  fhan_Input2->beta_0=ADRC_Unit[1][7];
//  fhan_Input2->beta_1=ADRC_Unit[1][8];
//  fhan_Input2->beta_2=ADRC_Unit[1][9];
//  fhan_Input2->N1=(uint16)(ADRC_Unit[1][10]);
//  fhan_Input2->c=ADRC_Unit[1][11];
//
//  fhan_Input2->alpha1=ADRC_Unit[1][12];
//  fhan_Input2->alpha2=ADRC_Unit[1][13];
//  fhan_Input2->zeta=ADRC_Unit[1][14];
//  fhan_Input2->b=ADRC_Unit[1][15];
}

float ADRC_Sign(float input)
{
    float output = 0.0;

    if(input >= 1e-6)
        output = 1.0;
    else if (input <= -1e-6)
        output = -1.0;
    else
        output = 0;

    return output;
}

float ADRC_Sat(float a,float d)
{
    float y = 0.0;

    if (fabs(a)<=d)
        y=a/d;
    else
        y=ADRC_Sign(a);

    return y;
}

float ADRC_Fst(float x1,float x2,float r,float h0)
{
    float d = 0.0,d0 = 0.0,y = 0.0,a0 = 0.0,a  = 0.0;
    float output = 0.0;

    d = h0 * r;
    d0 = h0 * d;
    y = x1 + h0 * x2;

    a0 = sqrt(powf(d,2.0) + 8.0 * r * fabs(y));

    if(fabs(y) <= d0)
        a = x2 + y/h0;
    else
        a = x2 + 0.5 * (a0 - d) * ADRC_Sign(y);

    output = -r * ADRC_Sat(a,d);


    return output;

}

float ADRC_Fal(float e,float alpha,float delta)
{
    float y = 0.0;

    if (fabs(e)>delta)
        y=powf(fabs(e),alpha)*ADRC_Sign(e);
    else
        y=e/(powf(delta,(1-alpha)));

    return y;
}

void ADRC_TD(ADRC_TypeDef *adrc,float target)
{
    float dx1=0;                                        //ADRC״̬���������
    dx1=adrc->x1-target;                                //��x1-v(k)���x1�õ���ɢ���¹�ʽ

    adrc->h0=adrc->N0*adrc->h;                          //��h0���h��������ٸ���΢�����ٶȳ�������

    adrc->fh = ADRC_Fst(dx1,adrc->x2,adrc->r,adrc->h0);

    adrc->x1+=adrc->h*adrc->x2;                         //�������ٸ���״̬��x1
    adrc->x2+=adrc->h*adrc->fh;                         //�������ٸ���״̬��΢��x2
}

void ADRC_NolinearConbination(ADRC_TypeDef *adrc)
{
    adrc->u0 = adrc->beta_01 * ADRC_Fal(adrc->e1,adrc->alpha1,adrc->zeta) +\
               adrc->beta_02 * ADRC_Fal(adrc->e2,adrc->alpha2,adrc->zeta);
}

void ADRC_ESO(ADRC_TypeDef *adrc)
{

    adrc->e=adrc->z1-adrc->y;//״̬���

    adrc->fe=ADRC_Fal(adrc->e,0.5,adrc->zeta);//�����Ժ�������ȡ����״̬�뵱ǰ״̬���
    adrc->fe1=ADRC_Fal(adrc->e,0.25,adrc->zeta);

  /*************��չ״̬������**********/
    adrc->z1+=adrc->h*(adrc->z2-adrc->beta_01*adrc->e);
    adrc->z2+=adrc->h*(adrc->z3
                                 -adrc->beta_02*adrc->fe
                                   +adrc->b*adrc->u);

    adrc->z3+=adrc->h*(-adrc->beta_03*adrc->fe1);
}

float ADRC_Control(ADRC_TypeDef *adrc,float target,float actual)
{
    ADRC_TD(adrc,target);       //����΢����
    adrc->y = actual * adrc->b;
    ADRC_ESO(adrc);             //����״̬�۲���
    adrc->e1=adrc->x1-adrc->z1; //״̬ƫ����
    adrc->e2=adrc->x2-adrc->z2; //״̬΢����
    ADRC_NolinearConbination(adrc);//�����Է���
    adrc->u=adrc->u0                //���������
        -adrc->z3/adrc->b;

    return adrc->u;
}






































//ADRC���ٸ���΢����TD���Ľ����㷨fhan
void Fhan_ADRC(ADRC_TypeDef *fhan_Input,float expect_ADRC)//����ADRC���ȹ���
{
  float d=0,a0=0,y=0,a1=0,a2=0,a=0;
  float x1_delta=0;//ADRC״̬���������
  x1_delta=fhan_Input->x1-expect_ADRC;//��x1-v(k)���x1�õ���ɢ���¹�ʽ
  fhan_Input->h0=fhan_Input->N0*fhan_Input->h;//��h0���h��������ٸ���΢�����ٶȳ�������
  d=fhan_Input->r*fhan_Input->h0*fhan_Input->h0;//d=rh^2;
  a0=fhan_Input->h0*fhan_Input->x2;//a0=h*x2
  y=x1_delta+a0;//y=x1+a0
  a1=sqrt(d*(d+8*fabs(y)));//a1=sqrt(d*(d+8*ABS(y))])
  a2=a0+Sign_ADRC(y)*(a1-d)/2;//a2=a0+sign(y)*(a1-d)/2;
  a=(a0+y)*Fsg_ADRC(y,d)+a2*(1-Fsg_ADRC(y,d));
  fhan_Input->fh=-fhan_Input->r*(a/d)*Fsg_ADRC(a,d)
    -fhan_Input->r*Sign_ADRC(a)*(1-Fsg_ADRC(a,d));//�õ�����΢�ּ��ٶȸ�����
  fhan_Input->x1+=fhan_Input->h*fhan_Input->x2;//�������ٸ���״̬��x1
  fhan_Input->x2+=fhan_Input->h*fhan_Input->fh;//�������ٸ���״̬��΢��x2
}

//ԭ�㸽���������Զε������ݴκ���
float Fal_ADRC(float e,float alpha,float zeta)
{
  int16_t s=0;
  float fal_output=0;
  s=(Sign_ADRC(e+zeta)-Sign_ADRC(e-zeta))/2;
  fal_output=e*s/(powf(zeta,1-alpha))+powf(fabs(e),alpha)*Sign_ADRC(e)*(1-s);
  return fal_output;
}
/************����״̬�۲���********************/
//״̬�۲�������beta01=1/h  beta02=1/(3*h^2)  beta03=2/(8^2*h^3) ...
void ESO_ADRC(ADRC_TypeDef *fhan_Input)
{
  fhan_Input->e=fhan_Input->z1-fhan_Input->y;//״̬���

  fhan_Input->fe=Fal_ADRC(fhan_Input->e,0.5,fhan_Input->N1*fhan_Input->h);//�����Ժ�������ȡ����״̬�뵱ǰ״̬���
  fhan_Input->fe1=Fal_ADRC(fhan_Input->e,0.25,fhan_Input->N1*fhan_Input->h);

  /*************��չ״̬������**********/
  fhan_Input->z1+=fhan_Input->h*(fhan_Input->z2-fhan_Input->beta_01*fhan_Input->e);
  fhan_Input->z2+=fhan_Input->h*(fhan_Input->z3
                                 -fhan_Input->beta_02*fhan_Input->fe
                                   +fhan_Input->b0*fhan_Input->u);
  //ESO����״̬���ٶ��źţ������Ŷ���������ͳMEMS������Ư�ƽϴ󣬹��ƻ����Ư��
  fhan_Input->z3+=fhan_Input->h*(-fhan_Input->beta_03*fhan_Input->fe1);
}
/************���������****************/
/*
void Nolinear_Conbination_ADRC(ADRC_TypeDef *fhan_Input)
{
float d=0,a0=0,y=0,a1=0,a2=0,a=0;
float Sy=0,Sa=0;//ADRC״̬���������
fhan_Input->h1=fhan_Input->N1*fhan_Input->h;
d=fhan_Input->r*fhan_Input->h1*fhan_Input->h1;
a0=fhan_Input->h1*fhan_Input->c*fhan_Input->e2;
y=fhan_Input->e1+a0;
a1=sqrt(d*(d+8*ABS(y)));
a2=a0+Sign_ADRC(y)*(a1-d)/2;
Sy=Fsg_ADRC(y,d);
a=(a0+y-a2)*Sy+a2;
Sa=Fsg_ADRC(a,d);
fhan_Input->u0=-fhan_Input->r*((a/d)-Sign_ADRC(a))*Sa-fhan_Input->r*Sign_ADRC(a);
//a=(a0+y)*Fsg_ADRC(y,d)+a2*(1-Fsg_ADRC(y,d));
//fhan_Input->fh=-fhan_Input->r*(a/d)*Fsg_ADRC(a,d)
//                -fhan_Input->r*Sign_ADRC(a)*(1-Fsg_ADRC(a,d));//�õ�����΢�ּ��ٶȸ�����
}
*/
void Nolinear_Conbination_ADRC(ADRC_TypeDef *fhan_Input)
{
  float temp_e2=0;
  //temp_e2=Constrain_Float(fhan_Input->e2,-3000,3000);
  temp_e2=Constrain_Float(fhan_Input->e2_lpf,-3000,3000);
  fhan_Input->u0=fhan_Input->beta_1*Fal_ADRC(fhan_Input->e1,fhan_Input->alpha1,fhan_Input->zeta)
    +fhan_Input->beta_2*Fal_ADRC(temp_e2,fhan_Input->alpha2,fhan_Input->zeta);

}

void ADRC_Control0(ADRC_TypeDef *fhan_Input,float expect_ADRC,float feedback_ADRC)
{
  fhan_Input->Last_TD_Input=fhan_Input->TD_Input;
  fhan_Input->TD_Input=expect_ADRC;
  fhan_Input->TD_Input_Div=(fhan_Input->TD_Input-fhan_Input->Last_TD_Input)/fhan_Input->h;

  fhan_Input->Last_ESO_Input=fhan_Input->ESO_Input;
  fhan_Input->ESO_Input=feedback_ADRC;
  fhan_Input->ESO_Input_Div=(fhan_Input->ESO_Input-fhan_Input->Last_ESO_Input)/fhan_Input->h;

  /*�Կ��ſ�������1��*/
  /********
      **
      **
      **
      **
      **
  ********/
  /*****
  ���Ź��ȹ��̣�����Ϊ����������
  ��TD����΢�����õ���
  ���������ź�x1����������΢���ź�x2
  ******/
  Fhan_ADRC(fhan_Input,expect_ADRC);

  /*�Կ��ſ�������2��*/
  /********
          *
          *
  *  * *  *
  *
  *
  ********/
  /************ϵͳ���ֵΪ��������״̬������ESO����״̬�۲���������*********/
  fhan_Input->y=feedback_ADRC;
  /*****
  ����״̬�۲������õ������źŵ�����״̬��
  1��״̬�ź�z1��
  2��״̬�ٶ��ź�z2��
  3��״̬���ٶ��ź�z3��
  ����z1��z2������Ϊ״̬������TD΢�ָ������õ���x1,x2�����
  ���������Ժ���ӳ�䣬����betaϵ����
  ��ϵõ�δ����״̬���ٶȹ����Ŷ�������ԭʼ������u
  *********/
  ESO_ADRC(fhan_Input);//�ͳɱ�MEMS�����Ư�ƣ���չ������z3�����Ư�ƣ�Ŀǰ��ʱδ�뵽�취�����δ�õ�z3
  /*�Կ��ſ�������3��*/
  /********
         **
      **
     **
      **
        **
  ********/
  /********״̬������***/
  fhan_Input->e0+=fhan_Input->e1*fhan_Input->h;//״̬������
  fhan_Input->e1=fhan_Input->x1-fhan_Input->z1;//״̬ƫ����
  fhan_Input->e2=fhan_Input->x2-fhan_Input->z2;//״̬΢����
  fhan_Input->e2_lpf=fhan_Input->e2;//ADRC_LPF(fhan_Input->e2,
                              //&fhan_Input->ADRC_LPF_Buffer,
                              //&ADRC_Div_LPF_Parameter);//������˹��ͨ��õ���΢����,20hz
  /********�������*******/
  /*
  fhan_Input->u0=//fhan_Input->beta_0*fhan_Input->e0
  +fhan_Input->beta_1*fhan_Input->e1
  +fhan_Input->beta_2*fhan_Input->e2;
  */
  Nolinear_Conbination_ADRC(fhan_Input);
  /**********�Ŷ�����*******/
  fhan_Input->u=fhan_Input->u0//-fhan_Input->z3/fhan_Input->b;
    -fhan_Input->z3/fhan_Input->b0;
  //fhan_Input->u+=Constrain_Float(fhan_Input->beta_0*fhan_Input->e0,-150,150);
  //����MEMS������Ư�ƱȽ����أ���beta_03ȡֵ�Ƚϴ�ʱ����ʱ��z3Ư�ƱȽϴ�Ŀǰ�������Ŷ�����������
  fhan_Input->u=Constrain_Float(fhan_Input->u0,-10000,10000);//�������Ŷ�����
  //fhan_Input->u=Constrain_Float(fhan_Input->u,-450,450);//�����Ŷ�����
}
void ADRC_Integrate_Reset(ADRC_TypeDef *fhan_Input)
{
    fhan_Input->e0=0.0f;
}


