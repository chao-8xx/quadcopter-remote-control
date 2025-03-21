#ifndef _usart3_h_
#define _usart3_h_


#include "stm32f10x.h"



void usart3_init(u32 bound);



void USART3_DMA_SEND_DATA(u32 SendBuff,u16 len);










#endif

