#include "type_51.h"
# define SPA 0//在C或C++语言源程序中允许用一个标识符来表示一个字符串，称为“宏”。 “define”为宏定义命令。

# define MAN 1

# define COM 2

#define u8 unsigned char
    /* 空位置设为0 ，玩家下的位置设为1 ，电脑下的位置设为2 */
#define ORDEX 15
u8 qipan[ORDEX][ORDEX];  //自己定义的预处理函数，以二维数组形式构建棋盘  /* 15*15的棋盘 */
static int DirectionValue( int * piX, int * piY, int iDirection );
void PCTurn(u16 *x,u16 *y);
int qixing(int n,int p,int q) ;
int value(u8 p,u8 q);
extern void alert(char *s,u16 bcolor);
void yiwei(int n,int *i,int *j)        /* 在n方向上对坐标 i j 移位 n为1-8方向 从右顺时针开始数 */

{

    switch(n){

    case 1: *i+=1; break;

    case 2: *i+=1; *j+=1; break;

    case 3: *j+=1; break;

    case 4: *i-=1; *j+=1; break;

    case 5: *i-=1; break;

    case 6: *i-=1; *j-=1; break;

    case 7: *j-=1; break;

    case 8: *i+=1; *j-=1; break;

    }

}
int qixing(int n,int p,int q)            /* 返回空点p q在n方向上的棋型号 n为1-8方向 从右顺时针开始数 */ 

{

    int k,m=0;  /* 棋型号注解:  己活000-003 己冲010-013 对活100-103 对冲110-113 己空活020-023 己空冲030-033 对空活120-123 对空冲130-133 空-1 边界冲-2 边界空冲-3*/

    yiwei(n,&p,&q);

    if(p<0||p>14||q<0||q>14) k=-2;                                      /* 边界冲棋型 */

    switch(qipan[q][p]){

        case COM:{

                    m++; yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=m+9; return k; }

                    while(qipan[q][p]==COM) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+9; return k; } }

                    if(qipan[q][p]==SPA) k=m-1;                         /* 己方活棋型 */

                     else k=m+9;                                         /* 己方冲棋型 */

                 }break;

        case MAN:{

                    m++; yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=m+109; return k; }

                    while(qipan[q][p]==MAN) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+109; return k; } }

                    if(qipan[q][p]==SPA) k=m+99;                         /* 对方活棋型 */

                    else k=m+109;                                        /* 对方冲棋型 */

                 }break;

        case SPA:{

                    yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=-3; return k; }         /* 边界空冲棋型 */

                    switch(qipan[q][p]){

                    case COM:{

                                m++; yiwei(n,&p,&q);

                                if(p<0||p>14||q<0||q>14) { k=m+29; return k; }

                                while(qipan[q][p]==COM) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+29; return k; } }

                                if(qipan[q][p]==SPA) k=m+19;                      /* 己方空活棋型 */

                                else k=m+29;                                      /* 己方空冲棋型 */

                             }break;

                    case MAN:{

                                m++; yiwei(n,&p,&q);

                                if(p<0||p>14||q<0||q>14) { k=m+129; return k; }

                                while(qipan[q][p]==MAN) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+129; return k; } }           

                                if(qipan[q][p]==SPA) k=m+119;                     /* 对方空活棋型 */

                                else k=m+129;                                     /* 对方空冲棋型 */ 

                             }break;

                    case SPA: k=-1; break;                                         /* 空棋型 */

                    }

                }break;

    }

    return k;

}
int value(u8 p,u8 q) /* 计算空点p q的价值 以k返回 */ 

{
	

    int n=1,k=0,k1,k2,K1,K2,X1,Y1,Z1,X2,Y2,Z2,temp;  

    int a[2][4][4]={40,400,3000,10000,6,10,600,10000,20,120,200,0,6,10,500,0,30,300,2500,5000,2,8,300,8000,26,160,0,0,4,20,300,0};

           /* 数组a中储存己方和对方共32种棋型的值  己方0对方1    活0冲1空活2空冲3    子数0-3（0表示1个子，3表示4个子） */

    while(n!=5){

        k1=qixing(n,p,q); n+=4;            /* k1,k2为2个反方向的棋型编号 */

        k2=qixing(n,p,q); n-=3;

        if(k1>k2) { temp=k1; k1=k2; k2=temp; }  /* 使编号小的为k1,大的为k2 */

        K1=k1; K2=k2;       /* K1 K2储存k1 k2的编号 */

        Z1=k1%10; Z2=k2%10; k1/=10; k2/=10; Y1=k1%10; Y2=k2%10; k1/=10; k2/=10; X1=k1%10; X2=k2%10;

            /* X Y Z分别表示 己方0对方1    活0冲1空活2空冲3    子数0-3（0表示1个子，3表示4个子） */

        if(K1==-1) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]+5; continue;  };    /* 空棋型and其他 */

        if(K1==-2) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/2; continue; };       /* 边界冲棋型and其他 */ 

        if(K1==-3) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/3; continue; };    /* 边界空冲棋型and其他 */ 

        if(((K1>-1&&K1<4)&&((K2>-1&&K2<4)||(K2>9&&K2<14)))||((K1>99&&K1<104)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){

            /* 己活己活 己活己冲 对活对活 对活对冲 的棋型赋值*/

            if(Z1+Z2>=2) { k+=a[X2][Y2][3]; continue; }

            else { k+=a[X2][Y2][Z1+Z2+1]; continue; }

        }

        if(((K1>9&&K1<14)&&(K2>9&&K2<14))||((K1>109&&K1<114)&&(K2>109&&K2<114))){

            /* 己冲己冲 对冲对冲 的棋型赋值*/

            if(Z1+Z2>=2) { k+=10000; continue; }

            else { k+=0; continue; }

        }

        if(((K1>-1&&K1<4)&&((K2>99&&K2<104)||(K2>109&&K2<114)))||((K1>9&&K1<14)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){

            /* 己活对活 己活对冲 己冲对活 己冲对冲 的棋型赋值*/

            if(Z1==3||Z2==3) { k+=10000; continue; }

            else { k+=a[X2][Y2][Z2]+a[X1][Y1][Z1]/4; continue; } 

        }

        else  { k+=a[X1][Y1][Z1]+a[X2][Y2][Z2]; continue;  }    /* 其他棋型的赋值 */

    }
    return k;

}
void PCTurn(u16 *x,u16 *y)           /* 电脑下子 *p *q返回下子坐标 */

{

    u16 i,j,max=0,I,J;        /* I J为下点坐标 */
	
	int k;

    for(j=0;j<15;j++)

        for(i=0;i<15;i++)

            if(qipan[j][i]==SPA){      /* 历遍棋盘，遇到空点则计算价值，取最大价值点下子。 */

                k=value(i,j);

               if(k>=max) { I=i; J=j; max=k; }

            }

    *x=I; *y=J;

}

