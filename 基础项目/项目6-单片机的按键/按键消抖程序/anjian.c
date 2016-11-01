#include<reg51.h> 
#define uchar unsigned char
uchar code  seven_seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit key1 = P3^4;
sbit key2 = P3^5;
uchar key1_state,key2_state;       //按键按下的状态变量
void delay(uchar i)
{
	while(i) i--; 
}
void key_scan(void)              //键盘扫描函数
{	
	if(key1 == 0)                //如果按键按下
	{	
		delay(200);              //延时一段时间，消除键抖
		while(key1 == 0);
	   key1_state = 1;           //再对按键检测，如果确实按下，按键状态变量为1
	}
	if(key2 == 0)
	{	
		delay(200);
		while(key2 == 0)
 		key2_state = 1;
	}
}
void main(void)
{	
	char i;
	while(1)
	{
		key_scan();                        //调用键盘扫描函数
		if(key1_state == 1)                //判断键盘状态变量，如果为1，说明有按键按下
		{
			i++;                            //显示数据增加
			key1_state = 0;                 //数据增加后，键盘状态变量清零
			if(i>9) i = 0;                  //数据增加超过9后，回到0
		}
		if(key2_state == 1)
		{	
			i--;
			key2_state = 0; 
			if(i < 0) i = 9;
		}
		P0 = seven_seg[i];                  //显示键盘调整数据
		P2 = 0xfe;									//数码管SEG0显示
	}
}
