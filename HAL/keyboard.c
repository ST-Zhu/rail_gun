#include "main.h"

char key_real = 0;//����ԭʼ��ʵֵ
int16_t key_number_sum = 0;//������ʵֵ
uint8_t key_set_project_temp = 0;
uint8_t project_temp = 0;//��Ŀ����ʵֵ
char key_number_sure = 0;//������ֵȷ����־λ

void Kyeboard(void)
{
	scan();
	
	switch(key_real)
	{
		case '1':	key_change_number(1);	break;
		case '2':	key_change_number(2);	break;
		case '3':	key_change_number(3);	break;
		case '4':	key_change_number(4);	break;
		case '5':	key_change_number(5);	break;
		case '6':	key_change_number(6);	break;
		case '7':	key_change_number(7);	break;
		case '8':	key_change_number(8);	break;
		case '9':	key_change_number(9);	break;
		case '0':	key_change_number(0);	break;
		
		case 'A':	project_change();		break;
		case 'D':	key_set_sure();			break;
		
		case '*':	zero_clearing();		break;

	}
	key_real = 0;
}

void key_set_sure()
{
	project_temp = key_set_project_temp;
}

void project_change()
{
	key_set_project_temp ++;
	if(key_set_project_temp > 6)
	{
		key_set_project_temp = 1;
	}
	OLED_Clear();
	question_update(key_set_project_temp);//��ʾ
}

void key_change_number(uint16_t key)
{
	
	key_number_sum = key_number_sum * 10 + key;

}

void zero_clearing()
{
	key_number_sum = 0;
}

uint8_t key_temp_ones;

void scan()
{
		//��һ��
	KEY_row1 = 1;
	KEY_row2 = 0;
	KEY_row3 = 0;
	KEY_row4 = 0;
	key_temp_ones = (KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1);
	if(key_temp_ones == 1)
	{
		delay_ms(50);
		if(key_temp_ones == 1)
		{
			if(KEY_cols1 == 1)	key_real = '1';
			else if(KEY_cols2 == 1)	key_real = '2';
			else if(KEY_cols3 == 1)	key_real = '3';
			else if(KEY_cols4 == 1)	key_real = 'A';
		}
		while((KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1));//�ɿ�����ʱ�������
	}

	
		//�ڶ���
	KEY_row1 = 0;
	KEY_row2 = 1;
	KEY_row3 = 0;
	KEY_row4 = 0;
	key_temp_ones = (KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1);
	if(key_temp_ones == 1)
	{
		delay_ms(50);
		if(key_temp_ones == 1)
		{
			if(KEY_cols1 == 1)	key_real = '4';
			else if(KEY_cols2 == 1)	key_real = '5';
			else if(KEY_cols3 == 1)	key_real = '6';
			else if(KEY_cols4 == 1)	key_real = 'B';
		}
		while((KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1));//�ɿ�����ʱ�������
	}
	
		//������
	KEY_row1 = 0;
	KEY_row2 = 0;
	KEY_row3 = 1;
	KEY_row4 = 0;
	key_temp_ones = (KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1);
	if(key_temp_ones == 1)
	{
		delay_ms(50);
		if(key_temp_ones == 1)
		{
			if(KEY_cols1 == 1)	key_real = '7';
			else if(KEY_cols2 == 1)	key_real = '8';
			else if(KEY_cols3 == 1)	key_real = '9';
			else if(KEY_cols4 == 1)	key_real = 'C';
		}
		while((KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1));//�ɿ�����ʱ�������

	}
	
		//������
	KEY_row1 = 0;
	KEY_row2 = 0;
	KEY_row3 = 0;
	KEY_row4 = 1;
	key_temp_ones = (KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1);
	if(key_temp_ones == 1)
	{
		delay_ms(50);
		if(key_temp_ones == 1)
		{
			if(KEY_cols1 == 1)	key_real = '*';
			else if(KEY_cols2 == 1)	key_real = '0';
			else if(KEY_cols3 == 1)	key_number_sure = '#';
			else if(KEY_cols4 == 1)	key_real = 'D';
		}
		while((KEY_cols1 ==1) || (KEY_cols2 == 1) || (KEY_cols3 == 1) || (KEY_cols4 == 1));//�ɿ�����ʱ�������

	}
}

