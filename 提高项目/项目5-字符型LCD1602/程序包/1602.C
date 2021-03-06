/*

程序描述：M1602的控制芯片为HD44780
PORT 2.0,2.1,2.2则连接到LCD显示器的控制线,
P0口程序执行时将显示河南科技学院相关信息*/
/*******************************************************/
#include <REG51.H>
#define uchar unsigned char


sbit RS = P1^6;
sbit RW = P1^5;
sbit E  = P1^4;
 
char code num[]="0123456789";
void delay(unsigned int n)
{

   unsigned int j=0;
   for(;n>0;n--)
   { 
	  for(j=0;j<125;j++); 
   }
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
  	while(1)
  	{ 	
			F1602_clear();
			gotoxy(1,1);
  		display_string("AMCU HARDWARE");	
  		gotoxy(2,0);
  		display_string("DEVELOPMENT TEAM");
  		delay(3000);
			F1602_clear();
			gotoxy(1,3);
  		display_string("AMCU 89C51");
			gotoxy(2,2);
			display_string("STARTER  KIT");
			delay(3000);
  	}          
}

