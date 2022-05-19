#ifndef _CONTROL_TASK_H_
#define _CONTROL_TASK_H_
#include "stm32f4xx.h"

void control_task(void);
void fire_delay(void);
void fire_on(void);

void function_1(void);
void function_2(void);
void function_3(void);
void function_4(void);
void function_5(void);
void function_6(void);

void function_count_clear(void);

extern u8 auto_mode;


#endif
