#include "adc.h"
#include "led.h"
#include "usart1.h"
#include "nrf24l01.h"

uint16_t ADC_value[5];
float remoteVoltage;

/* 端口配置初始化 */
void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_initStructure;    
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	    
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_AIN;								    
	GPIO_Init(GPIOA,&GPIO_initStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_1;	    
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_AIN;								    
	GPIO_Init(GPIOB,&GPIO_initStructure);	
}

//遥感数据采集
/* adc配置 */ 
void adc_config(void)
{
    ADC_InitTypeDef ADC_initStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADC_initStructure.ADC_ContinuousConvMode = DISABLE;					        //单次转换模式，每次由定时器4通道4触发采样开始		
	ADC_initStructure.ADC_DataAlign = ADC_DataAlign_Right;		                //数据右对齐				
	ADC_initStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;	    //外部定时器4通道4定时触发	
	ADC_initStructure.ADC_Mode = ADC_Mode_Independent;							
	ADC_initStructure.ADC_NbrOfChannel = 5;										
	ADC_initStructure.ADC_ScanConvMode = ENABLE;		                        //扫描模式						
	ADC_Init(ADC1,&ADC_initStructure);

	ADC_Cmd(ADC1,ENABLE);
    
	ADC_DMACmd(ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);		//ADC时钟分频											
                                                                                
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_71Cycles5);//通道配置，采样时间设置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_71Cycles5);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,5,ADC_SampleTime_239Cycles5);//采集电压
	
	ADC_ResetCalibration(ADC1);	                                                //复位校准				
	while(ADC_GetCalibrationStatus(ADC1));		                                //等待
	ADC_StartCalibration(ADC1);					                                //启动校准
	while(ADC_GetCalibrationStatus(ADC1));		                                //等待校准完成

	ADC_ExternalTrigConvCmd(ADC1,ENABLE);	                                    //外部触发使能
}

void ADC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_initStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_initStructure.DMA_BufferSize = 5;										
	DMA_initStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        //传输方向：外设->内存						
	DMA_initStructure.DMA_M2M = DMA_M2M_Disable;								
	DMA_initStructure.DMA_MemoryBaseAddr = (u32)ADC_value;				        
	DMA_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //DMA传输的内存数据大小：半字为单位			
	DMA_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址自增
	DMA_initStructure.DMA_Mode = DMA_Mode_Circular;								
	DMA_initStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);	            //外设地址			
	DMA_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA传输的外设数据大小：半字为单位
	DMA_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址不变
	DMA_initStructure.DMA_Priority = DMA_Priority_Medium;						
	DMA_Init(DMA1_Channel1,&DMA_initStructure);
    
  DMA_ClearITPendingBit(DMA1_IT_TC1);			    

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);	
    
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}


void ADC_Config(void)
{
    adc_gpio_init();
    adc_config();
    ADC_DMA_Config();
}


void RemoteVoltageDetect(void)
{
	static float temp = 0;
	float temp2 = 0;
	
	remoteVoltage = 2.0f*(ADC_value[4]*3.3f/4096.0f);

	temp2 = temp - remoteVoltage;
	if(temp2 >= -0.03f && temp2 <= 0.03f){
		remoteVoltage = temp;
	} else {
		temp = remoteVoltage;
	}

}







