#include<reg51.h>

#define uchar unsigned char
uchar dot,time1[6],tt,flash;
code seven_tab[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
code bit_select[6] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf};

sbit rtc_clk=P1^0;
sbit rtc_data=P1^1;
sbit rtc_rst=P1^2;

sbit a0=ACC^0;
sbit a7=ACC^7;

void write_rtc(uchar date)	   //写一字节
{
	uchar i;
	ACC=date;
	for(i=8;i>0;i--)
		{
			rtc_data=a0;
			rtc_clk=1;
			rtc_clk=0;
			ACC=ACC>>1;	
		}
}

uchar read_rtc()		//读一字节
{
	uchar i;
	for(i=8;i>0;i--)
		{
			ACC=ACC>>1;
			a7=rtc_data;
			rtc_clk=1;
			rtc_clk=0;		
		}
	return(ACC);	
}

void write1302(uchar address,uchar date)	//写1302数据
{
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	write_rtc(address);
	write_rtc(date);
	rtc_clk=1;
	rtc_rst=0;
}
uchar read1302(uchar address)				//读1302数据
{
	uchar temp;
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	write_rtc(address);
	temp=read_rtc();
	rtc_clk=1;
	rtc_rst=0;
	return(temp);
}
void init1302()		//1302初始化
{
	write1302(0x8e,0x00);	//写操作
//	write1302(0x90,0xa5);
	write1302(0x80,0x56);	//写秒
	write1302(0x82,0x34);	//写分
	write1302(0x84,0x12);	//写时
	write1302(0x86,0x25);	//写月
	write1302(0x88,0x08);	//写日
	write1302(0x8a,0x05);	//写星期
	write1302(0x8c,0x09);	//写年
	write1302(0x8e,0x80);	//写保护
}
void get_time()				//获取1302的时间数据（时、分、秒），存入time1数组中
{
	uchar d;
	d=read1302(0x81);
	time1[0]=d&0x0f;
	time1[1]=(d>>4)&0x0f;	
	d=read1302(0x83);
	time1[2]=d&0x0f;
	time1[3]=(d>>4)&0x0f;	
	d=read1302(0x85);
	time1[4]=d&0x0f;
	time1[5]=(d>>4)&0x0f;	
}
void time0() interrupt 1	 //利用中断对数码管上显示的数据进行刷新
{
	uchar i;
	TR0=0;
	TH0=(65536-5000)/256;	
	TL0=(65536-5000)%256;
	TR0=1;
	tt++;
	if(tt==100)
	{
		tt=0;
		dot=!dot;	
	}
	get_time();
	flash=0x80*dot;
	flash=flash|0x7f;
	P0=0xff;
	P2=bit_select[i];
	if(i==2 || i==4)
	P0=seven_tab[time1[i]]&flash;
	else
	P0=seven_tab[time1[i]];
	i++;
	if(i==6)i=0;			
}
void initmcu()			//mcu初始化
{
	TMOD=0x01;
	TH0=(65536-5000)/256;	
	TL0=(65536-5000)%256;
	TR0=1;
	ET0=1;
	EA=1;
}
void main()
{
	initmcu();
	init1302();
	while(1)
		{
			
		}
	
}
