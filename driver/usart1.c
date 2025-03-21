#include "usart1.h"

#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
    
void _sys_exit(int x) 
{ 
	x = x; 
}
    
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
    {;}
		USART_SendData(USART1,(uint8_t)ch);
	return ch;
}

//上外上位机串口初始化
void Usart1Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);

    //  TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //  RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //stop bit is 1
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //no Hardware Flow Control
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //enable tx and rx
	USART_Init(USART1, &USART_InitStructure);
    
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                    //rx interrupt is enable
    
	USART_Cmd(USART1, ENABLE);    
}


