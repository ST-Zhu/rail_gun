#include "main.h"
float dis = 1750.0f;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	BSP_Init();
	
	while(1)
	{
//			cala_elevation(dis);
//			dis+=10.0f;
//			delay_ms(100);
	}
}
