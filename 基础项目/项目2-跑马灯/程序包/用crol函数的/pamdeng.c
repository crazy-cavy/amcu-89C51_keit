#include<reg51.h>                    //包含头文件
#include<intrins.h>                  
void delay(unsigned char x)          //延时函数
{
	unsigned char i,j;
	for(i = 0;i < x;i++)
	for(j = 0;j < 200;j++);
}
void main(void)                     //主函数
{
	unsigned int a = 0xfe ;
	P0 = 0x00;					  	//让P0输出低电平，8个led全灭
	while(1)                        //程序无限循环
	{
		delay(250);                 //实参不能大于255，因为形参x为char 
		a=_crol_(a,1);
	}
}
