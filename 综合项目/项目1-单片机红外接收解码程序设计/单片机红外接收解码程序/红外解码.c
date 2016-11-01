#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar code bit_ser[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf}; 
uchar code seven_seg[] = {0xf9,0x8e,0xc0};	//1,f,0依次表示高电平，悬空，低电平
uchar code address[]={1,1,1,1,1,1};	//存放预设的地址码高电平为2，低电平为0，悬空为1
uchar disbuffer[6];	   //数据显示缓冲区
uchar DATEA[25];	   //接收的信息
uchar DATE[12];		   //解码得到的数据
bit dateflag; 		   //接收数据时的标记
char num,i,j;		   //用于接收和显示时计数
sbit DATE_IN = P1^0;   //数据的接收端
//定时器初始化函数
void Timer_init()
{
	TMOD = 0x11;
	TH0 = 0xFA;
	TL0 = 0x00;
	EA = 1;
	ET0 = 1; 
	TL1 = (65536-5000) % 256;
	TH1 = (65536-5000) / 256;
	ET1 = 1;
	TR1 = 1;
}
//Timer1中断处理函数，用于显示接收到的数据
void time1_interrupt() interrupt 3
{
	TL1 = (65536-5000) % 256;
	TH1 = (65536-5000) / 256;
	P0 = 0xff;
	P2 = bit_ser[i];
	P0 = seven_seg[disbuffer[i]];
	i ++;
	if(i >= 6)
		i = 0;
}
//Timer0中断处理函数
void timer0_interrupt() interrupt 1
{
	if(dateflag)		//如果dateflag为高，说明是需要检测数据了，否则说明是同步码或误码（周期过长）
	{
		dateflag = 0;
		DATEA[num] = DATE_IN;
		num ++;
		TR0 = 0;
		TH0 = 0xFA;
		TL0 = 0x00;
		TR0 = 1;
	}
	else			 
	{
		num = 0;
	}
}
//数据接收函数
void jieshou_date()
{
	if(!dateflag)
	{
		TR0 = 0;
		TH0 = 0xFA;
		TL0 = 0x90;																									  
		TR0 = 1;
		dateflag = 1; 					
	}	
}
//从接收到的信息中提取数据
void huoqu_date()
{
	for(j = 0;j < 12;j ++)	//将接收到的24位电平信号转换成数据
	{
		DATE[j] = DATEA[j + j] + DATEA[j + j + 1];
		if(DATE[j] && ! DATEA[j + j])	 //如果接收到的两位数据是0，1说明是误码
		{
			break;
		}
	}
	num = 0;
	if(j == 12)
	{
		if(DATE[0] == address[0] && DATE[1] == address[1]&& DATE[2] == address[2] &&
			DATE[3] == address[3] && DATE[4] == address[4] && DATE[5] == address[5])
		{		 //如果解码得到的地址码和预设的地址码相同，就将解码得到的数据放入显示缓冲区
			for(j = 0;j < 6; j++)
			{
				disbuffer[j] = DATE[6 + j];
			}	
		}
	}	
}
//主函数
void main()
{
	Timer_init();
	while(1)
	{
		if(!DATE_IN)		 //说明有信号输入
		{
			jieshou_date();	 //接收信息
		} 
		if(num >= 25)		 //说明已经接收到了一组完整数据
		{
			huoqu_date();	 //从接收的信息中提取数据			
		}					
	}
}