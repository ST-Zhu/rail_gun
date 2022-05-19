#include "main.h"
#define FUNCTION_4_DELTA__ 1.8f;
#define FUNCTION_5_DELTA__ 0.115f;
int16_t key_set_distance = 2000;//按键设定的距离mm
int16_t key_set_angle = 90;//按键设定的角度
//初始化标志位
uint8_t function_2_count = 0;
uint8_t function_6_count = 0;
uint8_t function_3_count = 0;
uint8_t function_3_distance_count = 0;
//袁艺的变量
float yaw_real = 0.0f;
float diff = 0.0f;
float last_yaw_real = 0.0f;
float last_diff = 0.0f;
float p_angle = 0.0f;
u8 doing_shoot = 0;
uint64_t read_shoot = 0;
uint64_t function_change = 0;			
u8 auto_mode = 0;										//自瞄模式
float function_yaw_angle = 60.0f;		//任务4，5扫描起始角度
u8 if_lose_ = 0;
u8 last_if_lose_ = 0;
float funciotn_delta = 1.0f;				//扫描增量

int16_t fire_delay_time = 0;
uint8_t fire_on_temp = 0;
int16_t cannon_power_delay = 0;

void control_task() 
{
	if(project_temp == key_set_project_temp)
	{
		switch(project_temp)
		{
			case 1: 	function_1();	break;
			case 2:		function_2();	break;
			case 3:		function_3();	break;
			case 4:		function_4();	 break;
			case 5: 	function_5();	break;
			case 6:		function_6();	break;
		}
		
		fire_on();//开炮判断
	}
	else if(project_temp != key_set_project_temp)
	{
		function_count_clear();
		angle_init();
	}
}

void function_count_clear()
{
	
	//数据清零
	key_set_distance = 2000;
	key_set_angle = 90;
	key_number_sum = 0;
	fire_delay_time = 0;
	cannon_power_delay = 0;
	project_temp = 0;
	//标志位清除
	function_2_count = 0;
	function_6_count = 0;
	function_3_count = 0;
	function_3_distance_count = 0;
	fire_on_temp = 0;
	key_number_sure = 0;
	//袁艺的变量初始化
	yaw_real = 0.0f;
	diff = 0.0f;
	last_yaw_real = 0.0f;	
	last_diff = 0.0f;
	p_angle = 0.0f;
	doing_shoot = 0;
	read_shoot = 0;
	function_change = 0;
	auto_mode = 0;
	function_yaw_angle = 60.0f;
	funciotn_delta = 1.0f;
	target = 0.0f;
}

void fire_on()
{
	if(fire_on_temp == 1)
	{
		cannon_power_out();
		
		cannon_power_delay ++;
		if(cannon_power_delay > 500)
		{
			cannon_power_delay = 0;
			cannon_power_in();
			fire_on_temp = 0;
		}
	}
}

void fire_delay()
{
	if(fire_delay_time < 500)
	{
		fire_delay_time ++;
	}
	else if(fire_delay_time == 500)
	{
		fire_delay_time = 501;
		fire_on_temp = 1;
	}
}

void function_1()
{
	key_set_distance = 2000;
	key_set_angle = 90;
	
	cloud_control(key_set_angle,key_set_distance);//云台操作
	fire_delay();
	
}

void function_2()
{
	if(function_2_count == 0)//输入数值
	{
		//显示
		D_A_update(key_number_sum, 90);
		question_update(project_temp);
		
	
		if(key_number_sure == '#')//确定数值
		{
			key_number_sure = 0;
			function_2_count = 1;//改变标志位执行下一步指令
			key_set_distance = key_number_sum;
			key_number_sum = 0;//按键输入数据清零
			key_set_angle = 90;
			//显示设定最终值
			OLED_Clear();
			D_A_update(key_set_distance, key_set_angle);
			question_update(project_temp);
		}
	}
	else if(function_2_count == 1)//执行
	{
		cloud_control(key_set_angle,key_set_distance);
		
		fire_delay();
	}

}

void function_3()
{
	if(function_3_count == 0)//输入数值
	{
		if(function_3_distance_count == 0)//输入D
		{
			//显示
			D_A_update(key_number_sum, 90);
			question_update(project_temp);
			if(key_number_sure == '#')//确定数值
			{
				key_number_sure = 0;
				function_3_distance_count = 1;//改变标志位执行下一步指令
				key_set_distance = key_number_sum;
				key_number_sum = 0;//按键输入数据清零
				key_set_angle = 90;
				//显示设定最终值
				OLED_Clear();
				D_A_update(key_set_distance, key_set_angle);
				question_update(project_temp);
			}
		}
		else if(function_3_distance_count == 1)//输入A
		{
			//显示
			D_A_update(key_set_distance, key_number_sum);
			question_update(project_temp);
			if(key_number_sure == '#')//确定数值
			{
				key_number_sure = 0;
				function_3_count = 1;//改变标志位执行下一步指令
				key_set_angle = key_number_sum;
				key_number_sum = 0;//按键输入数据清零
				//显示设定最终值
				OLED_Clear();
				D_A_update(key_set_distance, key_set_angle);
				question_update(project_temp);
				
			}
		}
	}
	else if(function_3_count == 1)//执行
	{
		cloud_control(key_set_angle,key_set_distance);
		
		fire_delay();
	}
}
//u8 wwwwwww = 0;
//float p_angle = 0.0f;
void function_4()
{
	last_if_lose_ = if_lose_;
	if_lose_ = if_lose();
	if(last_if_lose_ == 0 && if_lose_ == 1)
		function_yaw_angle = 60.0f;
	if(function_change >= 100)
	{
	
		if(if_lose_ == 0)
		{	
			if(auto_mode != 2)
				auto_mode = 1;
		}
			
		else 
		{
			if(function_change % 10 == 0)
				function_yaw_angle += funciotn_delta;
				auto_mode = 0;
		}
	}
	if(auto_mode == 0)
		angle_set_update(function_yaw_angle, 90.0f);
	
	if(function_yaw_angle>= 120.0f)
		funciotn_delta = -FUNCTION_4_DELTA__;
	if(function_yaw_angle <= 60.0f)
		funciotn_delta = FUNCTION_4_DELTA__;
	function_change++;
	
	if(aimed())
	{
		p_angle = cala_elevation((real_dis - 40) * 10);
		angle_set_update(t_angle.real_yaw, p_angle);
		read_shoot  = function_change;
	}
	
	if((function_change - read_shoot >= 10) && read_shoot != 0)
		doing_shoot = 1;
	if(doing_shoot)
		fire_delay();
//		

}

void function_5()
{
	if(function_change % 10 == 0)
		function_yaw_angle += funciotn_delta;

	if(function_yaw_angle >= 120.0f)
	{
		funciotn_delta = -FUNCTION_5_DELTA__;

	}
		
	if(function_yaw_angle <= 61.0f)
		funciotn_delta = FUNCTION_5_DELTA__;
	angle_set_update(function_yaw_angle, t_angle.real_pitch);
	function_change++;
	if(target != 0.0f)
	{
		last_yaw_real = yaw_real;
		yaw_real = t_angle.real_yaw;
		if(last_yaw_real != 0.0f)
		{
			last_diff = last_yaw_real - target;
			diff = yaw_real - target;
			
			if(fabs(last_diff) > fabs(diff))
			{
				if(fabs(diff) <= 5.0f)
					doing_shoot = 1;
			}
		}
		p_angle = cala_elevation(2500);
		angle_set_update(function_yaw_angle, p_angle);		
	}
	if(doing_shoot)
		fire_delay();
}

void function_6()//测试弹道曲线
{
	if(function_6_count == 0)//输入数值
	{
		//显示
		D_A_update(key_number_sum, 90);
		question_update(project_temp);
		
	
		if(key_number_sure == '#')//确定数值
		{
			key_number_sure = 0;
			function_6_count = 1;//改变标志位执行下一步指令
			key_set_distance = key_number_sum;
			key_number_sum = 0;//按键输入数据清零
			key_set_angle = 90;
			//显示设定最终值
			OLED_Clear();
			D_A_update(key_set_distance, key_set_angle);
			question_update(project_temp);
		}
	}
	else if(function_6_count == 1)//执行
	{
		angle_set_update(90, key_set_distance);
		
		fire_delay();
	}
}
