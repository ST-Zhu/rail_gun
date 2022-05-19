#include "main.h"



u8 LIGHT_RXBuff[9];
u8 light_rx_data[9];
LightData light_data;


int dist = 0;
int strength = 0;


void light_data_init()
{
	light_data.light_dist.d = 0;
	light_data.light_strength.d = 0;
	light_data.mode = 0;
}

void light_uart1_init(void)
{
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma用到中断
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);;//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA2时钟使能 ---********************
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* TXIO */  

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;                
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* RXIO */

  //USART1 初始化设置
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART2, ENABLE);  //使能串口1
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收     
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;//DMA1 数据流5 中断通道*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	
  /* 配置 DMA Stream */
	DMA_DeInit(DMA1_Stream5); 	
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);//等待DMA可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);//DMA外设地址****************************
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)LIGHT_RXBuff;//DMA 存储器0地址 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式*******************************
	DMA_InitStructure.DMA_BufferSize = 9;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//中等优先级**********
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//***************
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输*****
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输

	DMA_Init(DMA1_Stream5, &DMA_InitStructure);//初始化DMA2 Stream5	
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Stream5,ENABLE);
	
	light_data_init();

	
}


int hhhh = 0;
void DMA1_Stream5_IRQHandler(void)
{
	hhhh ++;
	if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5)!=RESET)
	{	
		
		DMA_Cmd(DMA1_Stream5, DISABLE); //关闭DMA,防止处理其间有数据		
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);		
		memcpy(light_rx_data,LIGHT_RXBuff,9);
		getLightData(light_rx_data,&light_data);
		dist = light_data.light_dist.d;
//		if(light_data.light_dist.d <= 250)
//		{
//			dist = light_data.light_dist.d - 10 + 20;
//		}
//		else if(light_data.light_dist.d > 250 && light_data.light_dist.d <= 270)
//		{
//			dist = light_data.light_dist.d - 10 + 15;
//		}
//		else 
//		{
//			dist = light_data.light_dist.d - 10;
//		}
		DMA_Cmd(DMA1_Stream5, ENABLE); 
	}
}

void getLightData(u8* data,LightData* light_data)
{
	if(data[0] == HEADER &&data[1] == HEADER)
	{
		u8 check = 0x00;
		int i;
		for(i=0;i<8;i++)
		{
			check += data[i];
		}
		
		if((check&0xff) == data[8])
		{
			light_data->light_dist.c[0] = data[2];
			light_data->light_dist.c[1] = data[3];
			
			light_data->light_strength.c[0] = data[4];
			light_data->light_strength.c[1] = data[5];
			
			light_data->mode = data[6];
		}
	}
}



