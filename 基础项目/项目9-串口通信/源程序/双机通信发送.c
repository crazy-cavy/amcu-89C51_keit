#include <reg51.h>
#define uchar unsigned char
sbit key3=P3^2;
uchar i;
uchar code tb[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delay(int z)			//延时子函数
{
	int i,j;
	for(i=0;i<z;i++)
	for(j=0;j<500;j++);
}
void init()					//串口初始化
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
		for(i=0;i<10;i++)		//连续发送数组数据
		{	
		SBUF=tb[i];				//送缓冲区
		P0=tb[i];				//数据送P0口
		while(TI==0);			//判断是否发送完成
		TI=0;					
	    delay(500);
		}	
	}
}