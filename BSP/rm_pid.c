#include "main.h"

PID pid[ALL_PID];		//��������PID�ṹ������
float out[ALL_PID] = {0};//PID���ֵ����

//pidֵ��ʼ��
static void pid_init(PID *pid, float kp, float ki, float kd, float poutmax, float ioutmax, float doutmax, float outmax)
{	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->poutmax = poutmax;
	pid->ioutmax = ioutmax;
	pid->doutmax = doutmax;
	pid->outmax = outmax;
	
	pid->f_pid_reset = pid_reset;
	pid->f_pid_reset(pid);
	
	pid->err = 0;
	pid->err_last = 0;
	pid->err_llast = 0;
	pid->integral = 0;
	
	
}
//pid���ֵ����
static void pid_reset(PID *pid)
{	
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	pid->out  = 0;
}
//���е��pidֵ��ʼ�����β���Ҫ�������ģ���һ�β����β�ȫΪ0��
//(&pid[i], kp, ki, kd, poutmax, ioutmax, doutmax, outmax)
void All_Pid_Configuration(PID pid[])
{	
	pid[VISUAL_YAW_OFFSET].f_pid_init = pid_init;
	pid[VISUAL_YAW_OFFSET].f_pid_init(&pid[VISUAL_YAW_OFFSET], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	
	pid[VISUAL_PITCH_OFFSET].f_pid_init = pid_init;
	pid[VISUAL_PITCH_OFFSET].f_pid_init(&pid[VISUAL_PITCH_OFFSET], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

//pid��������ֵ 
float Calculate_Current_Value(PID *pid, float set, float real)
{
	//����������һ�ε����ֵ
	pid->f_pid_reset = pid_reset;
	pid->f_pid_reset(pid);
	
	pid->set = set ;
	pid->real = real;
	
	pid->err_last = pid->err;
	pid->err = pid->set - pid->real;
	pid->integral += pid->err;
	
	pid->pout = pid->kp * pid->err;
	pid->pout = pid->pout < pid->poutmax ? pid->pout : pid->poutmax;
	pid->pout = pid->pout > -pid->poutmax ? pid->pout : -pid->poutmax;
	
	pid->iout = pid->ki * pid->integral;
	pid->iout = pid->iout < pid->ioutmax  ? pid->iout : pid->ioutmax;
	pid->iout = pid->iout > -pid->ioutmax ? pid->iout : -pid->ioutmax;
	
	pid->dout = pid->kd * (pid->err - pid->err_last);
	pid->dout = pid->dout < pid->doutmax ? pid->dout : pid->doutmax;
	pid->dout = pid->dout > -pid->doutmax ? pid->dout : -pid->doutmax;
	
	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = pid->out < pid->outmax ? pid->out : pid->outmax;
	pid->out = pid->out > -pid->outmax ? pid->out : -pid->outmax;

	return pid->out;
}
float Calculate_Current_Value_For_Err(PID *pid, float err)
{
	pid->f_pid_reset = pid_reset;
	pid->f_pid_reset(pid);
	
	
	pid->err_last = pid->err;
	pid->err = err;
	pid->integral += pid->err;
	
	pid->pout = pid->kp * pid->err;
	pid->pout = pid->pout < pid->poutmax ? pid->pout : pid->poutmax;
	pid->pout = pid->pout > -pid->poutmax ? pid->pout : -pid->poutmax;
	
	pid->iout = pid->ki * pid->integral;
	pid->iout = pid->iout < pid->ioutmax  ? pid->iout : pid->ioutmax;
	pid->iout = pid->iout > -pid->ioutmax ? pid->iout : -pid->ioutmax;
	
	pid->dout = pid->kd * (pid->err - pid->err_last);
	pid->dout = pid->dout < pid->doutmax ? pid->dout : pid->doutmax;
	pid->dout = pid->dout > -pid->doutmax ? pid->dout : -pid->doutmax;
	
	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = pid->out < pid->outmax ? pid->out : pid->outmax;
	pid->out = pid->out > -pid->outmax ? pid->out : -pid->outmax;
	
	return pid->out;
}
