/******************************************************************
**���ֿ�оƬ�������ֿ�ΪGBK��ASCII��
**��Ҫ�ֿ�оƬ�ɽ����̣�http://shop105602061.taobao.com/
**������������ϵQQ��765109842
*******************************************************************/

#ifndef W25Qxx_H
#define W25Qxx_H	

#include "stc12c5a60s2.h"
#include "type_51.h"
#include "HX8347D.h" 

/*****************************************************************
�û���Ҫ����ʵ����Ҫ�޸ĵĵط���
//ͷ�ļ�
//Һ������غ궨��
//IO�ڶ���
*****************************************************************/
//ͷ�ļ�
#include "string.h"		

//Һ������غ궨��
#define max_x 240	//Һ����X�����ߴ�
#define max_y 320	//Һ����Y�����ߴ�
//��LCD_SetPoint��Ϊ��Ӧ�Ĵ�㺯����������ԭ�ͣ�void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
#define SHOW_POINT_FUNC		LCD_SetPoint 

//��Ƭ��IO�ڶ���
sbit	W25Qxx_CS =	P2^4; 
sbit	W25Qxx_DO =	P0^3;   
sbit	W25Qxx_DI =	P0^2;
sbit	W25Qxx_CLK = P0^1;


//W25Qϵ��оƬID�б�	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128 0XEF17

//ָ���
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


//��ʾ��غ������ȵ��� font_init();���г�ʼ�����ٵ�����ʾ������
u8 	 font_init(void);
void Show_Str(u16 x,u16 y,u8 *str,u8 size,u16 p_color,u16 b_color);	//��ָ��λ����ʾһ���ַ��� 
void Show_number(u16 x,u16 y,u32 num,u8 len,u8 gbsize,u16 p_color,u16 b_color);//����ָ��λ����ʾһ������ 
u16 W25Qxx_ReadID(void);
u8 W25Qxx_Read_Byte(void);
void W25Qxx_Write_Byte(u8 value);
//������Ϣ�����ַռ100���ֽ�,��4���ֽ����ڱ���ֿ��Ƿ����.����ÿ8���ֽ�һ��,�ֱ𱣴���ʼ��ַ���ļ���С														   
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
typedef struct 
{
	u32 fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	u32 asc68addr; 			//ascii6*8�ĵ�ַ
	u32 asc68size;			//ascii6*8�Ĵ�С
	
	u32 gbk12addr;		//gbk12��ַ	
	u32 gbk12size;		//gbk12�Ĵ�С	
	u32 asc12addr;			//ascii12��ַ
	u32 asc12size;			//ascii12�Ĵ�С
	
	u32 gbk16addr;		//gbk16��ַ
	u32 gbk16size;		//gbk16�Ĵ�С
	u32 asc16addr;			//ascii16��ַ
	u32 asc16size;			//ascii16�Ĵ�С
	
	u32 gbk24addr;		//gbk24��ַ
	u32 gbk24size;		//gbk24�Ĵ�С
	u32 asc24addr;			//ascii24��ַ
	u32 asc24size;			//ascii24�Ĵ�С
	
	u32 gbk32addr;		//gbk32��ַ
	u32 gbk32size;		//gbk32�Ĵ�С
	u32 asc32addr;			//ascii32��ַ
	u32 asc32size;			//ascii32�Ĵ�С
}_font_info;																   

#endif

