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

void write_rtc(uchar date)	   //дһ�ֽ�
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

uchar read_rtc()		//��һ�ֽ�
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

void write1302(uchar address,uchar date)	//д1302����
{
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	write_rtc(address);
	write_rtc(date);
	rtc_clk=1;
	rtc_rst=0;
}
uchar read1302(uchar address)				//��1302����
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
void init1302()		//1302��ʼ��
{
	write1302(0x8e,0x00);	//д����
//	write1302(0x90,0xa5);
	write1302(0x80,0x56);	//д��
	write1302(0x82,0x34);	//д��
	write1302(0x84,0x12);	//дʱ
	write1302(0x86,0x25);	//д��
	write1302(0x88,0x08);	//д��
	write1302(0x8a,0x05);	//д����
	write1302(0x8c,0x09);	//д��
	write1302(0x8e,0x80);	//д����
}
void get_time()				//��ȡ1302��ʱ�����ݣ�ʱ���֡��룩������time1������
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
void time0() interrupt 1	 //�����ж϶����������ʾ�����ݽ���ˢ��
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
void initmcu()			//mcu��ʼ��
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
