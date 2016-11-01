#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define nop _nop_()
sbit sda=P1^7;   //SDA和单片机的P17脚相连
sbit scl=P1^1;   //SCL和单片机的P16脚相连
//定义ACC的位，利用ACC操作速度最快   
sbit a0=ACC^0;
sbit a1=ACC^1;
sbit a2=ACC^2;
sbit a3=ACC^3;
sbit a4=ACC^4;
sbit a5=ACC^5;
sbit a6=ACC^6;
sbit a7=ACC^7;
//开始函数
void start()
{
	sda=1;
	nop;
	scl=1;
	nop;
	sda=0;
	nop;
	scl=0;
	nop;
}
//停止函数
void stop()
{
	sda=0;
	nop;
	scl=1;
	nop;
	sda=1;
	nop;
}
//响应函数
void ack()
{
	uchar i;
	scl=1;
	nop;
	while((sda==1) && (i<250))i++;
	scl=0;
	nop;
}
//写一个字节函数
void write_byte(uchar dd)
{
	ACC=dd;
	sda=a7;scl=1;scl=0;
	sda=a6;scl=1;scl=0;
	sda=a5;scl=1;scl=0;
	sda=a4;scl=1;scl=0;
	sda=a3;scl=1;scl=0;
	sda=a2;scl=1;scl=0;
	sda=a1;scl=1;scl=0;
	sda=a0;scl=1;scl=0;
	sda=1;
}
//读一个字节函数
uchar read_byte()
{
	sda=1;
	scl=1;a7=sda;scl=0;
	scl=1;a6=sda;scl=0;
	scl=1;a5=sda;scl=0;
	scl=1;a4=sda;scl=0;
	scl=1;a3=sda;scl=0;
	scl=1;a2=sda;scl=0;
	scl=1;a1=sda;scl=0;
	scl=1;a0=sda;scl=0;
	sda=1;
	return(ACC);
}
//写地址和数据函数
void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);//写2404地址命令
	ack();
	write_byte(address);//写地址
	ack();
	write_byte(date);//写数据
	ack();
	stop();
}
//读地址、数据函数
uchar read_add(uchar address)
{
	uchar temp;
	start();
	write_byte(0xa0);		  //
	ack();
	write_byte(address);	  //
	ack();
	start();
	write_byte(0xa1);
	ack();
	temp=read_byte();
	stop();
	return(temp);
}
void delay(uchar i)
{
	uchar a,b;
	for(a=0;a<i;i++)
		for(b=0;b<100;b++);
}
void init()
{
	sda=1;
	nop;
	scl=1;
	nop;
}
void main()
{
	init();            //初始化函数
	write_add(5,0x77);  //往地址5中写入0xaa
	delay(100);
	P0=read_add(5);   //读地址5中的数据，并送P0口驱动发光二极管显示	
	while(1);         //无限循环
}
