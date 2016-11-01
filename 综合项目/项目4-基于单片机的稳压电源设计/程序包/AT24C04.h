#ifndef AT24C04_10_04_07

sbit ATCLK=P1^6;
sbit SDA=P1^7;


sbit a7=ACC^7;
sbit a6=ACC^6;
sbit a5=ACC^5;
sbit a4=ACC^4;
sbit a3=ACC^3;
sbit a2=ACC^2;
sbit a1=ACC^1;
sbit a0=ACC^0;
/*********************
*******24C04的初始化**
**** **************/
void init_24C04()
{
	SDA=1;
	_nop_();
	ATCLK=1;
	_nop_();
}
/*********************
****启动24C04*********
*******************/
void start_24C04()
{

	SDA=1;
	_nop_();
	ATCLK=1;
	_nop_();

	SDA=0;
	_nop_();
	ATCLK=0;
	_nop_();
}
/********************
*****停止24C04*******
*********************/
void stop_24C04()
{
	SDA=0;
	_nop_();

	ATCLK=1;
	_nop_();
	SDA=1;
	_nop_();

}
/*********************
****24C04应答*********
*******************/
void response()
{
	unsigned char i;
	ATCLK=1;
	_nop_();
	while((SDA==1)&&(i<250))i++;
	ATCLK=0;
	_nop_();	
}
/********************
*****读取24C04一个字节**
**************************/
unsigned char read_byte()
{
	SDA=1;
	ATCLK=1;a7=SDA;ATCLK=0;
	ATCLK=1;a6=SDA;ATCLK=0;
	ATCLK=1;a5=SDA;ATCLK=0;
	ATCLK=1;a4=SDA;ATCLK=0;
	ATCLK=1;a3=SDA;ATCLK=0;
	ATCLK=1;a2=SDA;ATCLK=0;
	ATCLK=1;a1=SDA;ATCLK=0;
	ATCLK=1;a0=SDA;ATCLK=0;
	SDA=1;
	ATCLK=0;
	return ACC;
}
/*************************
**写入24C04一个字节******
*************************/
void write_byte(unsigned char addr)
{
	ACC=addr;
	SDA=a7;ATCLK=1;ATCLK=0;
	SDA=a6;ATCLK=1;ATCLK=0;
	SDA=a5;ATCLK=1;ATCLK=0;
	SDA=a4;ATCLK=1;ATCLK=0;
	SDA=a3;ATCLK=1;ATCLK=0;
	SDA=a2;ATCLK=1;ATCLK=0;
	SDA=a1;ATCLK=1;ATCLK=0;
	SDA=a0;ATCLK=1;ATCLK=0;
	SDA=1;
	ATCLK=0;

}
/***********************
**写24C04的数据*******
*****************/
void write_24C04(unsigned char addr,unsigned char dat)
{
	start_24C04();
	write_byte(0xa0);
	response();
	write_byte(addr);
	response();
	write_byte(dat);
	response();
	stop_24C04();
}
/**********************
***读24C04的数据*******
*****************/
unsigned char read_24C04(unsigned char addr)
{
	unsigned char t;
	start_24C04();
	write_byte(0xa0);
	response();
	write_byte(addr);
	response();
	start_24C04();
	write_byte(0xa1);
	response();
	t=read_byte();
	stop_24C04();
	return t;
}

#endif
