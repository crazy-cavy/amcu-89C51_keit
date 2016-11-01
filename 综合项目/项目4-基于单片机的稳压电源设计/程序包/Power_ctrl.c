#include <REG51.H>
#include "intrins.h"
#include "AT24C04.h"

#define  uchar unsigned char
#define  uint unsigned int

uchar code  LED[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code Bit_sel[4] = {0x08,0x04,0x02,0x01 }; //各个数码管对应的位选数据

sbit DIO = P1^0;	 	//数据线
sbit CS = P1^1;		 	//片选
sbit CLK = P1^2;		//io口时钟

sbit SCS = P1^4;
sbit SDATA = P1^5;
sbit SCLK = P1^3;

sbit ADD = P3^0;
sbit SUB = P3^1;
sbit Pre_read = P3^2;
sbit Store = P3^3;

uint qian,bai,shi,ge;  //用于显示数码管的 千，百，十，个 等四位的显示
uint val,num;		   //val 是用于输入DA  的数据，num是用于判断是不是长按的
uint cp;			   //计数的变量
uchar key_stat;
uchar add_stat,sub_stat;
uchar st_flag,pre_flag;

void delay(uint x)	 	//微妙级是延时函数
{
	while(x--)
		_nop_();
}

/**********************************************
	函数名称：deal
	函数作用：处理AD的返回值
	函数参数：无
	函数返回值：无
**********************************************/											   
void deal(uint num)	  //显示程序
{	
	qian=num/1000;	      //千，百，十，个处理
	bai=num/100%10;
	shi=num/10%10;
	ge=num%10;
} 

/**********************************************
	函数名称：TL549_AD()
	函数作用：返回AD的返回值
	函数参数：无
	函数返回值：data_ad
**********************************************/	
uint TL549_AD()		    //TLC549处理
{
	uchar i;
	uint data_ad = 0;
	CS = 1;				//初始化，启动
	CLK = 0;
	CS = 0;
	_nop_();		    
	for(i = 0;i < 8;i++)	//读取采集数据，读取的是上一次采集数据
	{
		CLK = 1;	 
		if(DIO)data_ad |= 0x01;
		CLK = 0;  
		data_ad = data_ad << 1;  
	} 	
	CS = 1;
	data_ad = data_ad  * (500/ 256);   
	return(data_ad); 
}

/**********************************************
	函数名称：TLC5615_DA(uint da)
	函数作用：TLC5615_DA 将da转换后模拟输出
	函数参数：da
	函数返回值：无
**********************************************/	
void TLC5615_DA(uint da) //	TLC5615 的DA 转换函数
{
	uchar i;
	da<<=6;

	SCS=0;
	SCLK=0;

	for (i=0;i<12;i++)
	{
		SDATA=(bit)(da&0x8000);
		SCLK=1;
		da<<=1;
		SCLK=0;
	}

	SCS=1;
	SCLK=0;
	for (i=0;i<12;i++);
}
/**********************************************
	函数名称：key_scan()
	函数作用：处理那些独立键盘
	函数参数：无
	函数返回值：无
**********************************************/	
void key_scan()
{
	if (ADD == 0)						   // ADD 按键的键盘处理函数
	{
		delay(10);
		if (ADD == 0)
		{
			add_stat = 1;
			num ++;
		}
		else
		{
			add_stat = 0;
			num = 0;
		}
		//此处判断是不是长按，长按的话 使其步进值 加大
		if (ADD == 0 && add_stat == 1 && num >= 300)  
		{
			val += 5;
			num = 0;
		}
		if (ADD == 1 && add_stat == 1)
		{
			val ++;
			num = 0;
			add_stat = 0;
		}
		if (val >= 1024)
		{
			val = 1023;
		}
	}

	if (SUB == 0)							//SUB 按键的键盘处理函数
	{
		delay(10);
		if (SUB == 0)
		{
			sub_stat = 1;
			num ++;
		}
		if (SUB == 0 && sub_stat == 1 && num >= 300)
		{
			val -= 5;
			num = 0;
		}
		if (SUB == 1 && sub_stat == 1)
		{
			val --;
			num = 0;
			sub_stat = 0;
		}
		if (val <= 0)
			val = 0;
	}
									
	if (Pre_read == 0)			   			//预读数据的 键盘处理函数
	{
		delay(100);
		if (Pre_read == 0)
		{
			pre_flag = 1;
		}
		if (Pre_read == 1 &&pre_flag == 1 )
		{
			pre_flag = 0;
			val = read_24C04(20);			//从AT24C04中的地址20 中读出预存储的数据
		}
	}

	if (Store == 0)						//保存数值 按键的键盘处理函数
	{
		delay(100);
		if (Store == 0)
		{
			st_flag = 1;
		}
		if (Store == 1 && st_flag == 1)
		{
			st_flag	= 0;
			write_24C04(20,val);		 //向AT24C04中的地址20 写入存储的数据
		}
	}
}
/**********************************************
	函数名称：timer0_init (void)
	函数作用：初始化定时器0，并设置
	函数参数：无
	函数返回值：无
**********************************************/	
void timer0_init (void)            		// timer0中断初始化函数
{
	EA = 0;                      
	TMOD = 0x01;             
	TR0 = 0;                      
	TL0 = (65536-5000)%256;		 //设置计数器初值
	TH0 = (65536-5000)/256;                  
	PT0 = 1;                      
	ET0 = 1;                     
	EA = 1;                      
	TR0 = 1;                      
}
/**********************************************
	函数名称：main(void)
	函数作用：main主函数入口
	函数参数：无
	函数返回值：无
**********************************************/							
void main(void)				 //主程序
{  
	timer0_init();			//初始化定时器0
	init_24C04();			//初始化AT24C04
	while(1)
	{ 
		key_scan();			//调用键盘扫描函数
		TLC5615_DA(val);	//处理键盘发送过来的值
	} 
}

/**********************************************
	函数名称：timer0_isr(void)  interrupt 1
	函数作用：定时器0，方式1，的中断服务子程序
	函数参数：无
	函数返回值：无
**********************************************/	
void timer0_isr(void)  interrupt 1       	// timer0中断服务函数
{
                         					//数码管的位选变量
	TR0 = 0;                       		 	//停止计数
	TL0 = (65536-5000)%256;					//重新载入计数器初值
	TH0 = (65536-5000)/256; 
	cp++;                                	//位循环变量加1
	if(cp >= 4)
		cp = 0; 
	deal(TL549_AD());                       //循环显示1次，j清零
   	TR0 = 1;

	P0=0xff;  //与j对应，P2输出数码管的位选信号
	switch(cp)
	{
		case 0: P0 = LED[ge]; break;   	
		case 1: P0 = LED[shi]; break;   	
		case 2: P0 = LED[bai]&0x7f; break;
		case 3: P0 = LED[qian]; break;  	
	} 
	P2 = Bit_sel[cp]; 
}  