#ifndef _IO_INIT_H_
#define _IO_INIT_H_

#include "delay.h"

void KEY_Init(void);
void Cannon_IO_Init(void);

#define KEY_row1	PEout(0)
#define KEY_row2	PEout(1)
#define KEY_row3	PEout(2)
#define KEY_row4	PEout(3)

#define KEY_cols1	PEin(4)
#define KEY_cols2	PEin(5)
#define KEY_cols3	PEin(6)
#define KEY_cols4	PEin(7)

#define CANNON_IN		PCout(6)//低开//高关
#define CANNON_OUT	PCout(7)//低开//高关

#endif


