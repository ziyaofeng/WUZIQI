/******************************************************************
**此字库芯片包含的字库为GBK和ASCII；
*******************************************************************/

#include "w25qxx.h" 

u16 W25Qxx_TYPE = W25Q64;	//定义使用的flash芯片型号

_font_info code ftinfo = //数字为字库在FLASH里的地址，不能修改！！！
{
	170,102476,570,103046,574560,677606,1140,678746,766080,1444826,1520,
	1446346,1723680,3170026,420,3173446,3064320,6237766,6080
};


//函数声明(模块内使用)
void W25Qxx_Init(void);			//W25Qxx初始化（IO口） 
u8 	 W25Qxx_Read_Byte(void);			//从W25Qxx读一个字节
void W25Qxx_Write_Byte(u8 value);	//向W25Qxx写一个字节
u16  W25Qxx_ReadID(void);  	    //读取FLASH ID
void W25Qxx_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void Get_HzMat(u8 *gbcode,u8 *mat,u8 gbsize);	//得到汉字的点阵码（数组）
void Show_ASCII(u16 x,u16 y,u8 num,u8 gbsize,u16 p_color,u16 b_color);	//在指定位置显示一个字符
void Show_Font(u16 x,u16 y,u8 *font,u8 gbsize,u16 p_color,u16 b_color);	//在指定位置显示一个汉字


/****************************************************************************
**函数功能：向W25Qxx写入一个字节（先写入高位）
**函数参数：value需写入的数据
****************************************************************************/
void W25Qxx_Write_Byte(u8 value)
{ 
   u8 i;
   for(i=0; i<8; i++)
   {    
     if(value & 0x80)
	  W25Qxx_DI = 1;    
     else 
	  W25Qxx_DI = 0;
	 
	 W25Qxx_CLK = 0;        
	 value <<= 1;
     W25Qxx_CLK = 1;       
   }
}


/****************************************************************************
**函数功能：从W25Qxx读出一个字节（先读出高位）
**返回值：读出的数据
****************************************************************************/
u8 W25Qxx_Read_Byte(void)
{ 
   u8 temp=0;
	u8 tt= 0;
   u8 i;
	
   for(i=0;i<8;i++)    
   {
     W25Qxx_CLK=0;         
     W25Qxx_CLK=1;        
	 tt = W25Qxx_DO;   
	 temp = ((temp << 1) | tt); 	  
   }
   return (temp);
}
			   
/**************************************************
**函数功能：读取芯片ID
**返回值：芯片ID
**************************************************/
u16 W25Qxx_ReadID(void)
{
	u16 Temp = 0;	  
	W25Qxx_CS=0;				    
	W25Qxx_Write_Byte(0x90);//发送读取ID命令	    
	W25Qxx_Write_Byte(0x00); 	    
	W25Qxx_Write_Byte(0x00); 	    
	W25Qxx_Write_Byte(0x00); 	 			   
	Temp|=W25Qxx_Read_Byte()<<8;  
	Temp|=W25Qxx_Read_Byte();	 
	W25Qxx_CS=1;				    
	return Temp;
}   		    


//W25Qxx写禁止	
//将WEL清零  
void W25Qxx_Write_Disable(void)   
{  
	W25Qxx_CS=0;                            //使能器件   
    W25Qxx_Write_Byte(W25X_WriteDisable);     //发送写禁止指令    
	W25Qxx_CS=1;                            //取消片选     	      
} 

/**************************************************
**函数功能：在指定地址开始读取指定长度的数据
**函数参数：pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
**************************************************/
void W25Qxx_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;
	W25Qxx_CS=0;                            //使能器件   
    W25Qxx_Write_Byte(W25X_ReadData);         //发送读取命令   
    W25Qxx_Write_Byte((u8)((ReadAddr)>>16));  //发送24bit地址    
    W25Qxx_Write_Byte((u8)((ReadAddr)>>8));   
    W25Qxx_Write_Byte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i] = W25Qxx_Read_Byte();   //循环读数  
    }
	W25Qxx_CS=1;                            //取消片选     	      
}  

/**************************************************
**函数功能：初始化字库
**返回值：0：字库完好.  1:字库丢失
**************************************************/
u8 font_init(void)
{			  												 
	W25Qxx_Write_Disable(); //禁止写   
	W25Qxx_TYPE = W25Qxx_ReadID();			//读取FLASH ID.  
	if(ftinfo.fontok!=0XAA)return 1;		//字库错误. 
	W25Qxx_CS=1;
	return 0;		    
}


/**************************************************
**函数功能：从字库中查找出字模
**函数参数：code 字符串的开始地址,GBK码,mat:数据存放地址 size :大小	 
**************************************************/
void Get_HzMat(unsigned char *gbcode,unsigned char *mat,u8 gbsize)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	u32 foffset; 
	qh=*gbcode;
	ql=*(++gbcode);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{  
	    for(i=0;i<(gbsize*2);i++)	*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;  
				  
	if(gbsize==32)
	{	
		foffset=((unsigned long)190*qh+ql) * 128;//得到字库中的字节偏移量 
		W25Qxx_Read(mat,foffset+ftinfo.gbk32addr,128);
	}
	else if(gbsize==24)	
	{
		foffset=((unsigned long)190*qh+ql) * 72;//得到字库中的字节偏移量  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk24addr,72);
	}
	else if(gbsize==16)	
	{
		foffset=((unsigned long)190*qh+ql)*32;//得到字库中的字节偏移量  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk16addr,32);
	}
	else if(gbsize==12)	
	{
		foffset=((unsigned long)190*qh+ql)*24;//得到字库中的字节偏移量  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk12addr,24);  
	}			
} 

/**************************************************
**函数功能：显示一个指定大小的字符
**函数参数：x,y :坐标,num:ASCII码,size:字体大小
			p_color:点的颜色, b_color：背景颜色	 
**************************************************/
void Show_ASCII(u16 x,u16 y,u8 num,u8 gbsize,u16 p_color,u16 b_color)
{  							  
    u8 temp,t1,t,max_t;
	u8 mat[65] = "";
	u16 y0=y;
	u32 temp_addr;
	u32 offset = num - ' ';//得到偏移后的值;
		   
	if(gbsize==32)		
	{
		offset *= 64;	
		max_t = 64;
		temp_addr = ftinfo.asc32addr;
	}
	else if(gbsize==24)	
	{
		offset *= 36;	
		max_t = 36;
		temp_addr = ftinfo.asc24addr;
	}
	else if(gbsize==16)	
	{
		offset *= 16;	
		max_t = 16;
		temp_addr = ftinfo.asc16addr;
	}
	else if(gbsize==12)	
	{
		offset *= 12;	
		max_t = 12;
		temp_addr = ftinfo.asc12addr;
	}
	
	else if(gbsize==8)	
	{
		offset *= 6;	
		max_t = 6;
		temp_addr = ftinfo.asc68addr;
	}
	
	W25Qxx_Read(mat,offset + temp_addr,max_t);
	
	for(t=0;t<max_t;t++)
	{   
		temp=mat[t];  //调用字体数组
		for(t1=0;t1<8;t1++)
		{		
			if(gbsize==8)
			{
				if((temp >> t1) & 0x01)
					SHOW_POINT_FUNC(x,y,p_color);//在指定位置打指定颜色的点
				else 
					if(b_color!=Nocolor) if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//在指定位置打指定颜色的点
			}	
			else
			{
				if((temp << t1) & 0x80)
					SHOW_POINT_FUNC(x,y,p_color);//在指定位置打指定颜色的点
				else 
					if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//在指定位置打指定颜色的点
			}
			
			y++;
			if(y >= max_y){return;}//超区域了
			
			if((y-y0)==gbsize)
			{
				y=y0;
				x++;
				if(x >= max_x){return;}//超区域了
				break;
			}
		}  	 
	}       	 	   	 	  
}


/**************************************************
**函数功能：显示一个指定大小的汉字
**函数参数：x,y :汉字的坐标,font:汉字GBK码,size:字体大小
				p_color:点的颜色, b_color：背景颜色
**************************************************/
void Show_Font(u16 x,u16 y,u8 *font,u8 gbsize,u16 p_color,u16 b_color)
{
	u16 temp,t,t1,max_t;
	u16 y0=y;
	u8 dzk[256]; 
	if(gbsize!=12 && gbsize!=16 && gbsize!=24 && gbsize!= 32)
		return;//不支持的size
	
	Get_HzMat(font,dzk,gbsize);//得到相应大小的点阵数据
	
	if(gbsize == 32)      max_t = 128;
	else if(gbsize == 24) max_t = 72;
	else				max_t = gbsize * 2;
	
 
	for(t = 0;t < max_t;t++)
	{   												   
		temp=dzk[t];                         
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
			{
				SHOW_POINT_FUNC(x,y,p_color);//在指定位置打指定颜色的点
			}
			else 
			{
				if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//在指定位置打指定颜色的点
			}
			
			temp<<=1;
			y++;
			if((y-y0)==gbsize)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	} 
}


/*********************************************************************
**函数功能：在指定位置开始显示一个字符串	  
**函数参数：(x,y):起始坐标，str  :字符串，size :字体大小，
			p_color:点的颜色, b_color：背景颜色
**********************************************************************/
void Show_Str(u16 x,u16 y,u8 *str,u8 gbsize,u16 p_color,u16 b_color)
{								  	  
    u8 bHz=0;     //字符或者中文 	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str > 0x80)
				bHz=1;//中文 
	        else              //字符
	        {  				
				if(y > max_y)break;//越界返回 
				Show_ASCII(x,y,*str,gbsize,p_color,b_color);//有效部分写入 
				str++; 
				
		        if(gbsize == 8)
					x += 6; //字符,为全字的一半 
				else	
					x += gbsize/2; //字符,为全字的一半 
					
				if(x > max_x)//换行
				{	
					str -= 1; 
					y += gbsize;
					x = 0;	   
				}
	        }
        }
		else//中文 
        {     
            bHz=0;//有汉字库 

	        if(y > max_y)break;//越界返回 	
			
	        Show_Font(x,y,str,gbsize,p_color,b_color); //显示这个汉字,空心显示 
	        str+=2; 
	        x += gbsize;//下一个汉字偏移
			
			if(x > max_x)//换行
			{	
				str -= 2; 
				y += gbsize;
				x = 0;	   
			}			
        }						 
    }   
}  

/*********************************************************************
**函数功能：在指定位置开始显示数字	  
**函数参数：(x,y):起始坐标，num :数字，len:数字的位数，size :字体大小，
			p_color:点的颜色, b_color：背景颜色
**********************************************************************/
/*void Show_number(u16 x,u16 y,u32 num,u8 len,u8 gbsize,u16 p_color,u16 b_color)
{	
	u8 buf[10] = "";
	u32 t_num = 1;
	u8 i;
	for(i = 0; i < len; i++)
	{
		buf[len - i - 1] = num / t_num % 10 + '0';
		t_num *= 10;
	}
		
	Show_Str(x,y,buf,gbsize,p_color,b_color);
		
}*/

