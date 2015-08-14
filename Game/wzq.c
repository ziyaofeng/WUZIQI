#include "type_51.h"
#include "stc12c5a60s2.h"
#include "hx8347d.h"
#include "touch.h"
#define ORDEX 15
extern u8 qipan[ORDEX][ORDEX];  //自己定义的预处理函数，以二维数组形式构建棋盘  /* 15*15的棋盘 */
extern void PCTurn(u8 *x,u8 *y);         /* 电脑下子 *p *q返回下子坐标 */
extern void delay_ms(u16 ms);
extern void alert(char *s,u16 bcolor);
extern void Get_Click(u16 *x,u16 *y);
extern void Show_Font(u16 x,u16 y,u8 *font,u8 gbsize,u16 p_color,u16 b_color);
u8 cbx,cby,pbx,pby;
u16 ucolor=White,ccolor=Black;
bit first=0;
void gamecfg()
{
	u16 x,y;
	LCD_Clear(0x051d);
	Show_Str(40,0,"人机五子棋",32,White,Nocolor);
	Show_Str(20,40,"□使用黑色棋子",24,Black,Nocolor);
	Show_Str(20,70,"□使用白色棋子",24,Black,Nocolor);
	Show_Str(20,100,"□机器先下子",24,Black,Nocolor);
	Show_Str(20,130,"□你先下子",24,Black,Nocolor);
	LCD_DrawBox(20,180,40,20,0xc618);
	Show_Str(24,182,"确认",16,Black,Nocolor);
	LCD_DrawEBox(20,180,40,20,Blue);
	if(ucolor==Black){Show_Str(20,40,"√",24,Black,Nocolor);Show_Str(20,70,"□",24,Black,0x051d);}
	if(ucolor==White){Show_Str(20,70,"√",24,Black,Nocolor);Show_Str(20,40,"□",24,Black,0x051d);}
	if(first){Show_Str(20,100,"√",24,Black,Nocolor);Show_Str(20,130,"□",24,Black,0x051d);}
	else{Show_Str(20,100,"□",24,Black,0x051d);Show_Str(20,130,"√",24,Black,Nocolor);}
	while(1)
	{
		while(PEN);
		delay_ms(1);
		if(!PEN)
		{
			Get_Click(&x,&y);
			if((x>20&&x<200)&&(y>40&&y<64)){ucolor=Black;ccolor=White;}
			if((x>20&&x<200)&&(y>70&&y<94)){ucolor=White;ccolor=Black;}
			if((x>20&&x<200)&&(y>100&&y<124))first=1;
			if((x>20&&x<200)&&(y>130&&y<154))first=0;
			if((x>20&&x<60)&&(y>180&&y<200)) return;
			if(ucolor==Black){Show_Str(20,40,"√",24,Black,Nocolor);Show_Str(20,70,"□",24,Black,0x051d);}
			if(ucolor==White){Show_Str(20,70,"√",24,Black,Nocolor);Show_Str(20,40,"□",24,Black,0x051d);}
			if(first){Show_Str(20,100,"√",24,Black,Nocolor);Show_Str(20,130,"□",24,Black,0x051d);}
			else{Show_Str(20,100,"□",24,Black,0x051d);Show_Str(20,130,"√",24,Black,Nocolor);}
		}
		while(!PEN);
	}
}
void GameInit()
{
	u8 x,y;
	for(y=0;y<=14;y++)
	{
		for(x=0;x<=14;x++) qipan[y][x]=0;
	}
	if(first)qipan[7][7]=2;
}
void Show_QIPAN()
{
	u16 x,y;
	for(y=35;y<=275;y+=16)
	{
		LCD_DrawLine( 0,y,240,y ,White );
	}
	for(x=0;x<241;x+=16)
	{
		LCD_DrawLine( x,35,x,275 ,White );
	}
}
void Refresh()
{
	u16 x,y;
	for(y=0;y<=14;y++)
	{
		for(x=0;x<=14;x++)
		{
			if(qipan[y][x]==1) {Show_Font(x*16,y*16+35,"●",16,ucolor,Nocolor);}
			if(qipan[y][x]==2) {Show_Font(x*16,y*16+35,"●",16,ccolor,Nocolor);}
		}
	}
}
u8 Victory(u8 ix,u8 iy)      /* 判断胜利 p q为判断点坐标，胜利返回1，否则返回0 */
{
	u8 x=ix,y=iy;
	u8 qpvalue=qipan[iy][ix],count=0;
	while((qipan[y][x+1]==qpvalue)&&(x<=14))
	{
		x++;
	}
	while((qipan[y][x]==qpvalue)&&(x>=0))
	{
		count++;
		if(x!=0)x--;else break;
	}
	if(count==5){return 1;}
	qpvalue=qipan[iy][ix],count=0;x=ix;y=iy;
	while((qipan[y+1][x]==qpvalue)&&(y<=14))
	{
		y++;
	}
	while((qipan[y][x]==qpvalue)&&(y>=0))
	{
		count++;
		if(y!=0)y--;else break;
	}
	if(count==5){return 1;}
	qpvalue=qipan[iy][ix],count=0;x=ix;y=iy;
	while((qipan[y+1][x+1]==qpvalue)&&(y<=14&&x<=14))
	{
		y++;
		x++;
	}
	while((qipan[y][x]==qpvalue)&&(y>=0&&x>=0))
	{
		count++;
		if(y!=0&&x!=0){y--;x--;}else break;
	}
	if(count==5){return 1;}
	qpvalue=qipan[iy][ix],count=0;x=ix;y=iy;
	while((qipan[y-1][x+1]==qpvalue)&&(y>=0&&x<=14))
	{
		y--;
		x++;
	}
	while((qipan[y][x]==qpvalue)&&(y<=14&&x>=0))
	{
		count++;
		if(y!=0&&x!=0){y++;x--;}else break;
	}
	if(count==5){return 1;}
	return 0;
}
void huiqi()
{
	alert("悔棋",0x051d);
	qipan[pby][pbx]=0;
	qipan[cby][cbx]=0;
	Show_Font(pbx*16,pby*16+35,"●",16,0x051d,Nocolor);
	Show_Font(cbx*16,cby*16+35,"●",16,0x051d,Nocolor);
}
void WZQ()
{
	uint16_t x,y;
	u8 temp;
	u8 win=0;
	LCD_Clear(0x051d);
	Show_Str(40,0,"人机五子棋",32,White,Nocolor);
	Show_QIPAN();
	LCD_DrawBox(20,280,40,20,0xc618);
	Show_Str(24,282,"悔棋",16,Black,Nocolor);
	LCD_DrawEBox(20,280,40,20,Blue);
	LCD_DrawBox(70,280,40,20,0xc618);
	Show_Str(74,282,"重来",16,Black,Nocolor);
	LCD_DrawEBox(70,280,40,20,Blue);
	Refresh();
	while(1)
	{
		while(PEN);
		delay_ms(1);
		if(!PEN)
		{
			Get_Click(&x,&y);
			if((y>280&&y<300)&&(x>20&&x<60)) huiqi();
			if((y>280&&y<300)&&(x>70&&x<110)) return;
			while(win)
			{
				Get_Click(&x,&y);
				if((y>280&&y<300)&&(x>20&&x<60)){ huiqi();win=0;}
				if((y>280&&y<300)&&(x>70&&x<110)){return;win=0;}
			}
			if(y>35&&y<275) 
			{
				temp=(y-35)%16;
				(u16)y=(y-35-temp)/15;
				temp=x%16;
				(u16)x=(x-temp)/15;
				if((x>=0&&x<=14)&&(y>=0&&y<=14)&&(!qipan[y][x]))
				{
					qipan[y][x]=1;
					pbx=x;pby=y;
					Refresh();
					if(Victory(x,y)){alert("你赢了！",0x051d);win=1;}
					PCTurn(&x,&y);
					if(!qipan[y][x])qipan[y][x]=2;
					cbx=x;cby=y;
					Refresh();
					if(Victory(x,y)){alert("你输了！",0x051d);win=1;}
				}else{
					alert("棋子已存在",0x051d);
					Show_Str(40,0,"人机五子棋",32,White,Nocolor);
				}
			}
		}
		while(!PEN);
	}
}
void main()
{
	LCD_Initializtion();
	font_init();
	TP_Init();
	while(1)
	{
		gamecfg();
		GameInit();
		WZQ();
	}
}
