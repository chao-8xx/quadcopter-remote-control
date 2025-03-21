#ifndef _iic_h_
#define _iic_h_

#include "stm32f10x.h"

#define  IIC_RCC_port   RCC_APB2Periph_GPIOB

#define  SCL_port   GPIOB
#define  SDA_port   GPIOB

#define  SCL_pin    GPIO_Pin_13
#define  SDA_pin    GPIO_Pin_14

#define SCL_HIGH          SCL_port->BSRR = SCL_pin
#define SCL_LOW           SCL_port->BRR  = SCL_pin

#define SDA_HIGH          SDA_port->BSRR = SDA_pin 
#define SDA_LOW           SDA_port->BRR  = SDA_pin

#define SDA_READ        ((SDA_port->IDR &  SDA_pin)!=0) ? 1 : 0

void I2cInit(void); 

void SDA_OUT(void);
void SDA_IN(void);

void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Slave_Ack(void);
void IIC_Send_Byte(unsigned char byte);
unsigned char IIC_Read_Byte(void);

void IIC_Write_One_Byte(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);

unsigned char IIC_Read_One_Byte(unsigned char SlaveAddress,unsigned char REG_Address);
unsigned short int IIC_Read_Two_Bytes(unsigned char SlaveAddress,unsigned char REG_Address);


#endif


