#ifndef _adc_h_
#define _adc_h_

#include "stm32f10x.h"



void adc_gpio_init(void);
void adc_config(void);
void ADC_DMA_Config(void);
void ADC_Config(void);


extern uint16_t ADC_value[5];

void RemoteVoltageDetect(void);
#endif


