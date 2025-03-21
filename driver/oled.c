#include "oled.h"
#include "systick.h"
#include "iic.h"
#include "led.h"



/*******************************************************************************
* 函 数 名         : OLED_Init()
* 函数功能		     : OLED初始化
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : Write_OLED_Command()
* 函数功能		     : OLED写命令
* 输    入         : OLED_Command(命令)
* 输    出         : 无
*******************************************************************************/
void Write_OLED_Command(unsigned char OLED_Command)
{
  IIC_Start();
  IIC_Send_Byte(0x78);       //Slave address,SA0=0
	IIC_Slave_Ack();	
  IIC_Send_Byte(0x00);			//D/C#=0;写命令
	IIC_Slave_Ack();	
  IIC_Send_Byte(OLED_Command); 
	IIC_Slave_Ack();	
  IIC_Stop();
}
//
/*******************************************************************************
* 函 数 名         : Write_OLED_Data()
* 函数功能		     : OLED写数据
* 输    入         : OLED_Data(数据)
* 输    出         : 无
*******************************************************************************/
void Write_OLED_Data(unsigned char OLED_Data)
{
  IIC_Start();
  IIC_Send_Byte(0x78);			// R/W#=0;读写地址及模式选择，这里SA=0，R/W#=0（写模式）
	IIC_Slave_Ack();	
  IIC_Send_Byte(0x40);			//D/C#=1;写数据
	IIC_Slave_Ack();	
  IIC_Send_Byte(OLED_Data);//写入数据
	IIC_Slave_Ack();	
  IIC_Stop();
}




/*******************************************************************************
* 函 数 名         : OLED_WR_Byte()
* 函数功能		     : OLED命令/数据模式选择
* 输    入         : Data(数据)
										 DataType(数据类型：数据/命令)
* 输    出         : 无
*******************************************************************************/
void OLED_WR_Byte(unsigned Data,unsigned DataType)
{
	if(DataType)
	{
   Write_OLED_Data(Data);//写数据，cmd=0

	}
	else 
	{
   Write_OLED_Command(Data);	//写命令，cmd=1
	}
}



/*******************************************************************************
* 函 数 名         : OLED_Display_On()
* 函数功能		     : 开启OLED显示 
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


/*******************************************************************************
* 函 数 名         : OLED_Display_Off()
* 函数功能		     : 关闭OLED显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   	


/*******************************************************************************
* 函 数 名         : OLED_Clear()
* 函数功能		     : 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}



/*******************************************************************************
* 函 数 名         : OLED_On()
* 函数功能		     : oled满屏显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0xff,OLED_DATA); 
	} //更新显示
}



//显示中文(双行显示)
//row：0~7
//column：0~7
//strChinese：中文数组
//num：1~16
void OledDisplayChinese(u8 row,u8 column,const u8 strChinese[][32],u8 num)
{  
	u8 temp,arrRaw,arrColumn;
	u8 page;
	u8 test = 0,testCount = 0,arrRawTest = 0;
	
	if((num + column) >=8 ){	//判断一行是否能显示完
		test = 8 - column;
	} else {
		test = num;
	}
	
	for(page = row; page < 2+row; page++){ //1页等于一行，一循环刷新一行
		OLED_WR_Byte (0xb0 + page,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10 + column,OLED_CMD);      //设置显示位置—列高地址 
		
		for(arrRaw = arrRawTest; arrRaw < test; arrRaw++){	//一循环刷新一个字
			for(arrColumn = page - row; arrColumn < 32; arrColumn += 2){ //一个中文字有32字节，占两行，一循环刷新一行
				temp = strChinese[arrRaw][arrColumn];
				OLED_WR_Byte(temp,OLED_DATA);
		  }
	  }
		
		testCount++;
		if((testCount == 2) && ((num + column) >=8)){	//一行显示不完切换到下一行
			row += 2;
			test = num ;
			arrRawTest = arrRaw;
		}
		
	}
	
}


//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   

//在指定位置显示一个字符,包括部分字符
//x:0~15
//y:0~6
//size:选择字体 16/12 
void OLED_ShowChar(u8 row,u8 column,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值		
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

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//显示数字
//row：行数（0~6）
//column：列数（0~15）
//len :数字的位数(1~16)
//num:数值(0~4294967295) 
//size:字体大小(8或16)
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

//显示字符串
//row：行数（0~6）
//column：列数（0~15）
//*chr :字符串指针 
//size:字体大小(8或16)
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
