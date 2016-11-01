/*

程序描述：M1602的控制芯片为HD44780
PORT 2.0,2.1,2.2则连接到LCD显示器的控制线,
P0口程序执行时将显示河南科技学院相关信息*/
/*******************************************************/
#include <REG51.H>
#include"18B20.c"	
#define uchar unsigned char 
#define uint unsigned int
	
sbit RS = P1^6;
sbit RW = P1^5;
sbit E  = P1^4;

 
code char num[]="0123456789";
code  uchar seven_seg[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code uchar seven_bit[] = {0x20,0x10,0x08,0x04,0x02,0x01};
uchar cp;
uint temp1,temp2;
sbit led = P0^6;    
void timer0_init(void)	 //初始化
{
	TMOD = 0x01;
	TH0 = 0xec;
	TL0 = 0x78;
	TR0 = 1;
	EA  = 1;
	ET0 = 1;
}
/*****************************************************************************/
void timer0_isr(void) interrupt 1		  //中断
{
	uchar j;
	TR0 = 0;
	EA = 0;
	TH0 = 0xec;
	TL0 = 0x78;
	TR0 = 1;
	EA = 1;
	cp++;
	if(cp >= 200)   //刚好1秒
   {
		cp = 0;
		temp1 = Read_Temperature();//得到温度
		if((temp1 & 0x80) == 0x80) //如果温度为负值
		{	
		
			temp2 =0x7f & ~temp1  +  (0x0f & ~c) /15;  //整数处理,需要加上小数来的借位
			c = 0x0f & ~c + 0x01;   //小数处理
		}
		else temp2 = temp1;//如果温度为正值，不用处理
	}
	P0 = seven_bit[j];
	switch(j)
	{
		case 0:P0 |= 0x80;P2 = seven_seg[c * 10/16];P0 &= 0x7f;break;//显示小数
		case 1:P0 |= 0x80;P2 = 0x80 | seven_seg[temp2 % 10];P0 &= 0x7f;break;//显示个位并加上小数点
		case 2:{	
							if((temp2 < 10) && (temp1 < 0x80))//如果温度小于10度，且为正值，十位0不显示
							{
								P0 |= 0x80;       //打开锁存器
								P2  = 0x00;
								P0 &= 0x7f;				//关闭锁存器
								break;
							}
							if((temp1 > 0x80) && (temp2 < 10)){P0 |= 0x80;P2 = 0x40;P0 &= 0x7f;break;}//如果温度小于10度，且为负值，十位0不显示，只显示“-”
								P0 |= 0x80; P2 = seven_seg[temp2 / 10];	P0 &= 0x7f;break;//如果温度大于10度，10位正常显示				
						}
		case 3:{
					if((temp1 > 0x80) && ((temp2 / 10) > 0))//如果温度为负值，且十位不为0，百位显示“-”
					P0 |= 0x80;P2 = 0x40;P0 &= 0x7f;
					break;
				}
	}
	j++;
	if(j >= 3)
	j=0;
}
void write_command(uchar command)
{
	RW = 0;
	RS = 0;
	E = 1;
	P2 = command;
	delay(20);
	E = 0;
	RW = 1;
}
void write_data(uchar date)
{
	RW = 0;
	RS = 1;
	E = 1;
	P2 = date;
	delay(20);
	E = 0;
	RW = 1;
}

void display_string(uchar *p)
{
	while(*p)
	{
		write_data(*p);
		p++;
	}
}
void gotoxy(unsigned y,unsigned x)
{
	if(y == 1)  
		write_command(0x80+x);
	else if(y == 2) 
    	write_command(0x80+0x40+x);
}
 void F1602_init(void)
{
	write_command(0x38); 	// 两行，每行16字符，每个字符5*7点阵	
	write_command(0x0C);		// CURSOR_OFF
	write_command(0x06C);	// CURSOR_RIGHT
}
void F1602_clear()
{
	write_command(0x01);	
	write_command(0x02);	
}	
void main (void)  
{
  	F1602_init();
  	F1602_clear();
		timer0_init();
  	while(1)
  	{ 	
			ET0=0;
			F1602_clear();
			gotoxy(1,1);
  		display_string("AMCU HARDWARE");	
  		gotoxy(2,0);
  		display_string("DEVELOPMENT TEAM");
			ET0=1;
  		delay(3000);
			ET0=0;
			F1602_clear();
			gotoxy(1,3);
  		display_string("AMCU 89C51");
			gotoxy(2,2);
			display_string("STARTER  KIT");
			ET0=1;
			delay(3000);
  	}          
}

