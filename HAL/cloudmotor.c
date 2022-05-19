#include "main.h"


Angle t_angle;

//云台电机初始化
void angle_init(void)//根据MPU位置改
{
//	TIM4->CCR1 = 1500;
//	TIM4->CCR2 = 1500;	
	t_angle.set_yaw = 90.0f;
	t_angle.set_pitch = 90.0f;
	angle_set_update(t_angle.set_yaw, t_angle.set_pitch);
}


//云台电机更新设定值
//封装//输入角度
void angle_set_update(float yaw, float pitch)//左上减
{
	t_angle.set_yaw = yaw;
	t_angle.set_pitch = pitch;
	if(yaw <= -0.1f || yaw >= 180.1f)
		yaw = 90.0f;
	if(pitch <= 24.0f || pitch >= 121.0f )
		pitch = 90.0f;
	float b = 2500.0f;
	float a = -200.0f / 27.0f;

	float yaw_pwm_set   = (yaw + 45.0f) * a + b;
	float pitch_pwm_set = (pitch + 45.0f) * a + b;
	TIM4->CCR1 = (uint32_t)yaw_pwm_set;
	TIM4->CCR2 = (uint32_t)pitch_pwm_set;
	t_angle.real_yaw   = yaw;
	t_angle.real_pitch = pitch;
}

void cloud_control(float angle, float distance)
{
	float change_angle,change_distance;
	change_angle = angle;
	change_distance = cala_elevation(distance);
	
	angle_set_update(change_angle, change_distance);
}

double elevation = 0.0f;
float cala_elevation(float distance)
{
	elevation = -0.0000028891585730273089540698708277411 * (double)distance * (double)distance - 0.00070164970341503882581002393692415 * (double)distance + 93.413306827975560509003116749227;
	return (float)elevation;
}

