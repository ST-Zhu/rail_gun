#ifndef		_RM_PID_H_
#define		_RM_PID_H_

#include "delay.h"

//�궨��pid�ṹ�������±�
#define VISUAL_YAW_OFFSET 0
#define VISUAL_PITCH_OFFSET 1
#define ALL_PID 2

typedef struct PID
{
	float kp;
	float ki;
	float kd;
	
	float pout;
	float iout;
	float dout;
	
	float poutmax;
	float ioutmax;
	float doutmax;
	float outmax;
	
	float set;
	float real;
	float out;
	
	float err;							//����ƫ��ֵ
	float err_last;					//��һ��ƫ��ֵ
	float err_llast;				//���ϴ�ƫ��ֵ
	float integral;					//�ۼ�ƫ��ֵ
	void(*f_pid_init)(struct PID *pid, float kp, float ki, float kd, float poutmax, float ioutmax, float doutmax, float outmax);			//������ʼ��pid
	void(*f_pid_reset)(struct PID *pid);
}PID;

extern PID pid[ALL_PID];
extern float out[ALL_PID];
static void pid_init(PID *pid, float kp, float ki, float kd, float poutmax, float ioutmax, float doutmax, float outmax);
static void pid_reset(PID *pid);

void All_Pid_Configuration(PID pid[]);
float Calculate_Current_Value(PID *pid, float set, float real);
float Calculate_Current_Value_For_Err(PID *pid, float err);

#endif
