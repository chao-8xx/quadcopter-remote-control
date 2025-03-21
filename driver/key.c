#include "key.h"
#include "systick.h"


_KEY key = {0};

void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    RCC_APB2PeriphClockCmd(key_left_rcc, ENABLE);  
    RCC_APB2PeriphClockCmd(key_right_rcc, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = key_left_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key_left_port, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = key_right_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key_right_port, &GPIO_InitStructure); 
}

//按键扫描
//0：不支持连续按
//1：支持连续按
uint8_t KeyScan(uint8_t mode)
{
    static uint8_t key_up = 1;
    
    if(mode){
       key_up = 1;
		}
    if(key_up&&(key_left==0||key_right==0)){
        delay_ms(5);
        key_up = 0;
        if(key_left==0)     return 1;
        if(key_right==0)    return 2;
    }
    else if(key_left==1&&key_right==1){
        key_up = 1;
    }
    return 0;
}



void key_info(void)
{
    key.value = KeyScan(1);
	if(key.value==1)	key.l = 0xE1;
	if(key.value==2)	key.r = 0xC8;
	
	if(!key.value) 
	{
		key.l = 0;
		key.r = 0;
	}
}












