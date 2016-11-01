/**************************************************************************/
//            通过定时器中断，实现六位数码管的动态显示
/**************************************************************************/
#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
uchar j = 0, sec,min,hour;
uint i;
bit flash;
sbit dop = P0^7;   //小数点段
code uchar seven_seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
code uchar seven_bit[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf};
void timer0_init(void)	  //初始化
{
	TMOD = 0x01;
	TH0 = 0xf8;           //(65536 - 2000)/ 256;
	TL0 = 0x30;           //(65536 - 2000)% 256;
	TR0 = 1;
	EA  = 1;
	ET0 = 1;
}
void timer0_isr(void) interrupt 1		  //中断
{
	TR0 = 0;
	EA = 0;
	TH0 = 0xf8;
	TL0 = 0x30;
	TR0 = 1;
	EA = 1;
	P0 = 0xff;		//让数码管不显示内容，防止信号串扰
	i++;
	if(i >= 500)   	//刚好1秒
   	{
		i = 0;
		sec++;
		flash = !flash;	//2秒改变一次状态，如果想让flash1秒改变一次状态怎样做？
	}
	if(sec >= 60)
 	{
 		sec = 0;
 		min++;
 	}
 	if(min >= 60)
	{
		min = 0;
		hour++;
	}
	if(hour >= 24)
		hour = 0; 
	switch(j)
	{
		case 0: P0 = seven_seg[sec % 10]; break;	//显示秒个位
		case 1: P0 = seven_seg[sec / 10]; break;
		case 2: P0 = seven_seg[min % 10];dop = flash;break; 
		case 3: P0 = seven_seg[min / 10]; break;
		case 4: P0 = seven_seg[hour % 10];dop = flash; ;break;
		case 5: P0 = seven_seg[hour / 10]; break;	
	}
	P2 = seven_bit[j];	//每中断一次只显示一位数码管，显示停留时间为中断间隔
	j ++;
	if(j >= 6)	
	j=0;  //循环变量清零
}
void main(void)
{
	sec = 30;
	min = 30;
	hour = 8;
	P2 = 0xff;
	timer0_init();
	while(1);
}