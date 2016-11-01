#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int 
uchar code FFW[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};//转动数组
uchar code REV[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1};
uchar key1_down,key2_down,key3_down,key1_flag,key2_flag,key3_flag;//变量定义
sbit key1 = P3^0;//按键定义
sbit key2 = P3^1;
sbit key3 = P3^2;
/************************按键程序****************************/
/****************按键延迟函数**********/
void delay()
{
	uchar a,b;
	for(a=0;a<25;a++)
	for(b=0;b<255;b++);
}
void key_scan()
{
	/************按键1*************/
	if(key1 == 0)
	{
		delay();
		if(key1 == 0)key1_down = 1;	
	}
	if(key1_down == 1 && key1 == 1)
	{
		key1_flag = 1;
		key2_flag = 0;
		key3_flag = 0;
		key1_down = 0;	
	}
	/************按键2*************/
	if(key2 == 0)
	{
		delay();
		if(key2 == 0)key2_down = 1;	
	}
	if(key2_down == 1 && key2 == 1)
	{
		key1_flag = 0;
		key2_flag = 1;
		key3_flag = 0;
		key2_down = 0;	
	}
	/************按键3*************/
	if(key3 == 0)
	{
		delay();
		if(key3 == 0)key3_down = 1;	
	}
	if(key3_down == 1 && key3 == 1)
	{
		key1_flag = 0;
		key2_flag = 0;
		key3_flag = 1;
		key3_down = 0;	
	}
}
/****************转动延迟函数**********/
void delayB(uint t)
{                           
   uchar k;
   while(t--)
   {
    for(k=0; k<125; k++)
    { }
   }
}
/************************电机正转****************************/
void motor_ffw()
{ 
   	uchar i;
	for (i=0; i<8; i++)       //一个周期转30度
    {
       	P1 = FFW[i];          //取数据
        delayB(100);            //调节转速
    }
 }
/************************电机反转****************************/
void motor_rev()
{
 	uchar i;
	for (i=0; i<8; i++)     //一个周期转30度
    {
       	P1 = REV[i];          //取数据
        delayB(100);            //调节转速
    }
}
/************************主程序****************************/
main()
{ 
   	while(1)
    {  
      	key_scan();
	if(key1_flag == 1)motor_ffw();       //电机正转
	if(key2_flag == 1)motor_rev();		//电机反转
      	if(key3_flag == 1) P1 = 0x0f;			//电机停止
	}
}