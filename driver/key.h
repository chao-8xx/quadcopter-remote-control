#ifndef _key_h_
#define _key_h_

#include "stm32f10x.h"


typedef struct
{
    uint8_t value;
    uint8_t l;
    uint8_t r;
    uint16_t l_num;
    uint16_t r_num;
}_KEY;


//时钟宏定义区
#define key_left_rcc     RCC_APB2Periph_GPIOC   
#define key_right_rcc    RCC_APB2Periph_GPIOB  
//端口宏定义区
#define key_left_port    GPIOC
#define key_right_port   GPIOB
//引脚宏定义区
#define key_left_pin     GPIO_Pin_14
#define key_right_pin    GPIO_Pin_12

#define key_left   GPIO_ReadInputDataBit(key_left_port,key_left_pin)
#define key_right  GPIO_ReadInputDataBit(key_right_port,key_right_pin)


extern _KEY key;

void KeyInit(void);
uint8_t KeyScan(uint8_t mode);
void key_info(void);

#endif

