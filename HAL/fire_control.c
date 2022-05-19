#include "main.h"

void fire_off()//关
{
	CANNON_IN = 1;
	CANNON_OUT = 1;
}

void cannon_power_in()//充电
{
	CANNON_OUT = 1;
	CANNON_IN = 0;
}

void cannon_power_out()//放电
{
	CANNON_IN = 1;
	CANNON_OUT = 0;
}

