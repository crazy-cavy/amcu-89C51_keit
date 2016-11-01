#include <REGX51.H>
#include "intrins.h"
#define  uchar unsigned char
#define  uint	  unsigned int
uchar code  tb[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code seg_bit_scan[] = {0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; //各个数码管对应的位选数据
sbit data_temp = P1^5;	 //数据线
sbit cs = P1^4;		 	//片选
sbit sclk = P1^1;		 //io口时钟
uchar qian,bai,shi,ge;
uint j;
void chuli(uint num)	  //显示程序
{
  
  qian=num/1000;	      //千，百，十，个处理
  bai=num/100%10;
  shi=num/10%10;
  ge=num%10;
} 
uint ad_549()		    //TLC549处理
{
  uchar i;
  uint data_ad = 0;
  cs = 1;				//初始化，启动
  sclk = 0;
  cs = 0;
  _nop_();		    
  for(i = 0;i < 8;i++)	//读取采集数据，读取的是上一次采集数据
    {
	  sclk = 1; 
	 
	  if(data_temp)data_ad |= 0x01;
	  sclk = 0;  
	  data_ad = data_ad << 1;  
	} 	
  cs = 1;
  data_ad = data_ad  * (500/ 256);   
  return(data_ad); 
}
void timer0_isr(void)  interrupt 1       			 // timer0中断服务函数
{
                         							//数码管的位选变量
	TR0 = 0;                       		 			//停止计数
	TL0 = (65536-10000)%256;
	TH0 = (65536-10000)/256; 
	j++;                                			//位循环变量加1
	if(j >= 4)
	j = 0;                          				//循环显示1次，j清零
   	TR0 = 1;
}
void timer0_init (void)            		// timer0中断初始化函数
{
	EA = 0;                      
	TMOD = 0x01;             
	TR0 = 0;                      
	TL0 = (65536-10000)%256;
	TH0 = (65536-10000)/256;                  
	PT0 = 1;                      
	ET0 = 1;                     
	EA = 1;                      
	TR0 = 1;                      
} 						
void main(void)				 //主程序
{  

	uint i;
	timer0_init ();
	while(1)
	{  
		
		i = ad_549();
		
	   	chuli(i);	  //显示最终结果
		P0=0xff;
		switch(j)
		{
			case 0: P0 = tb[ge]; break;   	
			case 1: P0 = tb[shi]; break;   	
			case 2: P0 = tb[bai]&0x7f; break;
			case 3: P0 = tb[qian]; break;  	
		} 
		P2 = seg_bit_scan[j];                    //与j对应，P2输出数码管的位选信号
		} 

	}

  