#include<reg51.h>
#define uint unsigned int
void delay(uint x)
{
	while(x--);
}
void main(void)
{
	uint aa;
	aa = 0x01;			   //8位只有一位为高，其他为低
	while(1)
	{
		delay(10000);	   //调用延时函数
		P0 = ~aa;        //P0为aa取反值，8位只有一位为高，其他为低   从原理图看出 IO口输出低电平led亮
		aa = aa << 1;	   //第一次循环，led1亮，其他都不亮
		if(aa == 0x00)     //全部移出
		aa = 0x01;
	}
}