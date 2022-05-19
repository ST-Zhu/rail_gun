#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "delay.h"

extern uint8_t project_temp;
extern int16_t key_number_sum;
extern char key_number_sure;
extern uint8_t key_set_project_temp;

void Kyeboard(void);
void scan(void);
void zero_clearing(void);
void key_change_number(uint16_t key);
void project_change(void);
void key_set_sure(void);

#endif


