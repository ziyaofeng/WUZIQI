#include "type_51.h"
# define SPA 0//��C��C++����Դ������������һ����ʶ������ʾһ���ַ�������Ϊ���ꡱ�� ��define��Ϊ�궨�����

# define MAN 1

# define COM 2

#define u8 unsigned char
    /* ��λ����Ϊ0 ������µ�λ����Ϊ1 �������µ�λ����Ϊ2 */
#define ORDEX 15
u8 qipan[ORDEX][ORDEX];  //�Լ������Ԥ���������Զ�ά������ʽ��������  /* 15*15������ */
static int DirectionValue( int * piX, int * piY, int iDirection );
void PCTurn(u16 *x,u16 *y);
int qixing(int n,int p,int q) ;
int value(u8 p,u8 q);
extern void alert(char *s,u16 bcolor);
void yiwei(int n,int *i,int *j)        /* ��n�����϶����� i j ��λ nΪ1-8���� ����˳ʱ�뿪ʼ�� */

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
int qixing(int n,int p,int q)            /* ���ؿյ�p q��n�����ϵ����ͺ� nΪ1-8���� ����˳ʱ�뿪ʼ�� */ 

{

    int k,m=0;  /* ���ͺ�ע��:  ����000-003 ����010-013 �Ի�100-103 �Գ�110-113 ���ջ�020-023 ���ճ�030-033 �Կջ�120-123 �Կճ�130-133 ��-1 �߽��-2 �߽�ճ�-3*/

    yiwei(n,&p,&q);

    if(p<0||p>14||q<0||q>14) k=-2;                                      /* �߽������ */

    switch(qipan[q][p]){

        case COM:{

                    m++; yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=m+9; return k; }

                    while(qipan[q][p]==COM) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+9; return k; } }

                    if(qipan[q][p]==SPA) k=m-1;                         /* ���������� */

                     else k=m+9;                                         /* ���������� */

                 }break;

        case MAN:{

                    m++; yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=m+109; return k; }

                    while(qipan[q][p]==MAN) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+109; return k; } }

                    if(qipan[q][p]==SPA) k=m+99;                         /* �Է������� */

                    else k=m+109;                                        /* �Է������� */

                 }break;

        case SPA:{

                    yiwei(n,&p,&q);

                    if(p<0||p>14||q<0||q>14) { k=-3; return k; }         /* �߽�ճ����� */

                    switch(qipan[q][p]){

                    case COM:{

                                m++; yiwei(n,&p,&q);

                                if(p<0||p>14||q<0||q>14) { k=m+29; return k; }

                                while(qipan[q][p]==COM) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+29; return k; } }

                                if(qipan[q][p]==SPA) k=m+19;                      /* �����ջ����� */

                                else k=m+29;                                      /* �����ճ����� */

                             }break;

                    case MAN:{

                                m++; yiwei(n,&p,&q);

                                if(p<0||p>14||q<0||q>14) { k=m+129; return k; }

                                while(qipan[q][p]==MAN) { m++; yiwei(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+129; return k; } }           

                                if(qipan[q][p]==SPA) k=m+119;                     /* �Է��ջ����� */

                                else k=m+129;                                     /* �Է��ճ����� */ 

                             }break;

                    case SPA: k=-1; break;                                         /* ������ */

                    }

                }break;

    }

    return k;

}
int value(u8 p,u8 q) /* ����յ�p q�ļ�ֵ ��k���� */ 

{
	

    int n=1,k=0,k1,k2,K1,K2,X1,Y1,Z1,X2,Y2,Z2,temp;  

    int a[2][4][4]={40,400,3000,10000,6,10,600,10000,20,120,200,0,6,10,500,0,30,300,2500,5000,2,8,300,8000,26,160,0,0,4,20,300,0};

           /* ����a�д��漺���ͶԷ���32�����͵�ֵ  ����0�Է�1    ��0��1�ջ�2�ճ�3    ����0-3��0��ʾ1���ӣ�3��ʾ4���ӣ� */

    while(n!=5){

        k1=qixing(n,p,q); n+=4;            /* k1,k2Ϊ2������������ͱ�� */

        k2=qixing(n,p,q); n-=3;

        if(k1>k2) { temp=k1; k1=k2; k2=temp; }  /* ʹ���С��Ϊk1,���Ϊk2 */

        K1=k1; K2=k2;       /* K1 K2����k1 k2�ı�� */

        Z1=k1%10; Z2=k2%10; k1/=10; k2/=10; Y1=k1%10; Y2=k2%10; k1/=10; k2/=10; X1=k1%10; X2=k2%10;

            /* X Y Z�ֱ��ʾ ����0�Է�1    ��0��1�ջ�2�ճ�3    ����0-3��0��ʾ1���ӣ�3��ʾ4���ӣ� */

        if(K1==-1) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]+5; continue;  };    /* ������and���� */

        if(K1==-2) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/2; continue; };       /* �߽������and���� */ 

        if(K1==-3) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/3; continue; };    /* �߽�ճ�����and���� */ 

        if(((K1>-1&&K1<4)&&((K2>-1&&K2<4)||(K2>9&&K2<14)))||((K1>99&&K1<104)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){

            /* ����� ����� �Ի�Ի� �Ի�Գ� �����͸�ֵ*/

            if(Z1+Z2>=2) { k+=a[X2][Y2][3]; continue; }

            else { k+=a[X2][Y2][Z1+Z2+1]; continue; }

        }

        if(((K1>9&&K1<14)&&(K2>9&&K2<14))||((K1>109&&K1<114)&&(K2>109&&K2<114))){

            /* ���强�� �Գ�Գ� �����͸�ֵ*/

            if(Z1+Z2>=2) { k+=10000; continue; }

            else { k+=0; continue; }

        }

        if(((K1>-1&&K1<4)&&((K2>99&&K2<104)||(K2>109&&K2<114)))||((K1>9&&K1<14)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){

            /* ����Ի� ����Գ� ����Ի� ����Գ� �����͸�ֵ*/

            if(Z1==3||Z2==3) { k+=10000; continue; }

            else { k+=a[X2][Y2][Z2]+a[X1][Y1][Z1]/4; continue; } 

        }

        else  { k+=a[X1][Y1][Z1]+a[X2][Y2][Z2]; continue;  }    /* �������͵ĸ�ֵ */

    }
    return k;

}
void PCTurn(u16 *x,u16 *y)           /* �������� *p *q������������ */

{

    u16 i,j,max=0,I,J;        /* I JΪ�µ����� */
	
	int k;

    for(j=0;j<15;j++)

        for(i=0;i<15;i++)

            if(qipan[j][i]==SPA){      /* �������̣������յ�������ֵ��ȡ����ֵ�����ӡ� */

                k=value(i,j);

               if(k>=max) { I=i; J=j; max=k; }

            }

    *x=I; *y=J;

}

