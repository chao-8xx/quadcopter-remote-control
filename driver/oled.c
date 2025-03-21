#include "oled.h"
#include "systick.h"
#include "iic.h"
#include "led.h"



/*******************************************************************************
* �� �� ��         : OLED_Init()
* ��������		     : OLED��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OLED_Init(void)
{
	I2cInit(); 
	delay_ms(200);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address0
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address16
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address176
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0x08,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}



/*******************************************************************************
* �� �� ��         : Write_OLED_Command()
* ��������		     : OLEDд����
* ��    ��         : OLED_Command(����)
* ��    ��         : ��
*******************************************************************************/
void Write_OLED_Command(unsigned char OLED_Command)
{
  IIC_Start();
  IIC_Send_Byte(0x78);       //Slave address,SA0=0
	IIC_Slave_Ack();	
  IIC_Send_Byte(0x00);			//D/C#=0;д����
	IIC_Slave_Ack();	
  IIC_Send_Byte(OLED_Command); 
	IIC_Slave_Ack();	
  IIC_Stop();
}
//
/*******************************************************************************
* �� �� ��         : Write_OLED_Data()
* ��������		     : OLEDд����
* ��    ��         : OLED_Data(����)
* ��    ��         : ��
*******************************************************************************/
void Write_OLED_Data(unsigned char OLED_Data)
{
  IIC_Start();
  IIC_Send_Byte(0x78);			// R/W#=0;��д��ַ��ģʽѡ������SA=0��R/W#=0��дģʽ��
	IIC_Slave_Ack();	
  IIC_Send_Byte(0x40);			//D/C#=1;д����
	IIC_Slave_Ack();	
  IIC_Send_Byte(OLED_Data);//д������
	IIC_Slave_Ack();	
  IIC_Stop();
}




/*******************************************************************************
* �� �� ��         : OLED_WR_Byte()
* ��������		     : OLED����/����ģʽѡ��
* ��    ��         : Data(����)
										 DataType(�������ͣ�����/����)
* ��    ��         : ��
*******************************************************************************/
void OLED_WR_Byte(unsigned Data,unsigned DataType)
{
	if(DataType)
	{
   Write_OLED_Data(Data);//д���ݣ�cmd=0

	}
	else 
	{
   Write_OLED_Command(Data);	//д���cmd=1
	}
}



/*******************************************************************************
* �� �� ��         : OLED_Display_On()
* ��������		     : ����OLED��ʾ 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


/*******************************************************************************
* �� �� ��         : OLED_Display_Off()
* ��������		     : �ر�OLED��ʾ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   	


/*******************************************************************************
* �� �� ��         : OLED_Clear()
* ��������		     : ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}



/*******************************************************************************
* �� �� ��         : OLED_On()
* ��������		     : oled������ʾ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0xff,OLED_DATA); 
	} //������ʾ
}



//��ʾ����(˫����ʾ)
//row��0~7
//column��0~7
//strChinese����������
//num��1~16
void OledDisplayChinese(u8 row,u8 column,const u8 strChinese[][32],u8 num)
{  
	u8 temp,arrRaw,arrColumn;
	u8 page;
	u8 test = 0,testCount = 0,arrRawTest = 0;
	
	if((num + column) >=8 ){	//�ж�һ���Ƿ�����ʾ��
		test = 8 - column;
	} else {
		test = num;
	}
	
	for(page = row; page < 2+row; page++){ //1ҳ����һ�У�һѭ��ˢ��һ��
		OLED_WR_Byte (0xb0 + page,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10 + column,OLED_CMD);      //������ʾλ�á��иߵ�ַ 
		
		for(arrRaw = arrRawTest; arrRaw < test; arrRaw++){	//һѭ��ˢ��һ����
			for(arrColumn = page - row; arrColumn < 32; arrColumn += 2){ //һ����������32�ֽڣ�ռ���У�һѭ��ˢ��һ��
				temp = strChinese[arrRaw][arrColumn];
				OLED_WR_Byte(temp,OLED_DATA);
		  }
	  }
		
		testCount++;
		if((testCount == 2) && ((num + column) >=8)){	//һ����ʾ�����л�����һ��
			row += 2;
			test = num ;
			arrRawTest = arrRaw;
		}
		
	}
	
}


//��������
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~15
//y:0~6
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 row,u8 column,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ		
		column = column*8;
		if(column>Max_Column-1){column=0;row=row+2;}
		if(Char_Size ==16){
			OLED_Set_Pos(column,row);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(column,row+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		} else {	
				OLED_Set_Pos(column,row);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
			
}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//��ʾ����
//row��������0~6��
//column��������0~15��
//len :���ֵ�λ��(1~16)
//num:��ֵ(0~4294967295) 
//size:�����С(8��16)
void OLED_ShowNum(u8 row,u8 column,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;		
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
//			OLED_ShowChar(row+(size/2)*t,column,' ',size);
				OLED_ShowChar(row,column+t,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
//	 	OLED_ShowChar(row+(size/2)*t,column,temp+'0',size); 
			OLED_ShowChar(row,column+t,temp+'0',size);
	}
} 

//��ʾ�ַ���
//row��������0~6��
//column��������0~15��
//*chr :�ַ���ָ�� 
//size:�����С(8��16)
void OLED_ShowString(u8 row,u8 column,u8 *chr,u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(row,column,chr[j],size);
			row+=8;
		if(row>120){row=0;column+=2;}
			j++;
	}
}
