#include "systick.h"

/* 系统滴答定时器 */

uint32_t tick_count;

void systick_init(void)
{
	SysTick->LOAD  = (uint32_t)(SystemCoreClock/1000000 - 1UL);
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk;  //配置滴答定时器时钟源和启动定时中断
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                              //失能滴答定时器中断中断
}
//us延时
void delay_us(uint32_t time)
{
	if(time<=0)
		return;

	tick_count = time;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                               //使能滴答定时器中断
	while(tick_count!=0);                                                   //等待计时完成
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                              //失能滴答定时器中断中断
}
//ms延时
void delay_ms(uint32_t time)
{
	if(time<=0)
		return;

	tick_count = time*1000;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                               //使能滴答定时器中断
	while(tick_count!=0);                                                   //等待计时完成
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                              //失能滴答定时器中断中断
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    if(tick_count!=0)
    {
        tick_count--;
    }
}


