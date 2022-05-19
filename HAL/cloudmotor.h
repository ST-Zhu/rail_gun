#ifndef __CLOUDMOTOR_H___
#define __CLOUDMOTOR_H___

#include "delay.h"

typedef struct Angle
{
	u32 real_yaw;
	u32 real_pitch;
	float set_yaw;
	float set_pitch;
}Angle;

extern Angle t_angle;
void angle_init(void);
void angle_set_update(float yaw, float pitch);
void cloud_control(float angle, float distance);
float cala_elevation(float distance);
#endif
