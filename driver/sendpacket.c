#include "sendpacket.h"
#include "timing_trigger.h"
#include "usart1.h"
#include "adc.h"
#include "nrf24l01.h"
#include "key.h"
#include "led.h"
#include "pair_freq.h"
#include "oled.h"
#include "compile.h"

RemoteData tx = {0};
extern Pair pair;
extern float remoteVoltage;

static u8 rxPacket[TX_PLOAD_WIDTH] = {0};
u8 rxPacketStatus;



void analyze_packet(uint16_t *adcData)
{
#if FOUR_AXIS_UAV
		//油门采集的数据量化为 0 ~ 1000
		//舵面采集的数据量化为 0 ~ 100
    tx.thr = (uint16_t)(((float)(adcData[1]))*0.24420f);
    tx.yaw = (uint8_t)(((float)(adcData[0]))*0.02442f);
    tx.pit = (uint8_t)(((float)(adcData[3]))*0.02442f);
    tx.rol = (uint8_t)(((float)(adcData[2]))*0.02442f);
#elif FIXED_WING_AIRCRAFT
    tx.thr = (uint16_t)(((float)(adcData[1]))*0.24420f);
    tx.yaw = (uint8_t)(((float)(adcData[0]))*0.02442f);
    tx.pit = (uint8_t)(((float)(adcData[3]))*0.02442f);
    tx.rol = (uint8_t)(((float)(adcData[2]))*0.02442f);
#elif BRUSHLESS_FOUR_AXIS_UAV
		//油门采集的数据量化为 4000 ~ 8000
		//舵面采集的数据量化为 0 ~ 400
    tx.thr = (uint16_t)(((float)(adcData[1]))*0.9765625f + 4000);//4000~8000
    tx.yaw = (uint16_t)(((float)(adcData[0]))*0.09765625f);
    tx.pit = (uint16_t)(((float)(adcData[3]))*0.09765625f);
    tx.rol = (uint16_t)(((float)(adcData[2]))*0.09765625f);
#endif
}

void data_exchange(uint8_t *dateBuff)
{
	//发送新的地址和频点给飞机
	if(pair.step == STEP1){
			dateBuff[0] = 0xA8;					//帧头
			dateBuff[1] = pair.addr[0];		  //5个字节地址    
			dateBuff[2] = pair.addr[1];	    //    
			dateBuff[3] = pair.addr[2];			//	
			dateBuff[4] = pair.addr[3];			//	
			dateBuff[5] = pair.addr[4];			//	
			
			dateBuff[6] = pair.freq_channel;		//通信频点		


			dateBuff[TX_PLOAD_WIDTH-1] = 0x8B;	//帧尾      
	}
	//正常数据通信
	else if(pair.step == DONE){
		#if FOUR_AXIS_UAV
			dateBuff[0] = 0xA8;		//帧头
			dateBuff[1] = *((u8*)&tx.thr);		//油门低八位
			dateBuff[2] = *(((u8*)&tx.thr)+1);		//油门高八位
			dateBuff[3] = tx.pit;		//俯仰舵向
			dateBuff[4] = tx.rol;		//横滚舵向
			dateBuff[5] = tx.yaw;		//偏航舵向
			dateBuff[6] = key.l;		//左功能按键
			dateBuff[7] = key.r;		//右功能按键
			dateBuff[8] = 0;		//未使用，可自行添加
			dateBuff[9] = 0;		//未使用，可自行添加
			dateBuff[TX_PLOAD_WIDTH-1] = 0x8B;		//帧尾 
		#elif FIXED_WING_AIRCRAFT
			dateBuff[0] = 0xA8;		//帧头
			dateBuff[1] = *((u8*)&tx.thr);		//油门低八位
			dateBuff[2] = *(((u8*)&tx.thr)+1);		//油门高八位
			dateBuff[3] = tx.pit;		//俯仰舵向
			dateBuff[4] = tx.rol;		//横滚舵向
			dateBuff[5] = tx.yaw;		//偏航舵向
			dateBuff[6] = key.l;		//左功能按键
			dateBuff[7] = key.r;		//右功能按键
			dateBuff[8] = 0;		//未使用，可自行添加
			dateBuff[9] = 0;		//未使用，可自行添加
			dateBuff[TX_PLOAD_WIDTH-1] = 0x8B;		//帧尾 
		#elif BRUSHLESS_FOUR_AXIS_UAV
			dateBuff[0] = 0xA8;		//帧头
			dateBuff[1] = *((u8*)&tx.thr);		//油门低八位
			dateBuff[2] = *(((u8*)&tx.thr)+1);		//油门高八位
			dateBuff[3] = *((u8*)&tx.pit);		//俯仰舵向
			dateBuff[4] = *(((u8*)&tx.pit)+1);		//俯仰舵向
			dateBuff[5] = *((u8*)&tx.rol);		//横滚舵向
			dateBuff[6] = *((u8*)&tx.rol+1);		//横滚舵向
			dateBuff[7] = *((u8*)&tx.yaw);		//偏航舵向
			dateBuff[8] = *((u8*)&tx.yaw+1);		//偏航舵向
			dateBuff[9] = key.l;		//左功能按键
			dateBuff[10] = key.r;		//右功能按键
			dateBuff[11] = 0;		//未使用，可自行添加
			dateBuff[12] = 0;		//未使用，可自行添加
			dateBuff[TX_PLOAD_WIDTH-1] = 0x8B;		//帧尾 
		#endif     
	}
}

uint8_t tx_dat[TX_PLOAD_WIDTH] = {0};


//DMA通道1中断触发，如果触发，则表明DMA已经将ADC数据搬运到内存数组中，则可以进行发送数据了    
void DMA1_Channel1_IRQHandler(void)
{
	uint8_t ret=0;
	
	if(DMA_GetITStatus(DMA1_IT_TC1) == SET){	
		analyze_packet(ADC_value);		
		data_exchange(tx_dat);//数据装入发送数组

		ret=NRF24L01_TxPacket(tx_dat);//发送数据
		if(ret == TX_OK){
			LedColorSet(GREEN);		//发送成功绿灯亮
		} else{
			LedBlink(YELLOW);		//信号丢失黄灯闪烁
		}
		
		DMA_ClearITPendingBit(DMA1_IT_TC1);		
		
	}
}

//接收数据包
void NrfRxPacket(void)
{
	NRF24L01_Read_Buf(RD_RX_PLOAD,rxPacket,11);
	if(rxPacket[0] == 0XAA && rxPacket[TX_PLOAD_WIDTH - 1] == 0XAC){//检查包头包尾
		rxPacketStatus = 1;  
	} else {
		rxPacketStatus = 0;
	}
	
}

void DisplayPlaneInfo(void)
{
	static u8 clear = 0;
	uint16_t voltage = 0;
	
	if(clear != ((rxPacket[1]<<4)|rxPacket[2])){
		OLED_Clear();
		clear = (rxPacket[1]<<4)|rxPacket[2];
	}
		
	if(rxPacket[1] == 0){		
		OledDisplayChinese(2,3,lock,2);//锁定
	}
	else if(rxPacket[1] == 1){		
		OledDisplayChinese(2,3,unlock,2);//解锁
	}
	
	if(rxPacket[2] == 1){		
		OledDisplayChinese(6,5,powerLow,3);//电量低
	} 
	
		//遥控电压显示
	OledDisplayChinese(4,0,remote,2);
	OLED_ShowChar(4,4,':',16);
	voltage = (uint16_t)(remoteVoltage * 100);
	OLED_ShowNum(4,5,(u32)(voltage/100),1,16);
	OLED_ShowChar(4,6,'.',16);
	
	if((voltage%100) < 10){
		OLED_ShowNum(4,7,(u32)(0),1,16);
		OLED_ShowNum(4,8,(u32)(voltage%10),1,16);
	} else {
		OLED_ShowNum(4,7,(u32)(voltage%100),2,16);
	}
	OLED_ShowChar(4,9,'V',16);
	
	//飞机电压显示
	OledDisplayChinese(6,0,planeVoltage,2);
	OLED_ShowChar(6,4,':',16);
	voltage = rxPacket[4]<<8|rxPacket[3];
	OLED_ShowNum(6,5,(u32)(voltage/100),1,16);
	OLED_ShowChar(6,6,'.',16);
	
	if((voltage%100) < 10){
		OLED_ShowNum(6,7,(u32)(0),1,16);
		OLED_ShowNum(6,8,(u32)(voltage%10),1,16);
	} else {
		OLED_ShowNum(6,7,(u32)(voltage%100),2,16);
	}
	OLED_ShowChar(6,9,'V',16);
	
	
}

//屏幕显示
void OledDisplayPairStatus(void)
{
	static u8 clearDetect = 0;

	if(clearDetect != ((rxPacketStatus<<4)|pair.step)){
		OLED_Clear();
		clearDetect = (rxPacketStatus<<4)|pair.step;
	}
	
	if(rxPacketStatus == 1 && pair.step == NOT){		
		OledDisplayChinese(0,2,pairNot,3);//未对频
		OledDisplayChinese(3,0,pairTips1,9);//左遥感上推进行对频
	}
	else if(pair.step == STEP1){
		OledDisplayChinese(0,2,pairing,3);//对频中
		OledDisplayChinese(3,0,pairTips2,9);//左摇杆下拉完成对频
	}
	else if(rxPacketStatus == 1 && pair.step == DONE){
		OledDisplayChinese(0,2,pairDone,4);//对频完成
		DisplayPlaneInfo();
	}
	else if(rxPacketStatus == 0 && pair.step != DONE){		
		OledDisplayChinese(0,1,NoPlaneDetected,6);//未检测到飞机
		OledDisplayChinese(3,0,planeIfOn,16);
	}
	else if(rxPacketStatus == 0 && pair.step == DONE){		
		OledDisplayChinese(0,2,signalLost,4);//信号丢失
	}

	
}

