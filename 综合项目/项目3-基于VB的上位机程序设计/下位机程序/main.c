/********该程序主要是利用DS18B20采集温度，然后通过数码管显示温度*************/
/*当程序收到上位机发送的命令之后，该程序会将当时的温度值通过串口发送给上位机*/
#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit DQ = P1^0;		//定义DS18B20的信号线端口
uchar i,j;
uchar dis_buffer[4];//定义数据缓冲数组
uchar bit_ser[]={0xfe,0xfd,0xfb,0xf7}; //定义数码管片选数组
uchar seven_seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
									   //定义数码管段选数组
/****************************延时函数*********************************/
void delay(uint x)
{
	while(x)   
	x--;
}
/*************************DS18B20初始化函数***************************/
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
/***************************从18B20中读一个字节************************/
uchar ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
 	{
  		DQ = 0; 	// 给脉冲信号
  		dat>>=1;
  		DQ = 1; 	// 给脉冲信号
  		if(DQ)
   		dat|=0x80;  
  		delay(8);
 	}
 	return(dat);
}
/***************************向18B20中写一个字节************************/
Write_OneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;	  //给脉冲信号
		DQ = dat & 0x01;
		delay(5); 
		DQ = 1;	  //给脉冲信号
		dat >>= 1;
	}
	delay(4);
}
/**************************从18B20中读取一个字节***********************/
int Read_Temperature(void)
{
	uchar i = 0,t = 0,a,b;
	int temp;
	Init_DS18B20();
	Write_OneChar(0xcc); 	// 跳过读序号列号的操作
	Write_OneChar(0x44); 	// 启动温度转换
	Init_DS18B20();
	Write_OneChar(0xcc); 	//跳过读序号列号的操作
	Write_OneChar(0xbe); 	//读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	i = ReadOneChar();   	//读取温度值低位
	t = ReadOneChar();   	//读取温度值高位
	a = i & 0x0f;
	b = t;
	i = i >> 4;             //低位右移4位，舍弃小数部分
	t = t << 4;             //高位左移4位，舍弃符号位
	t = t | i;
	temp = (t + a * 0.0625) * 100; //得到一个比实际温度扩到100倍的值，主要是为了更好的显示和传输          
	return(temp);			//返回温度值
}

/***************************初始化定时器0******************************/
void timer0_init(void)			 //
{
	TMOD = 0x21; //由于串口通信需要使用定时器1，因此TMOD的值是0x21
	TL0 = (65536-5000) % 256;
	TH0 = (65536-5000) / 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
/*************************发送数据的函数********************************/
void txd_data(char send_data)	
{
	SBUF = send_data; //将需要发送的数据放入发送缓冲区
	while(!TI);		  //等待发送数据
	TI = 0;
}
/********************T0中断处理函数，主要用于显示当前温度***************/
void timer0_isr(void) interrupt 1
{
	int temp; 
	TR0 = 0;
	TL0 = (65536-5000) % 256;
	TH0 = (65536-5000) / 256;
	TR0 = 1;
	switch(i)
	{
		case 0:
			P2 = bit_ser[0];
			P0 = seven_seg[dis_buffer[0]];				
			break;
		case 1:
			P2 = bit_ser[1];
			P0 =seven_seg[dis_buffer[1]] & 0x7f;
			break;
		case 2:
			P2 = bit_ser[2];
			P0 =seven_seg[dis_buffer[2]];
			break;
		case 3:
			P2 = bit_ser[3];
			P0 =seven_seg[dis_buffer[3]];
			break;
	} 
	i++;
	if(i >= 4) 
	{
		i = 0;
		j++;
		if(j >= 10)	//如果到200ms就会读取一次温度，并将温度值放入显示缓冲区
		{
			j = 0;
			temp = Read_Temperature();
			dis_buffer[0] = temp / 1000;
		   	dis_buffer[1] = temp % 1000 / 100;
			dis_buffer[2] = temp % 100 / 10;
			dis_buffer[3] = temp % 10;
		}
	}
}

/***************************串口通信初始化函数***************************/
void uart_init(void)
{ 
	SCON = 0x50;         //方式1,充许接收
	TMOD = 0x21;         //T1方式2定时，T0方式是1          
	TH1 = 0xFd;          //波特率9600,Fosc=11.0592MHz
	TL1 = 0xFd;
	TR1 = 1;
	ES = 1;           	 // 打开串口中断
}
/**************************串口中断处理函数*****************************/
void uart() interrupt 4			  
{
	char y,i;
	while(!RI);
	y = SBUF;			 //读取接收的信息，然后判断是否为发送温度命令
	if(y == '0')		 //如果为发送命令，就将当前数据缓冲数组内的数据发送给上位机
	{
		for(i = 0;i <= 3;i++)
		{
			txd_data(dis_buffer[i] + 48);
		}
	}
	RI = 0;		
}
/*******************************主函数*********************************/
void main()
{
	timer0_init();		//调用T0初始化函数
	uart_init();		//调用串口初始化函数
	while(1)
	{}
}