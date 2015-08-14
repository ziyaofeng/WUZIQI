/* Includes ------------------------------------------------------------------*/
#include "HX8347D.h" 
#include "font.h"
#include "w25qxx.h"


/* Private variables ---------------------------------------------------------*/
//static uint8_t LCD_Code;
u16 LCD_id;


//д�Ĵ�������
void LCD_WR_REG(u8 dat)
{ 
	LCD_RS=0;//д��ַ  
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
* Description    : ����Ļ����ָ������ɫ��������������� 0xffff
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
* Description    : RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* Input          : - color: BRG ��ɫֵ  
* Output         : None
* Return         : RGB ��ɫֵ
* Attention		 : �ڲ���������
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
* Description    : ��ָ�����껭��
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
* Input          : - x1: A��������
*                  - y1: A�������� 
*				   - x2: B��������
*				   - y2: B�������� 
*				   - color: ����ɫ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* ����X Y�������ӵı���ֵ */
    short temp;       /* ��� �յ��С�Ƚ� ��������ʱ���м���� */

    if( x0 > x1 )     /* X�����������յ� �������� */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     /* Y�����������յ� �������� */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       /* X�᷽���ϵ����� */
	dy = y1-y0;       /* Y�᷽���ϵ����� */

    if( dx == 0 )     /* X����û������ ����ֱ�� */ 
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* �����ʾ �费ֱ�� */
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     /* Y����û������ ��ˮƽֱ�� */ 
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* �����ʾ ��ˮƽ�� */
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	/* ����ɭ��ķ(Bresenham)�㷨���� */
    if( dx > dy )                         /* ����X�� */
    {
	    temp = 2 * dy - dx;               /* �����¸����λ�� */         
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);    /* ����� */ 
	        x0++;                         /* X���ϼ�1 */
	        if( temp > 0 )                /* �ж����¸����λ�� */
	        {
	            y0++;                     /* Ϊ�������ڵ㣬����x0+1,y0+1�� */ 
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;           /* �ж����¸����λ�� */  
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                      /* ����Y�� */       
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
* Description    : ��Lcd��������λ����ʾһ���ַ�
* Input          : - Xpos: ˮƽ���� 
*                  - Ypos: ��ֱ����  
*				   - ASCI: ��ʾ���ַ�
*				   - charColor: �ַ���ɫ   
*				   - bkColor: ������ɫ 
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
		temp=asc2_1608[num][t];;  //������������
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
				LCD_SetPoint(x,y,p_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
			else 
				if(b_color!=0x1111)LCD_SetPoint(x,y,b_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
									
			temp<<=1;
			y++;
			if(x >= 240){return;}//��������
			
			if((y-y0)==16)
			{
				y=y0;
				x++;
				if(x >= 320){return;}//��������
				break;
			}
		}  	 
	}       
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : ��ָ��������ʾ�ַ���
* Input          : - Xpos: ������
*                  - Ypos: ������ 
*				   - str: �ַ���
*				   - charColor: �ַ���ɫ   
*				   - bkColor: ������ɫ 
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
**�������ܣ���ָ��λ����ʾָ����С��ͼƬ
**����������x,yΪ��ʾ��λ�����ꣻwidthΪͼƬ�Ŀ�ȣ�highΪͼƬ�ĸ߶ȡ�
**ReadAddrΪͼƬ�洢��flash�е���ʼλ�õ�ַ
*************************************************/
void show_picture(u16 x,u16 y,u16 width,u16 high,u32 ReadAddr)
{
	u8 temp1=0x00,temp2=0x00;
	uint32_t i,j = 0;
	LCD_SetCursor(x,y);
  LCD_WriteRAM_Prepare();
	W25Qxx_CS=0;                            //ʹ������   
  W25Qxx_Write_Byte(W25X_ReadData);         //���Ͷ�ȡ����   
  W25Qxx_Write_Byte((u8)((ReadAddr)>>16));  //����24bit��ַ    
  W25Qxx_Write_Byte((u8)((ReadAddr)>>8));   
  W25Qxx_Write_Byte((u8)ReadAddr);
	for( i = 0; i < high; i++ )
	{
		for(j = 0; j < width; j++)
		{
			temp2=W25Qxx_Read_Byte();
			temp1=W25Qxx_Read_Byte();
			LCD_WR_DATA(temp1); //�߰�λ
			LCD_WR_DATA(temp2); //�Ͱ�λ
		}
		LCD_SetCursor(x,y+i+1);
		LCD_WriteRAM_Prepare();
	}
	W25Qxx_CS=1;
}
/*
**�������ܣ�����Ļ�ϻ�ʵ�ķ���
**������x y��������Ͻ�
width high����Ŀ��
color �������ɫ
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
			LCD_WR_DATA(color>>8); //�߰�λ
			LCD_WR_DATA(color); //�Ͱ�λ
		}
		LCD_SetCursor(x,y+i+1);
		LCD_WriteRAM_Prepare();
	}
	
}
/*
**�������ܣ�����Ļ�ϻ����ķ���
**������x y��������Ͻ�
width high����Ŀ��
color �������ɫ
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

