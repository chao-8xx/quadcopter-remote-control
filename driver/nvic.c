
#include "nvic.h"


void NVIC_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;    

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //优先级组别2

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_Init(&NVIC_InitStructure);                                         
}







