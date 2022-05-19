#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "stm32f4xx.h"




#define RX_BUF_SIZE 16		//DMA接收缓冲区大小
extern	u8 if_;

//装甲信息结构体定义
typedef union {
	int32_t d;
	unsigned char c[4];
}int32uchar;



typedef struct {
	int32uchar viosion_yaw;
	int32uchar viosion_pitch;
	int32uchar viosion_dis;
}VisionData;


extern u8 RXBuff[RX_BUF_SIZE];		//DMA接收缓冲区
extern u8 rx_data[16];
extern VisionData visiondata;

void getVisionData(u8* data,VisionData* enemy_position);
void miniPC_uart1_init(void);
void miniPC_uart1_tx(u8 *USART_RX_BUF ,int len);
u8 if_lose(void);
extern int32_t posx;
u8 aimed(void);
extern int real_dis;
extern float target;
#endif 
