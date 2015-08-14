/* Includes ------------------------------------------------------------------*/
#include "HX8347D.h" 
#include "font.h"
#include "w25qxx.h"


/* Private variables ---------------------------------------------------------*/
//static uint8_t LCD_Code;
u16 LCD_id;


//写寄存器函数
void LCD_WR_REG(u8 dat)
{ 
	LCD_RS=0;//写地址  
 	LCD_CS=0; 
	DATA_Port = dat; 
	LCD_WR=0; 
	LCD_WR=1; 
 	LCD_CS=1;   
} 

void LCD_WR_DATA(u16 dat)
{
	LCD_RS=1;
	LCD_CS=0;
	DATA_Port = dat;
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
} 

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void LCD_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	
}


/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void LCD_WriteRAM_Prepare(void)
{
  LCD_WR_REG(R34);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void LCD_WriteRAM(uint16_t RGB_Code)					 
{
  /* Write 16-bit GRAM Reg */
  LCD_WR_DATA(RGB_Code);
}


/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )
	
 	uint16_t temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;  

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
	
	#endif

	LCD_WriteReg(0x02, Xpos>>8 );                                                  
	LCD_WriteReg(0x03, Xpos );  
	LCD_WriteReg(0x06, Ypos>>8 );                           
	LCD_WriteReg(0x07, Ypos );     
}

/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initializtion(void)
{
	uint16_t DeviceCode = 0x9325;
	
	
	delay_ms(50); // delay 50 ms 
 	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	

	DeviceCode = 0x0047;
	delay_ms(100);

	if( DeviceCode == 0x0047 )
	{


		/* Start Initial Sequence */
		LCD_WriteReg(0xEA,0x00);                          
		LCD_WriteReg(0xEB,0x20);                                                     
		LCD_WriteReg(0xEC,0x0C);                                                   
		LCD_WriteReg(0xED,0xC4);                                                    
		LCD_WriteReg(0xE8,0x40); 
		//LCD_WriteReg(0xE8,0x38);		
		LCD_WriteReg(0xE9,0x38);                                                    
		LCD_WriteReg(0xF1,0x01);                                                    
		LCD_WriteReg(0xF2,0x10);                                                    
		LCD_WriteReg(0x27,0xA3);                                                    
		/* GAMMA SETTING */
		LCD_WriteReg(0x40,0x00);                           
		LCD_WriteReg(0x41,0x00);                                                   
		LCD_WriteReg(0x42,0x01);                                                   
		LCD_WriteReg(0x43,0x12);                                                    
		LCD_WriteReg(0x44,0x10);                                                   
		LCD_WriteReg(0x45,0x26);                                                  
		LCD_WriteReg(0x46,0x08);                                                  
		LCD_WriteReg(0x47,0x53);                                                   
		LCD_WriteReg(0x48,0x02);                                                    
		LCD_WriteReg(0x49,0x15);                                                  
		LCD_WriteReg(0x4A,0x19);                                                  
		LCD_WriteReg(0x4B,0x19);                                                 
		LCD_WriteReg(0x4C,0x16); 
		
		LCD_WriteReg(0x50,0x19);                                                   
		LCD_WriteReg(0x51,0x2F);                                                    
		LCD_WriteReg(0x52,0x2D);                                                     
		LCD_WriteReg(0x53,0x3E);                                                    
		LCD_WriteReg(0x54,0x3F);                                                     
		LCD_WriteReg(0x55,0x3F);                                                     
		LCD_WriteReg(0x56,0x2C);                                                   
		LCD_WriteReg(0x57,0x77);                                                     
		LCD_WriteReg(0x58,0x09);                                                  
		LCD_WriteReg(0x59,0x06);                                                 
		LCD_WriteReg(0x5A,0x06);                                                   
		LCD_WriteReg(0x5B,0x0A);                                                   
		LCD_WriteReg(0x5C,0x1D);                                                   
		LCD_WriteReg(0x5D,0xCC);   
		
		/* Power Voltage Setting */
		LCD_WriteReg(0x1B,0x1B);                                                    
		LCD_WriteReg(0x1A,0x01);  
		LCD_WriteReg(0x24,0x2F);                                                    
		LCD_WriteReg(0x25,0x57);                                                    
		LCD_WriteReg(0x23,0x97);
		
                                                  
		LCD_WriteReg(0x18,0x36);                           
		LCD_WriteReg(0x19,0x01);                                                                                                  
		LCD_WriteReg(0x1F,0x88);                                                    
		delay_ms(5);
		LCD_WriteReg(0x1F,0x80);                                                  
		delay_ms(5);
		LCD_WriteReg(0x1F,0x90);                                                   
		delay_ms(5);
		LCD_WriteReg(0x1F,0xD0);                                                   
		delay_ms(5);
		
		LCD_WriteReg(0x17,0x05);
		
		LCD_WriteReg(0x36,0x00);                                                    
		LCD_WriteReg(0x28,0x38);                                                 
		delay_ms(40);
		LCD_WriteReg(0x28,0x3C);   


//------------------ Set GRAM area ---------------//

		LCD_WriteReg(0x02,0x00);                                                  
		LCD_WriteReg(0x03,0x00);                           
		LCD_WriteReg(0x04,0x00);                           
		LCD_WriteReg(0x05,0xEF);                           
		LCD_WriteReg(0x06,0x00);                           
		LCD_WriteReg(0x07,0x00);                           
		LCD_WriteReg(0x08,0x01);                           
		LCD_WriteReg(0x09,0x3F); 

		 
		LCD_WriteReg(0x0019,0x0001);
		//LCD_WriteReg(0x22,0x22);
	}
					
    delay_ms(50);   /* delay 50 ms */
	//LCD_Clear(White);
//	delay_ms(50);   /* delay 50 ms */
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0,i;
	LCD_SetCursor(0,0);
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

	for( index = 0; index < 320; index++ )
	{
		for(i = 0;i < 240; i++)
		{
		LCD_WR_DATA(Color >> 8);
		LCD_WR_DATA(Color); 
		}
	}
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* Input          : - color: BRG 颜色值  
* Output         : None
* Return         : RGB 颜色值
* Attention		 : 内部函数调用
*******************************************************************************/
/*
static __attribute__((always_inline)) uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}
*/

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 在指定座标画点
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(point>>8);
	LCD_WriteRAM(point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A点行座标
*                  - y1: A点列座标 
*				   - x2: B点行座标
*				   - y2: B点列座标 
*				   - color: 线颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* 定义X Y轴上增加的变量值 */
    short temp;       /* 起点 终点大小比较 交换数据时的中间变量 */

    if( x0 > x1 )     /* X轴上起点大于终点 交换数据 */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     /* Y轴上起点大于终点 交换数据 */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       /* X轴方向上的增量 */
	dy = y1-y0;       /* Y轴方向上的增量 */

    if( dx == 0 )     /* X轴上没有增量 画垂直线 */ 
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描垂直线 */
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     /* Y轴上没有增量 画水平直线 */ 
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描水平线 */
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	/* 布兰森汉姆(Bresenham)算法画线 */
    if( dx > dy )                         /* 靠近X轴 */
    {
	    temp = 2 * dy - dx;               /* 计算下个点的位置 */         
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);    /* 画起点 */ 
	        x0++;                         /* X轴上加1 */
	        if( temp > 0 )                /* 判断下下个点的位置 */
	        {
	            y0++;                     /* 为右上相邻点，即（x0+1,y0+1） */ 
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;           /* 判断下下个点的位置 */  
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                      /* 靠近Y轴 */       
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        LCD_SetPoint(x0,y0,color);
	}
} 


/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - ASCI: 显示的字符
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar(u16 x,u16 y,u8 num,u16 p_color,u16 b_color)
{

	u8 temp,t1,t;
	u16 y0=y;
	num -= ' ';

	
	for(t=0;t<16;t++)
	{   
		temp=asc2_1608[num][t];;  //调用字体数组
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
				LCD_SetPoint(x,y,p_color);//在指定位置打指定颜色的点
			else 
				if(b_color!=0x1111)LCD_SetPoint(x,y,b_color);//在指定位置打指定颜色的点
									
			temp<<=1;
			y++;
			if(x >= 240){return;}//超区域了
			
			if((y-y0)==16)
			{
				y=y0;
				x++;
				if(x >= 320){return;}//超区域了
				break;
			}
		}  	 
	}       
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < 240 - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < 320 - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}
/************************************************
**函数功能：在指定位置显示指定大小的图片
**函数参数：x,y为显示的位置坐标；width为图片的宽度，high为图片的高度。
**ReadAddr为图片存储在flash中的起始位置地址
*************************************************/
void show_picture(u16 x,u16 y,u16 width,u16 high,u32 ReadAddr)
{
	u8 temp1=0x00,temp2=0x00;
	uint32_t i,j = 0;
	LCD_SetCursor(x,y);
  LCD_WriteRAM_Prepare();
	W25Qxx_CS=0;                            //使能器件   
  W25Qxx_Write_Byte(W25X_ReadData);         //发送读取命令   
  W25Qxx_Write_Byte((u8)((ReadAddr)>>16));  //发送24bit地址    
  W25Qxx_Write_Byte((u8)((ReadAddr)>>8));   
  W25Qxx_Write_Byte((u8)ReadAddr);
	for( i = 0; i < high; i++ )
	{
		for(j = 0; j < width; j++)
		{
			temp2=W25Qxx_Read_Byte();
			temp1=W25Qxx_Read_Byte();
			LCD_WR_DATA(temp1); //高八位
			LCD_WR_DATA(temp2); //低八位
		}
		LCD_SetCursor(x,y+i+1);
		LCD_WriteRAM_Prepare();
	}
	W25Qxx_CS=1;
}
/*
**函数功能，在屏幕上画实心方框
**参数：x y方框的左上角
width high方框的宽高
color 方框的颜色
*/
void LCD_DrawBox(u16 x,u16 y,u16 width,u16 high,uint16_t color)
{
	uint32_t i,j = 0;
	LCD_SetCursor(x,y);
  LCD_WriteRAM_Prepare();
	for( i = 0; i < high; i++ )
	{
		for(j = 0; j < width; j++)
		{
			LCD_WR_DATA(color>>8); //高八位
			LCD_WR_DATA(color); //低八位
		}
		LCD_SetCursor(x,y+i+1);
		LCD_WriteRAM_Prepare();
	}
	
}
/*
**函数功能，在屏幕上画空心方框
**参数：x y方框的左上角
width high方框的宽高
color 方框的颜色
*/
void LCD_DrawEBox(u16 x,u16 y,u16 width,u16 high,uint16_t color)
{
	LCD_DrawLine(x,y,x+width,y,color);
	LCD_DrawLine(x+1,y+1,x+width-1,y+1,color);
	LCD_DrawLine(x,y,x,y+high,color);
	LCD_DrawLine(x+1,y+1,x+1,y+high-1,color);
	LCD_DrawLine(x,y+high,x+width,y+high,color);
	LCD_DrawLine(x+1,y+high-1,x+width-1,y+high-1,color);
	LCD_DrawLine(x+width,y,x+width,y+high,color);
	LCD_DrawLine(x+width-1,y+1,x+width-1,y+high-1,color);
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

