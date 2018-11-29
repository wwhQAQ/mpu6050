#include "usart.h"






void usart_init(u32 bound)
{
	USART_InitTypeDef USART_InitTypeInstructure;
	GPIO_InitTypeDef GPIO_InitTypeInstructure;
	NVIC_InitTypeDef NVIC_InitTypeInstructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);				//1.使能GPIOA与串口1的时钟；
	
	USART_DeInit(USART1);																	//2.串口1复位;
	
	USART_InitTypeInstructure.USART_BaudRate=bound;
	USART_InitTypeInstructure.USART_WordLength=USART_WordLength_8b;							//8位数据
	USART_InitTypeInstructure.USART_StopBits=USART_StopBits_1;								//1位停止位
	USART_InitTypeInstructure.USART_Parity=USART_Parity_No;									//无校验位；
	USART_InitTypeInstructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//串口模式-收发模式；
	USART_InitTypeInstructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//无流控制位
	USART_Init(USART1,&USART_InitTypeInstructure);											//3.串口初始化		
	
	GPIO_InitTypeInstructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeInstructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitTypeInstructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitTypeInstructure);												//初始化GPIOA9为复用推挽输出，50MHZ
	
	GPIO_InitTypeInstructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeInstructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitTypeInstructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitTypeInstructure);												//初始化GPIOA10为浮空输入，50MHZ
	
	NVIC_InitTypeInstructure.NVIC_IRQChannel=USART1_IRQn;									//选择要配置的中断频道
	NVIC_InitTypeInstructure.NVIC_IRQChannelCmd=ENABLE;										//中断使能;
	NVIC_InitTypeInstructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitTypeInstructure.NVIC_IRQChannelSubPriority=3;									//优先级设定，抢占优先级3，响应优先级3；
	NVIC_Init(&NVIC_InitTypeInstructure);													//中断配置函数
	
	USART_Cmd(USART1,ENABLE);																//串口使能；
	
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);											//打开串口接收完成中断；
}




void USART1_IRQHandler(void)
{
	USART_SendData(USART1,USART_ReceiveData(USART1));
}

void usart1_send(u8 xx)
{
	USART_SendData(USART1,xx);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
}

