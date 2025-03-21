#include "usart3.h"




//上外上位机串口初始化
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //  TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    //  RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //stop bit is 1
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //no Hardware Flow Control
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //enable tx and rx
	USART_Init(USART3, &USART_InitStructure);
    
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);                                    //rx interrupt is enable
    
	USART_Cmd(USART3, ENABLE);    
    
}

u16 USART3_DMA1_MEM_LEN;   


/* 串口3的DMA通道配置                */
/* 存储器到外设传输方向              */
/* DMA_CHx:         DMA传输通道x     */
/* peripheral_addr: 外设地址         */
/* memory_addr:     内存地址         */
/* data_length:     传输的数据长度   */  
void USART3_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 peripheral_addr,u32 memory_addr,u16 data_length)
{                  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                        //时钟使能   
    
    DMA_DeInit(DMA_CHx);                                                        //复位
                                                                                
    USART3_DMA1_MEM_LEN = data_length;                                          //传输的数据长度  
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = peripheral_addr;                 //外设地址     
    DMA_InitStructure.DMA_MemoryBaseAddr =memory_addr;                          //内存地址  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                          //外设作为传输的目的地
    DMA_InitStructure.DMA_BufferSize = data_length;                             //数据缓存大小                       
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址不自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址自增   
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //外设数据宽度8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //内存数据宽度8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     //高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //无内存到内存传输                    
    DMA_Init(DMA_CHx, &DMA_InitStructure);                                 
}
/* 开启1次DMA传输 */
void USART3_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx) 
{
 	DMA_Cmd(DMA_CHx, ENABLE);                                                   //使能传输
}
/* 串口3DMA数据发送 */
void USART3_DMA_SEND_DATA(u32 SendBuff,u16 len) 
{
	USART3_DMA_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)SendBuff,len);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);                              //使能串口DMA发送
	USART3_DMA_Enable(DMA1_Channel2);
}












