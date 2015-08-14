/******************************************************************
**此字库芯片包含的字库为GBK和ASCII；
**需要字库芯片可进店铺：http://shop105602061.taobao.com/
**技术交流请联系QQ：765109842
*******************************************************************/

#ifndef W25Qxx_H
#define W25Qxx_H	

#include "stc12c5a60s2.h"
#include "type_51.h"
#include "HX8347D.h" 

/*****************************************************************
用户需要根据实质需要修改的地方：
//头文件
//液晶屏相关宏定义
//IO口定义
*****************************************************************/
//头文件
#include "string.h"		

//液晶屏相关宏定义
#define max_x 240	//液晶屏X轴最大尺寸
#define max_y 320	//液晶屏Y轴最大尺寸
//把LCD_SetPoint改为对应的打点函数名，函数原型：void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
#define SHOW_POINT_FUNC		LCD_SetPoint 

//单片机IO口定义
sbit	W25Qxx_CS =	P2^4; 
sbit	W25Qxx_DO =	P0^3;   
sbit	W25Qxx_DI =	P0^2;
sbit	W25Qxx_CLK = P0^1;


//W25Q系列芯片ID列表	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128 0XEF17

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


//显示相关函数（先调用 font_init();进行初始化，再调用显示函数）
u8 	 font_init(void);
void Show_Str(u16 x,u16 y,u8 *str,u8 size,u16 p_color,u16 b_color);	//在指定位置显示一个字符串 
void Show_number(u16 x,u16 y,u32 num,u8 len,u8 gbsize,u16 p_color,u16 b_color);//在在指定位置显示一个数字 
u16 W25Qxx_ReadID(void);
u8 W25Qxx_Read_Byte(void);
void W25Qxx_Write_Byte(u8 value);
//字体信息保存地址占100个字节,第4个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小														   
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
typedef struct 
{
	u32 fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	u32 asc68addr; 			//ascii6*8的地址
	u32 asc68size;			//ascii6*8的大小
	
	u32 gbk12addr;		//gbk12地址	
	u32 gbk12size;		//gbk12的大小	
	u32 asc12addr;			//ascii12地址
	u32 asc12size;			//ascii12的大小
	
	u32 gbk16addr;		//gbk16地址
	u32 gbk16size;		//gbk16的大小
	u32 asc16addr;			//ascii16地址
	u32 asc16size;			//ascii16的大小
	
	u32 gbk24addr;		//gbk24地址
	u32 gbk24size;		//gbk24的大小
	u32 asc24addr;			//ascii24地址
	u32 asc24size;			//ascii24的大小
	
	u32 gbk32addr;		//gbk32地址
	u32 gbk32size;		//gbk32的大小
	u32 asc32addr;			//ascii32地址
	u32 asc32size;			//ascii32的大小
}_font_info;																   

#endif

