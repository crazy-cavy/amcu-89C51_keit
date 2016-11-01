/*********************************************************************/
//读取DS18B20温度，通过数码管显示，在温度超过35度时继电器吸合																						
/*********************************************************************/
#include<reg51.h>		
#define uchar unsigned char 
#define uint unsigned int
uchar a,b,c;
sbit DQ = P1^0;
void delay(uint x)
{
	while(x--); 
 
}
void Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1;         //DQ复位
	delay(8);       //稍做延时
	DQ = 0;    	    //单片机将DQ拉低
	delay(80);   	//精确延时 大于 480us
	DQ = 1;      	//拉高总线
	delay(14);
	x=DQ;      	    //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delay(20);
}
/******************************从18B20中读一个字节****************************/
uchar Read_OneChar(void)
{
	uchar i = 0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	{
  		DQ = 0;          // 给脉冲信号
  		dat >>= 1;
  		DQ = 1;          // 给脉冲信号
  		if(DQ)
  		dat |= 0x80;
  		delay(8);
	}
	return(dat);
}
/******************************向18B20中写一个字节****************************/
void Write_OneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		delay(10);
		DQ = 1;
		dat >>= 1;
	}
	delay(8);
}
/***********************************读取温度**********************************/
uchar Read_Temperature(void)
{
	uchar i = 0,t = 0;
	Init_DS18B20();
	Write_OneChar(0xcc); 	// 跳过读序号列号的操作
	Write_OneChar(0x44); 	// 启动温度转换
	Init_DS18B20();
	Write_OneChar(0xcc); 	//跳过读序号列号的操作
	Write_OneChar(0xbe); 	//读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a = Read_OneChar();   	//读取温度值低位
	b = Read_OneChar();   	//读取温度值高位
	c = 0x0f & a;			//得到小数部分
	a = a >> 4;				//低位右移4位
	b = b << 4;             //高位左移4位
    t = a | b;				//得到8位温度的整数部分，最高为符号位
	return(t);
}


void write_temprom(uchar temp_up_set,uchar temp_down_set )//保存温度上下限temp_up_set,temp_down_set;
{
 	Init_DS18B20();
	Write_OneChar(0xcc);      		//跳过读序号列号的操作
	Write_OneChar(0x4e); 	  		//写准备 
	Write_OneChar(temp_up_set);	    //向18B20的暂存写上限		 
	Write_OneChar(temp_down_set);   //向18B20的暂存写下限
	Write_OneChar(0x7f);
	Init_DS18B20();
	Write_OneChar(0xcc); 
	Write_OneChar(0x48); 	  		//向18B20的rom中写数据
	Init_DS18B20();
	Write_OneChar(0xcc);
	Write_OneChar(0xb8);
	;;
}
uchar  temp_up,temp_down;//读出存储在DS18B20内的温度上下限值
void read_temprom(void)//读取温度温度上下限
{
   	uchar i;
	Init_DS18B20();
	Write_OneChar(0xcc);	            // 跳过读序号列号的操作        	
	Write_OneChar(0xbe); 				// 读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	i = Read_OneChar();	  				//读0地址寄存器，扔掉
	i = Read_OneChar();  				//读1地址寄存器，扔掉
	temp_up = Read_OneChar();			//读2地址寄存器
	temp_down = Read_OneChar();			//读3地址寄存器	
}	
