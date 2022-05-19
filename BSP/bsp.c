#include "main.h"


void BSP_Init()	//底层初始化
{
	TIM6_Configure();	
	KEY_Init();
	Cannon_IO_Init();
	CloudMotor_Configure();
	angle_init();
	delay_ms(1000);
	All_Pid_Configuration(pid);//pid参数初始化	
	
	OLED_Init();
	OLED_Clear();
	Rheostat_Init();//ADC初始化	

	miniPC_uart1_init();
	light_uart1_init();
	cannon_power_in();
	TIM6_Start();
}
