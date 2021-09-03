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

    float b;

    float nlsef_alpha1;
    float nlsef_alpha2;

    float delta;

    float alpha1;
    float alpha2;

//    float b0;//�Ŷ�����

}ADRC_TypeDef;



void ADRC_Init(ADRC_TypeDef *adrc);
void Fhan_ADRC(ADRC_TypeDef *fhan_Input,float expect_ADRC);
float ADRC_Control(ADRC_TypeDef *adrc,float target,float actual);

extern ADRC_TypeDef M_ADRC;

#endif /* APP_USR_ADRC_H_ */
