#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int 
uchar ge,shi,bai;
uchar tt;
uchar code tb[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; 
uchar code bit_ser[]={0xff,0xdf,0xef,0xf7};   
sbit  DA = P1^5;
sbit CLK = P1^1;
sbit  CS = P1^6;
void timer0_init()									 //T0的初始化函数
{
	TMOD =0x01;
	TL0=(65536-50000)%256;
	TH0=(65536-50000)/256;
	TR0=1;
	ET0=1;
	EA=1;
}

void timer0_isr() interrupt 1						 //T0的中断处理函数，用于显示输出的电压值
{
	TR0=0;
	TL0=(65536-5000)%256;
	TH0=(65536-5000)/256;
	TR0=1;
	tt++;
	if(tt==4)
	tt=1;
	P0 = 0xff;
	P2 = bit_ser[tt];
	switch(tt)
	{
		case 1 : P0=tb[ge];break;
		case 2 : P0=tb[shi];break;
		case 3 : P0=tb[bai]&0x7f;break;
		default : break;	
	}
}
void TLC5615(uint dat)   //由于TLC5615是十位转换，所以在这里要定义一个两字节的变量
{
	 uchar i;
	 dat<<=6;             //两个字节有十六位，去掉高六位剩下十位有效位
	 CLK=0;
	 CS=0;                //初始化
	 for(i=0;i<12;i++)  //根据PDF文档可知,要送的只有十位数但是后面要跟着多加两位零才能将一个数据送出去
	 {
		  DA=(bit)(dat&0x8000); //当dat与0x8000相与然后强制转换后有一位,这时只就只可以直接送给DA端进行发送
		  CLK=1;     //前面一句亦可用DA=CY来代替,但后面的顺序要调换才行
		  dat<<=1;
		  CLK=0;
	 }
	 CS=1;
	 CLK=0;       
}
void main()
{
	uint a;
	uint A_MONI=500;
	timer0_init();
	while(1)
	{
		TLC5615(A_MONI);//最大1023
		a=A_MONI*0.464;//450除以1024，基准电压是2.25V,输出的最大电压是2.25*2,将4.5V分成1024等份，所有输出的电压就是A_MONI*450/1024
		ge=a%10;	   //获取输出电压各位的值
		shi=a/10%10;
		bai=a/100;		
	}
}