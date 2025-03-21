#include "timing_trigger.h"
#include "systick.h"


//定时器初始化
void timing_trigger_init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_timeBaseStucture;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_timeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;       
	TIM_timeBaseStucture.TIM_Prescaler = 72-1;                	 
	TIM_timeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;	 
	TIM_timeBaseStucture.TIM_Period = 20000-1;		                //20ms周期			
	TIM_TimeBaseInit(TIM4,&TIM_timeBaseStucture);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 5000;					        //5ms触发	     
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);							 
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);			 
	
	TIM_Cmd(TIM4,ENABLE);	    
}





