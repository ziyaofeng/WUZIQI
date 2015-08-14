/******************************************************************
**���ֿ�оƬ�������ֿ�ΪGBK��ASCII��
*******************************************************************/

#include "w25qxx.h" 

u16 W25Qxx_TYPE = W25Q64;	//����ʹ�õ�flashоƬ�ͺ�

_font_info code ftinfo = //����Ϊ�ֿ���FLASH��ĵ�ַ�������޸ģ�����
{
	170,102476,570,103046,574560,677606,1140,678746,766080,1444826,1520,
	1446346,1723680,3170026,420,3173446,3064320,6237766,6080
};


//��������(ģ����ʹ��)
void W25Qxx_Init(void);			//W25Qxx��ʼ����IO�ڣ� 
u8 	 W25Qxx_Read_Byte(void);			//��W25Qxx��һ���ֽ�
void W25Qxx_Write_Byte(u8 value);	//��W25Qxxдһ���ֽ�
u16  W25Qxx_ReadID(void);  	    //��ȡFLASH ID
void W25Qxx_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void Get_HzMat(u8 *gbcode,u8 *mat,u8 gbsize);	//�õ����ֵĵ����루���飩
void Show_ASCII(u16 x,u16 y,u8 num,u8 gbsize,u16 p_color,u16 b_color);	//��ָ��λ����ʾһ���ַ�
void Show_Font(u16 x,u16 y,u8 *font,u8 gbsize,u16 p_color,u16 b_color);	//��ָ��λ����ʾһ������


/****************************************************************************
**�������ܣ���W25Qxxд��һ���ֽڣ���д���λ��
**����������value��д�������
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
**�������ܣ���W25Qxx����һ���ֽڣ��ȶ�����λ��
**����ֵ������������
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
**�������ܣ���ȡоƬID
**����ֵ��оƬID
**************************************************/
u16 W25Qxx_ReadID(void)
{
	u16 Temp = 0;	  
	W25Qxx_CS=0;				    
	W25Qxx_Write_Byte(0x90);//���Ͷ�ȡID����	    
	W25Qxx_Write_Byte(0x00); 	    
	W25Qxx_Write_Byte(0x00); 	    
	W25Qxx_Write_Byte(0x00); 	 			   
	Temp|=W25Qxx_Read_Byte()<<8;  
	Temp|=W25Qxx_Read_Byte();	 
	W25Qxx_CS=1;				    
	return Temp;
}   		    


//W25Qxxд��ֹ	
//��WEL����  
void W25Qxx_Write_Disable(void)   
{  
	W25Qxx_CS=0;                            //ʹ������   
    W25Qxx_Write_Byte(W25X_WriteDisable);     //����д��ָֹ��    
	W25Qxx_CS=1;                            //ȡ��Ƭѡ     	      
} 

/**************************************************
**�������ܣ���ָ����ַ��ʼ��ȡָ�����ȵ�����
**����������pBuffer:���ݴ洢��
			ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
			NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
**************************************************/
void W25Qxx_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;
	W25Qxx_CS=0;                            //ʹ������   
    W25Qxx_Write_Byte(W25X_ReadData);         //���Ͷ�ȡ����   
    W25Qxx_Write_Byte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    W25Qxx_Write_Byte((u8)((ReadAddr)>>8));   
    W25Qxx_Write_Byte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i] = W25Qxx_Read_Byte();   //ѭ������  
    }
	W25Qxx_CS=1;                            //ȡ��Ƭѡ     	      
}  

/**************************************************
**�������ܣ���ʼ���ֿ�
**����ֵ��0���ֿ����.  1:�ֿⶪʧ
**************************************************/
u8 font_init(void)
{			  												 
	W25Qxx_Write_Disable(); //��ֹд   
	W25Qxx_TYPE = W25Qxx_ReadID();			//��ȡFLASH ID.  
	if(ftinfo.fontok!=0XAA)return 1;		//�ֿ����. 
	W25Qxx_CS=1;
	return 0;		    
}


/**************************************************
**�������ܣ����ֿ��в��ҳ���ģ
**����������code �ַ����Ŀ�ʼ��ַ,GBK��,mat:���ݴ�ŵ�ַ size :��С	 
**************************************************/
void Get_HzMat(unsigned char *gbcode,unsigned char *mat,u8 gbsize)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	u32 foffset; 
	qh=*gbcode;
	ql=*(++gbcode);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{  
	    for(i=0;i<(gbsize*2);i++)	*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;  
				  
	if(gbsize==32)
	{	
		foffset=((unsigned long)190*qh+ql) * 128;//�õ��ֿ��е��ֽ�ƫ���� 
		W25Qxx_Read(mat,foffset+ftinfo.gbk32addr,128);
	}
	else if(gbsize==24)	
	{
		foffset=((unsigned long)190*qh+ql) * 72;//�õ��ֿ��е��ֽ�ƫ����  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk24addr,72);
	}
	else if(gbsize==16)	
	{
		foffset=((unsigned long)190*qh+ql)*32;//�õ��ֿ��е��ֽ�ƫ����  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk16addr,32);
	}
	else if(gbsize==12)	
	{
		foffset=((unsigned long)190*qh+ql)*24;//�õ��ֿ��е��ֽ�ƫ����  	
		W25Qxx_Read(mat,foffset+ftinfo.gbk12addr,24);  
	}			
} 

/**************************************************
**�������ܣ���ʾһ��ָ����С���ַ�
**����������x,y :����,num:ASCII��,size:�����С
			p_color:�����ɫ, b_color��������ɫ	 
**************************************************/
void Show_ASCII(u16 x,u16 y,u8 num,u8 gbsize,u16 p_color,u16 b_color)
{  							  
    u8 temp,t1,t,max_t;
	u8 mat[65] = "";
	u16 y0=y;
	u32 temp_addr;
	u32 offset = num - ' ';//�õ�ƫ�ƺ��ֵ;
		   
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
		temp=mat[t];  //������������
		for(t1=0;t1<8;t1++)
		{		
			if(gbsize==8)
			{
				if((temp >> t1) & 0x01)
					SHOW_POINT_FUNC(x,y,p_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
				else 
					if(b_color!=Nocolor) if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
			}	
			else
			{
				if((temp << t1) & 0x80)
					SHOW_POINT_FUNC(x,y,p_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
				else 
					if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
			}
			
			y++;
			if(y >= max_y){return;}//��������
			
			if((y-y0)==gbsize)
			{
				y=y0;
				x++;
				if(x >= max_x){return;}//��������
				break;
			}
		}  	 
	}       	 	   	 	  
}


/**************************************************
**�������ܣ���ʾһ��ָ����С�ĺ���
**����������x,y :���ֵ�����,font:����GBK��,size:�����С
				p_color:�����ɫ, b_color��������ɫ
**************************************************/
void Show_Font(u16 x,u16 y,u8 *font,u8 gbsize,u16 p_color,u16 b_color)
{
	u16 temp,t,t1,max_t;
	u16 y0=y;
	u8 dzk[256]; 
	if(gbsize!=12 && gbsize!=16 && gbsize!=24 && gbsize!= 32)
		return;//��֧�ֵ�size
	
	Get_HzMat(font,dzk,gbsize);//�õ���Ӧ��С�ĵ�������
	
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
				SHOW_POINT_FUNC(x,y,p_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
			}
			else 
			{
				if(b_color!=Nocolor) SHOW_POINT_FUNC(x,y,b_color);//��ָ��λ�ô�ָ����ɫ�ĵ�
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
**�������ܣ���ָ��λ�ÿ�ʼ��ʾһ���ַ���	  
**����������(x,y):��ʼ���꣬str  :�ַ�����size :�����С��
			p_color:�����ɫ, b_color��������ɫ
**********************************************************************/
void Show_Str(u16 x,u16 y,u8 *str,u8 gbsize,u16 p_color,u16 b_color)
{								  	  
    u8 bHz=0;     //�ַ��������� 	
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str > 0x80)
				bHz=1;//���� 
	        else              //�ַ�
	        {  				
				if(y > max_y)break;//Խ�緵�� 
				Show_ASCII(x,y,*str,gbsize,p_color,b_color);//��Ч����д�� 
				str++; 
				
		        if(gbsize == 8)
					x += 6; //�ַ�,Ϊȫ�ֵ�һ�� 
				else	
					x += gbsize/2; //�ַ�,Ϊȫ�ֵ�һ�� 
					
				if(x > max_x)//����
				{	
					str -= 1; 
					y += gbsize;
					x = 0;	   
				}
	        }
        }
		else//���� 
        {     
            bHz=0;//�к��ֿ� 

	        if(y > max_y)break;//Խ�緵�� 	
			
	        Show_Font(x,y,str,gbsize,p_color,b_color); //��ʾ�������,������ʾ 
	        str+=2; 
	        x += gbsize;//��һ������ƫ��
			
			if(x > max_x)//����
			{	
				str -= 2; 
				y += gbsize;
				x = 0;	   
			}			
        }						 
    }   
}  

/*********************************************************************
**�������ܣ���ָ��λ�ÿ�ʼ��ʾ����	  
**����������(x,y):��ʼ���꣬num :���֣�len:���ֵ�λ����size :�����С��
			p_color:�����ɫ, b_color��������ɫ
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

