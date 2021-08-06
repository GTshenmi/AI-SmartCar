/*
 * adrc.c
 *
 *  Created on: 2021��7��31��
 *      Author: 936305695
 */
#include "adrc.h"
#include "include.h"

const float ADRC_Unit[4][16]=
{
  /*TD����΢����   �Ľ�����TD,h0=N*h      ����״̬�۲���ESO           �Ŷ�����     ���������*/
  /*  r     h      N                  beta_01   beta_02    beta_03     b0     beta_0  beta_1     beta_2     N1     C    alpha1  alpha2  zeta       b*/
  {1000000 ,0.002 , 5,               300,      10000,      100000,      0.5,    0.85,   3.3,      0.5,       2,    5,    0.9,   1.2,    0.03,    0.1},
  {1000000 ,0.005 , 5,               300,      10000,      100000,      0.5,    0.85,   1.5,      0.0003,       2,    5,    0.9,   1.2,    0.03,    0.1},
  {300000  ,0.005 , 3,               300,      4000,      10000,     100,   0.2,    1.2,      0.0010,       5,      5,    0.8,   1.5,    0.03,    0.05},
};

void ADRC_Init(ADRC_TypeDef *adrc)
{
    adrc->r= 10000;
    adrc->h= 0.002;
    adrc->N0= 5;
    adrc->beta_01= 300;
    adrc->beta_02= 500;
    adrc->beta_03= 600;
    adrc->nlsef_alpha1 = 0.7;
    adrc->nlsef_alpha2 = 0.95;

    adrc->alpha1=0.5;
    adrc->alpha2=0.25;
    adrc->delta= 0.0025;
    adrc-> b= 1.0;
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

    //Console.WriteLine("ADRC-TD:%f,%f",adrc->x1,adrc->x2);
}

void ADRC_NolinearConbination(ADRC_TypeDef *adrc)
{
    adrc->u0 = adrc->beta_01 * ADRC_Fal(adrc->e1,adrc->nlsef_alpha1,adrc->delta) +\
               adrc->beta_02 * ADRC_Fal(adrc->e2,adrc->nlsef_alpha2,adrc->delta);
}

void ADRC_ESO(ADRC_TypeDef *adrc)
{

    adrc->e=adrc->z1-adrc->y;//״̬���

    adrc->fe=ADRC_Fal(adrc->e,0.5,adrc->delta);//�����Ժ�������ȡ����״̬�뵱ǰ״̬���
    adrc->fe1=ADRC_Fal(adrc->e,0.25,adrc->delta);

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

    Console.WriteLine("ADRC:%f,%f,%f,%f,%f,%f",adrc->z1,adrc->z2,adrc->z3,adrc->e1,adrc->e2,adrc->u0);

    adrc->u=adrc->u0                //���������
        -adrc->z3/adrc->b;

    return adrc->u;
}
