#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define nop _nop_()
sbit sda=P1^7;   //SDA�͵�Ƭ����P17������
sbit scl=P1^1;   //SCL�͵�Ƭ����P16������
//����ACC��λ������ACC�����ٶ����   
sbit a0=ACC^0;
sbit a1=ACC^1;
sbit a2=ACC^2;
sbit a3=ACC^3;
sbit a4=ACC^4;
sbit a5=ACC^5;
sbit a6=ACC^6;
sbit a7=ACC^7;
//��ʼ����
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
//ֹͣ����
void stop()
{
	sda=0;
	nop;
	scl=1;
	nop;
	sda=1;
	nop;
}
//��Ӧ����
void ack()
{
	uchar i;
	scl=1;
	nop;
	while((sda==1) && (i<250))i++;
	scl=0;
	nop;
}
//дһ���ֽں���
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
//��һ���ֽں���
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
//д��ַ�����ݺ���
void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);//д2404��ַ����
	ack();
	write_byte(address);//д��ַ
	ack();
	write_byte(date);//д����
	ack();
	stop();
}
//����ַ�����ݺ���
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
	init();            //��ʼ������
	write_add(5,0x77);  //����ַ5��д��0xaa
	delay(100);
	P0=read_add(5);   //����ַ5�е����ݣ�����P0�����������������ʾ	
	while(1);         //����ѭ��
}
