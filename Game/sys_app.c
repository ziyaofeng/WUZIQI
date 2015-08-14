#include<stc12c5a60s2.h>
#include "w25qxx.h" 
#include "type_51.h"
#include "Touch.h"
#include <string.h>
#include "type_51.h"
void alert(char *s,u16 bcolor)
{
	u8 len;
	Show_Str(0,0,s,16,Red,bcolor);
	delay_ms(0x7ff);
	len=(u8)strlen(s)*8;
	LCD_DrawBox(0,0,len,16,bcolor);
}
void Get_Click(u16 *x,u16 *y)
{
	TP_Read_XY2(x,y); 
	//TP_Draw_Big_Point(*x=*x/Touch_X,*y=*y/Touch_Y,Red);
	*x=*x/14.03-14.85;*y=*y/10.81-17.52;
}
void delay_ms(u16 ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1000; j++ );
	}
} 
/*********************
导航栏构造
**********************/
void guidshow()
{
	LCD_DrawLine( 0, 288, 240, 288,White );
	Show_Str(20,288,"←",32,White,Nocolor);
	Show_Str(104,288,"█",32,0xa254,Nocolor);
	LCD_DrawLine( 108, 304, 131,304, White );
	LCD_DrawLine( 120, 293, 120,315, White );
	LCD_DrawLine( 80, 288, 80,320, White );
	LCD_DrawLine( 160, 288, 160,320, White );
	Show_Str(187,288,"〓",32,White,Nocolor);
}
/*********************
导航栏执行
参数:pback 返回键函数指针
pmenu 菜单键函数指针
**********************/
void guiddo(void (*pback)(void),void (*pmenu)(void),x)
{
	if(x<80&&x>0) {Show_Str(20,288,"←",32,White,Blue);pback();}
	if(x>80&&x<160) {Show_Str(104,288,"█",32,0xa254,Blue);LCD_DrawLine( 108, 304, 131,304, White );
		LCD_DrawLine( 120, 293, 120,315, White );LCD_DrawLine( 80, 288, 80,320, White );desktop();}
	if(x>160&&x<240){ Show_Str(187,288,"〓",32,White,Blue);pmenu();}
}