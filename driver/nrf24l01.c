#include "nrf24l01.h"
#include "spi.h"
#include "systick.h"
#include "pair_freq.h"
#include "oled.h"
#include "sendpacket.h"
extern Pair pair;

//初始化24L01的IO口
void NRF24L01_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //CE    
    RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);            //使能GPIO的时钟
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE;                  //NRF24L01 模块片选信号
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_NRF24L01_CE, &GPIO_InitStructure);

    //CSN
    RCC_APB2PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);           //使能GPIO的时钟
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN;      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_NRF24L01_CSN, &GPIO_InitStructure);
	
    Set_NRF24L01_CE();                                          //初始化时先拉高
    Set_NRF24L01_CSN();                                         //初始化时先拉高

    //IRQ
    RCC_APB2PeriphClockCmd(RCC_NRF24L01_IRQ, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;             //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_NRF24L01_IRQ, &GPIO_InitStructure);
 
    Clr_NRF24L01_CE(); 	                                        //使能24L01
    Set_NRF24L01_CSN();                                         //SPI片选取消	 
}
//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，相同时返回值:0，表示在位;否则返回1，表示不在位	
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t buf1[5];
	uint8_t i; 

	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);            //写入5个字节的地址.
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);                          //读出写入的地址 
	for(i=0;i<5;i++)
	{
	    if(buf1[i]!=0xA5)
		{
		    break;					   
		}
	}
	if(i!=5)
        return 1;                                               //NRF24L01不在位
	
	return 0;		                                            //NRF24L01在位
}	 	 

//通过SPI写寄存器
uint8_t NRF24L01_Write_Reg(uint8_t regaddr,uint8_t data)
{
    uint8_t status;
        
    Clr_NRF24L01_CSN();                                         //使能SPI传输
                                                                
    status =SPI1_RW(regaddr);                                   //发送寄存器号 
    SPI1_RW(data);                                              //写入寄存器的值
	                                                            
    Set_NRF24L01_CSN();                                         //禁止SPI传输
                                                                
    return(status);       		                                //返回状态值
}
//读取SPI寄存器值 ，regaddr:要读的寄存器
uint8_t NRF24L01_Read_Reg(uint8_t regaddr)
{
	uint8_t reg_val;
 	Clr_NRF24L01_CSN();                                         //使能SPI传输	
	                                                            
  	SPI1_RW(regaddr);                                           //发送寄存器号
  	reg_val=SPI1_RW(0XFF);                                      //读取寄存器内容
                                                                
  	Set_NRF24L01_CSN();                                         //禁止SPI传输	
                                                                
  	return(reg_val);                                            //返回状态值
}	


//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值 
uint8_t NRF24L01_Read_Buf(uint8_t regaddr,uint8_t *pBuf,uint8_t datalen)
{
	uint8_t status,ctr;
  	Clr_NRF24L01_CSN();                                         //使能SPI传输
                                                                
  	status=SPI1_RW(regaddr);                                    //发送寄存器值(位置),并读取状态值   	
 	for(ctr=0;ctr<datalen;ctr++)                                
          pBuf[ctr]=SPI1_RW(0XFF);                              //读出数据
  	Set_NRF24L01_CSN();                                         //关闭SPI传输
                                                                
  	return status;                                              //返回读到的状态值
}

//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
uint8_t NRF24L01_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen)
{
	uint8_t status,ctr;	
 	Clr_NRF24L01_CSN();                                         //使能SPI传输
                                                                
  	status = SPI1_RW(regaddr);                                  //发送寄存器值(位置),并读取状态值
  	for(ctr=0; ctr<datalen; ctr++)                              
          SPI1_RW(*pBuf++);                                     //写入数据	 
                                                                
  	Set_NRF24L01_CSN();                                         //关闭SPI传输
                                                                
  	return status;                                              //返回读到的状态值
}				   
//启动NRF24L01发送一次数据
//sendBuff:待发送数据首地址
//返回值:发送完成状况
uint8_t NRF24L01_TxPacket(uint8_t *sendBuff)
{
  uint8_t state;   
	Clr_NRF24L01_CE();
    
  //NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)pair.addr,RX_ADR_WIDTH);
	NRF24L01_Write_Buf(WR_TX_PLOAD,sendBuff,TX_PLOAD_WIDTH);
    
 	Set_NRF24L01_CE();		//启动发送
    
	while(READ_NRF24L01_IRQ()!=0);		//等待发送完成
    
	state=NRF24L01_Read_Reg(SPI_READ_REG+STATUS);		//读取状态寄存器的值	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);		//清除TX_DS或MAX_RT中断标志
    
	NrfRxPacket();		//接收带负载的ACK数据
	
	if(state&MAX_TX){		//达到最大重发次数	
    NRF24L01_Write_Reg(FLUSH_TX,0xff);		//清除TX FIFO寄存器 	
		return MAX_TX; 
	}
	if(state&TX_OK){		//发送完成
		return TX_OK;
	}
	return 0xff;		//其他原因发送失败
}

				 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	Clr_NRF24L01_CE();		 
	
	NRF24L01_Write_Reg(SETUP_AW, 0x03); // 设置地址宽度为 5bytes

	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(uint8_t*)pair.addr,TX_ADR_WIDTH);    //写TX节点地址 
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)pair.addr,RX_ADR_WIDTH); //设置TX节点地址,主要为了接收ACK	  

	NRF24L01_Write_Reg(SPI_WRITE_REG+FEATURE, 0x06 );//使能动态负载长度及带负载的ACK应答
	NRF24L01_Write_Reg(SPI_WRITE_REG+DYNPD, 0x01); //使能接收管道0动态负载长度

	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);               //使能通道0的自动应答    
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);           //使能通道0的接收地址  
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,pair.freq_channel);  //设置RF通道
	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);          //设置自动重发间隔时间:500us;最大自动重发次数:10次
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);						//设置射频数据率为1MHZ，发射功率为7dBm
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);              //配置基本工作模式的参数;开启CRC，配置为发射模式,开启所有中断
     
	Set_NRF24L01_CE();                                          //CE为高,10us后启动发送
}		  

