#include "led.h"

uint16_t blinkSpeedCount = 0;		//状态RGB灯闪烁速度设置
#define BLINK_SPEED 10

uint16_t blinkPeriodCount = 0;	//状态RGB灯闪烁周期设置
#define BLINK_PERIOD 5

/* led端口初始化 */
void LedInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
	//GPIOB->BSRR  = GPIO_Pin_7;//红
//	GPIOB->BSRR  = GPIO_Pin_8;//绿
//	GPIOB->BSRR  = GPIO_Pin_9;//蓝
		LedColorSet(RED);
}

//关闭状态指示灯
static void LedStatusOff(void)
{
	GPIOB->BSRR = GPIO_Pin_7;
	GPIOB->BSRR = GPIO_Pin_8;
	GPIOB->BSRR = GPIO_Pin_9;
}

//LED灯颜色设置
void LedColorSet(const uint8_t LedColor)
{
	LedStatusOff();
	
	switch(LedColor){
		case RED:
			RGB_RED;//红
			break;
		case GREEN:
			RGB_GREEN;//绿
			break;
		case BLUE:
			RGB_BLUE;//蓝
			break;
		case YELLOW:	//黄
			RGB_RED;
			RGB_GREEN;
			break;
		case PURPLE:	//紫
			RGB_RED;
			RGB_BLUE;
			break;
		case CYAN:	//青
			RGB_GREEN;
			RGB_BLUE;
			break;
		case WHITE:	//白
			RGB_RED;
			RGB_GREEN;
			RGB_BLUE;
			break;
		default :
			LedStatusOff();	//全灭
			break;
	}
}

void LedBlink(uint8_t ledColor)
{
	if( blinkSpeedCount < BLINK_SPEED ){
        blinkSpeedCount++;
		if( blinkSpeedCount == BLINK_SPEED ){
			blinkPeriodCount++;
			blinkSpeedCount=0;
		}
	}
	
	if( blinkPeriodCount >= BLINK_PERIOD - 2 && blinkPeriodCount <= BLINK_PERIOD){		//闪烁
		if(blinkPeriodCount == BLINK_PERIOD){
			blinkPeriodCount = 0;
		}
		
		LedColorSet(ledColor);
		
	}else {
		LedStatusOff();
	}
}

