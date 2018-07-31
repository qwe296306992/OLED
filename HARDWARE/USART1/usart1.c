#include "usart1.h"
#include "gizwits_product.h"

//wifi模块用



//串口1中断服务函数
void USART1_IRQHandler(void)
{
	uint8_t res;	      
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	
//		USART_GetFlagStatus(USART1, USART_IT_RXNE);		
		res =USART_ReceiveData(USART1);		 
		gizPutData(&res, 1);//数据写入到缓冲区
	}  				 											 
}

void usart1_init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
   USART_DeInit(USART1);      //复位串口3
	//USART1_TX   PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART1_RX	  PA10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;	
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);       //初始化串口3
	
	USART_Cmd(USART1, ENABLE);                      //使能串口 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //开启中断
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}


//串口3中断服务函数
//void USART1_IRQHandler(void)
//{
//	uint8_t res;	      
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
//	{	
////		USART_GetFlagStatus(USART1, USART_IT_RXNE);		
//		res =USART_ReceiveData(USART1);		 
//		gizPutData(&res, 1);//数据写入到缓冲区
//	}  				 											 
//}
