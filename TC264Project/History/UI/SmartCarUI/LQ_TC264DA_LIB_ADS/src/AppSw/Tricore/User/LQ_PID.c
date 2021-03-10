/*!
  * @file     LQ_PID.c
  *
  * @brief    PID����
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     Tab�� 4���ո�
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2020��6��5��
  */ 

#include <LQ_PID.h>


/*!
  * @brief    �޷�����
  *
  * @param    amt   �� ����
  * @param    low   �� ���ֵ
  * @param    high  �� ���ֵ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      ��
  *
  * @date     2020/6/8
  */
float constrain_float(float amt, float low, float high)
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}




/*!
  * @brief    pid������ʼ������
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      ��
  *
  * @date     2020/6/8
  */
void PidInit(pid_param_t * pid)
{
	pid->kp        = 0;
	pid->ki        = 0;
	pid->kd        = 0;
	pid->imax      = 0;
	pid->out_p     = 0;
	pid->out_i     = 0;
	pid->out_d     = 0;
	pid->out       = 0;
	pid->integrator= 0;
	pid->last_error= 0;
	pid->last_derivative   = 0;
	pid->last_t    = 0;
}


/*!
  * @brief    pidλ��ʽ���������
  *
  * @param    pid     pid����
  * @param    error   pid�������
  *
  * @return   PID������
  *
  * @note     ��
  *
  * @see      ��
  *
  * @date     2020/6/8
  */
float PidLocCtrl(pid_param_t * pid, float error)
{
	/* �ۻ���� */
	pid->integrator += error;

	/* ����޷� */
	constrain_float(pid->integrator, -pid->imax, pid->imax);


	pid->out_p = pid->kp * error;
	pid->out_i = pid->ki * pid->integrator;
	pid->out_d = pid->kd * (error - pid->last_error);

	pid->last_error = error;

	pid->out = pid->out_p + pid->out_i + pid->out_d;

	return pid->out;
}


/*!
  * @brief    pid����ʽ���������
  *
  * @param    pid     pid����
  * @param    error   pid�������
  *
  * @return   PID������   ע���������Ѿ��������ϴν��
  *
  * @note     ��
  *
  * @see      ��
  *
  * @date     2020/6/8
  */
float PidIncCtrl(pid_param_t * pid, float error)
{

	pid->out_p = pid->kp * (error - pid->last_error);
	pid->out_i = pid->ki * error;
	pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

	pid->last_derivative = error - pid->last_error;
	pid->last_error = error;

	pid->out += pid->out_p + pid->out_i + pid->out_d;

	return pid->out;
}

