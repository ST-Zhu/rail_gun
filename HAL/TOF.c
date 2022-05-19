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
  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma�õ��ж�
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);;//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA2ʱ��ʹ�� ---********************
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
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

  //USART1 ��ʼ������
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  //ʹ�ܴ���1��DMA����     
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;//DMA1 ������5 �ж�ͨ��*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	
  /* ���� DMA Stream */
	DMA_DeInit(DMA1_Stream5); 	
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);//�ȴ�DMA������
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);//DMA�����ַ****************************
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)LIGHT_RXBuff;//DMA �洢��0��ַ �ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ*******************************
	DMA_InitStructure.DMA_BufferSize = 9;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//�е����ȼ�**********
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//***************
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���*****
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���

	DMA_Init(DMA1_Stream5, &DMA_InitStructure);//��ʼ��DMA2 Stream5	
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
		
		DMA_Cmd(DMA1_Stream5, DISABLE); //�ر�DMA,��ֹ�������������		
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���DMA2_Steam7������ɱ�־
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



