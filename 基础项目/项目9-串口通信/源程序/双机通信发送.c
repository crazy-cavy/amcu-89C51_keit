#include <reg51.h>
#define uchar unsigned char
sbit key3=P3^2;
uchar i;
uchar code tb[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delay(int z)			//��ʱ�Ӻ���
{
	int i,j;
	for(i=0;i<z;i++)
	for(j=0;j<500;j++);
}
void init()					//���ڳ�ʼ��
{
	SCON=0X50;
	TMOD=0X20;
	TH1=0XFD;
	TL1=0XFD;
	TR1=1;
}
void main()
{
	init();
	P2=0Xdf;
	while(1)
	{
		for(i=0;i<10;i++)		//����������������
		{	
		SBUF=tb[i];				//�ͻ�����
		P0=tb[i];				//������P0��
		while(TI==0);			//�ж��Ƿ������
		TI=0;					
	    delay(500);
		}	
	}
}