#ifndef		_RM_PID_H_
#define		_RM_PID_H_

#include "delay.h"

//宏定义pid结构体数组下标
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
	
	float err;							//定义偏差值
	float err_last;					//上一次偏差值
	float err_llast;				//上上次偏差值
	float integral;					//累计偏差值
	void(*f_pid_init)(struct PID *pid, float kp, float ki, float kd, float poutmax, float ioutmax, float doutmax, float outmax);			//用来初始化pid
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
