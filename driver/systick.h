#ifndef _systick_h_
#define _systick_h_

#include "stm32f10x.h"

void systick_init(void);

void delay_us(uint32_t time);

void delay_ms(uint32_t time);


#endif

