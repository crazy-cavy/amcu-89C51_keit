#include<reg51.h>                    //包含头文件               
sbit led1 = P0^0;     //定义P0口的第一位 为led
void delay(unsigned char x)          //延时函数
{
	unsigned char i,j;
	for(i = 0;i < x;i++)
	for(j = 0;j < 200;j++);
}
void main(void)                     //主函数
{
	P0 = 0xFF;					  	//让P0输出高电平，8个led全灭
	while(1)                        //程序无限循环
	{
		led1=~led1;           //LED1取反；
		delay(250);           //实参不能大于255，因为形参x为char 
	}
}
