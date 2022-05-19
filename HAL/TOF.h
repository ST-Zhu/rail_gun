#ifndef __SERIAL
#define __SERIAL


#include "sys.h"
#include "string.h"

#define HEADER 0x59



extern  int dist;
extern  int strength;

typedef union{
	int d;
	unsigned char c[2];
}int16uchar;

typedef struct{
	int16uchar light_dist;
	int16uchar light_strength;
	char mode;

}LightData;


extern u8 LIGHT_RXBuff[9];
extern u8 light_rx_data[9];
extern LightData light_data;

void light_uart1_init(void);
void getLightData(u8* data,LightData* light_data);

#endif
