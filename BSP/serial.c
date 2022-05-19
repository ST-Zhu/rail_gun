//#include "sys.h"
//#include "string.h"
//#include "serial.h"
//#include "cloudmotor.h"
//#include "control_task.h"
//#include "math.h"
//#include "TOF.h"
#include "main.h"
u8 RXBuff[RX_BUF_SIZE];		//DMA接收缓冲区
u8 rx_data[16];
u8 aimed_ok = 0;
uint64_t aimed_times = 0;
VisionData visiondata;
/**
  * @brief  miniPC串口DMA初始化
  * @param  None
  * @retval None
  * @note   RXBuff 串口数据接受缓存
  */
	//jscope看值
	int32_t posx = 0;
	int32_t posy = 0;
	int32_t posdis = 0;
	u8 if_ = 0;
	u8 last_if_ = 0;
void miniPC_uart1_init(void)
{
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma用到中断
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);;//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能 ---********************
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* TXIO */  

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;                
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* RXIO */

  //USART1 初始化设置
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收     
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;//DMA1 数据流5 中断通道*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	
  /* 配置 DMA Stream */
	DMA_DeInit(DMA2_Stream5); 	
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);//等待DMA可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);//DMA外设地址****************************
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RXBuff;//DMA 存储器0地址 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式*******************************
	DMA_InitStructure.DMA_BufferSize = RX_BUF_SIZE;//数据传输量 
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

	DMA_Init(DMA2_Stream5, &DMA_InitStructure);//初始化DMA2 Stream5	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
	
	//视觉信息初始化防止编译器未定义错误
	visiondata.viosion_pitch.d = 0;
	visiondata.viosion_yaw.d = 0;
	visiondata.viosion_dis.d = 0;

	
	
}



float target = 0.0f;
void DMA2_Stream5_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream5,DMA_FLAG_TCIF5)!=RESET)
	{
		DMA_Cmd(DMA2_Stream5, DISABLE); //关闭DMA,防止处理其间有数据		
		DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);		
		memcpy(rx_data,RXBuff,16);
		getVisionData(rx_data,&visiondata);
		
		posx = visiondata.viosion_yaw.d;
		posy = visiondata.viosion_pitch.d;
		posdis = visiondata.viosion_dis.d;
		
		if(auto_mode == 1)
				angle_set_update(t_angle.real_yaw - posx / 10.0f,90.0f);	
//		if(if_ != 0)
//			target = t_angle.real_yaw - posx / 10.0f;
		if(if_!= 0 && (fabs(posx) <= 0.5f) && project_temp == 5)
			target = t_angle.real_yaw;
		DMA_Cmd(DMA2_Stream5, ENABLE); 
	}
}


void miniPC_uart1_tx(u8 *USART_RX_BUF ,int len)
{
			int t;
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
}


void getVisionData(u8* data,VisionData* enemy_position)
{
	
	if(data[0] == 0xA5 && data[1] == 0xA6) 
	{
		enemy_position->viosion_yaw.c[0] = data[5];
		enemy_position->viosion_yaw.c[1] = data[4];
		enemy_position->viosion_yaw.c[2] = data[3];
		enemy_position->viosion_yaw.c[3] = data[2];
		
		enemy_position->viosion_pitch.c[0] = data[9];
		enemy_position->viosion_pitch.c[1] = data[8];
		enemy_position->viosion_pitch.c[2] = data[7];
		enemy_position->viosion_pitch.c[3] = data[6];
		
		enemy_position->viosion_dis.c[0] = data[13];
		enemy_position->viosion_dis.c[1] = data[12];
		enemy_position->viosion_dis.c[2] = data[11];
		enemy_position->viosion_dis.c[3] = data[10];
		last_if_ = if_;
		if_ = data[14];
		
	}	
}
u8 if_lose(void)
{
	static u32 counts_i = 0;
	if(last_if_ == 0 && if_ == 0)
	{
		counts_i ++;
	}
	else 
	{
		counts_i = 0;
	}
	if(counts_i >= 499)
		return 1;
	return 0;
}

int real_dis = 0;
//uint64_t aimed_times = 0;
u8 aimed(void)
{
	if(auto_mode == 1 && fabs(posx) <= 10 && if_ != 0)
	{
		aimed_times ++;
	}
	else 
		aimed_times = 0;
	if(aimed_times >= 490)
	{
	
		if(aimed_times >= 500)
		{
			auto_mode = 2;
			aimed_times = 502;
			return 1;
		}	
		real_dis = dist;
		return 0;
	}

		
	return 0;
}
