#include "main.h"

void fire_off()//��
{
	CANNON_IN = 1;
	CANNON_OUT = 1;
}

void cannon_power_in()//���
{
	CANNON_OUT = 1;
	CANNON_IN = 0;
}

void cannon_power_out()//�ŵ�
{
	CANNON_IN = 1;
	CANNON_OUT = 0;
}

