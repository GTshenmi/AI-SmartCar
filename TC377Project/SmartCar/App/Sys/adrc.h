/*
 * adrc.h
 *
 *  Created on: 2021��7��31��
 *      Author: 936305695
 */

#ifndef APP_USR_ADRC_H_
#define APP_USR_ADRC_H_

#include "os.h"

typedef struct
{
    /*****���Ź��ȹ���*******/
    float x1;//����΢����״̬��
    float x2;//����΢����״̬��΢����
    float r;//ʱ��߶�
    float h;//ADRCϵͳ����ʱ��
    uint16_t N0;//����΢��������ٶȳ���h0=N*h

    float h0;
    float fh;//����΢�ּ��ٶȸ�����
    /*****����״̬�۲���*******/
    /******��ϵͳ���y������u�����ٹ���ϵͳ״̬���Ŷ�*****/
    float z1;
    float z2;
    float z3;//���ݿ��ƶ����������������ȡ���Ŷ���Ϣ
    float e;//ϵͳ״̬���
    float y;//ϵͳ�����
    float fe;
    float fe1;
    float beta_01;
    float beta_02;
    float beta_03;

    /**********ϵͳ״̬������*********/
    float e0;//״̬��������
    float e1;//״̬ƫ��
    float e2;//״̬��΢����
    float u0;//���������ϵͳ���
    float u;//���Ŷ�����������

    float e2_lpf;
    float b;

    /*********��һ�������ʽ*********/
    float beta_0;//����
    float beta_1;//��������ϲ���
    float beta_2;//u0=beta_1*e1+beta_2*e2+(beta_0*e0);
    /*********�ڶ��������ʽ*********/
    float alpha1;//u0=beta_1*fal(e1,alpha1,zeta)+beta_2*fal(e2,alpha2,zeta)
    float alpha2;//0<alpha1<1<alpha2
    float zeta;//���Զε����䳤��
    /*********�����������ʽ*********/
    float h1;//u0=-fhan(e1,e2,r,h1);
    uint16_t N1;//����΢��������ٶȳ���h0=N*h
    /*********�����������ʽ*********/
    float c;//u0=-fhan(e1,c*e2*e2,r,h1);
    float b0;//�Ŷ�����


    /*Control*/

    float Last_TD_Input;
    float TD_Input;
    float TD_Input_Div;

    float Last_ESO_Input;
    float ESO_Input;
    float ESO_Input_Div;


}ADRC_TypeDef;



void ADRC_Init(ADRC_TypeDef *fhan_Input1);
void Fhan_ADRC(ADRC_TypeDef *fhan_Input,float expect_ADRC);
float ADRC_Control(ADRC_TypeDef *adrc,float target,float actual);

extern ADRC_TypeDef M_ADRC;

#endif /* APP_USR_ADRC_H_ */
