#ifndef _oled_h_
#define _oled_h_

#include "stm32f10x.h"
#include "OledFont.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	


#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý



void OLED_Init(void); 
void OLED_WR_Byte(unsigned Data,unsigned DataType);
void Write_OLED_Command(unsigned char IIC_Command);
void Write_OLED_Data(unsigned char IIC_Data);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_On(void);
void OledDisplayString(u8 row,u8 column,u8 str[]);
void OledDisplayChinese(u8 row,u8 column,const u8 strChinese[][32],u8 num);
void OLED_ShowNum(u8 row,u8 column,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 row,u8 column,u8 *chr,u8 size);
void OLED_ShowChar(u8 row,u8 column,u8 chr,u8 Char_Size);

#endif


