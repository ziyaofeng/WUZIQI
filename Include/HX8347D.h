#ifndef HX8347D_H 
#define HX8347D_H

/* Includes ------------------------------------------------------------------*/
#include "stc12c5a60s2.h"
#include "w25qxx.h" 
#include "type_51.h"
#include <stdlib.h>


sbit	LCD_CS		= P1^4;  //片选端口  	    
sbit	LCD_RS		= P1^5;   //数据/命令        
sbit	LCD_WR		= P1^6;   //写数据			    
sbit	LCD_RD		= P1^7;   //读数据			 		

//数据线
#define DATA_Port P3 //数据

/* Private define ------------------------------------------------------------*/
#define DISP_ORIENTATION  0  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320  

#endif

/* LCD Registers */
#define R34            0x22

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xc618
#define Blue           0x001F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Nocolor        0x1111

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24位转换16位
* Input          : - red: R
*                  - green: G 
*				   - blue: B
* Output         : None
* Return         : RGB 颜色值
* Attention		 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void LCD_DrawBox(u16 x,u16 y,u16 width,u16 high,uint16_t color);
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void show_picture(u16 x,u16 y,u16 width,u16 high,u32 ReadAddr);
void LCD_DrawEBox(u16 x,u16 y,u16 width,u16 high,uint16_t color);
#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
