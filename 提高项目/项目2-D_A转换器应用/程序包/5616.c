#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int 
uchar ge,shi,bai;
uchar tt;
uchar code tb[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; 
uchar code bit_ser[]={0xff,0xdf,0xef,0xf7};   
sbit  DA = P1^5;
sbit CLK = P1^1;
sbit  CS = P1^6;
void timer0_init()									 //T0�ĳ�ʼ������
{
	TMOD =0x01;
	TL0=(65536-50000)%256;
	TH0=(65536-50000)/256;
	TR0=1;
	ET0=1;
	EA=1;
}

void timer0_isr() interrupt 1						 //T0���жϴ�������������ʾ����ĵ�ѹֵ
{
	TR0=0;
	TL0=(65536-5000)%256;
	TH0=(65536-5000)/256;
	TR0=1;
	tt++;
	if(tt==4)
	tt=1;
	P0 = 0xff;
	P2 = bit_ser[tt];
	switch(tt)
	{
		case 1 : P0=tb[ge];break;
		case 2 : P0=tb[shi];break;
		case 3 : P0=tb[bai]&0x7f;break;
		default : break;	
	}
}
void TLC5615(uint dat)   //����TLC5615��ʮλת��������������Ҫ����һ�����ֽڵı���
{
	 uchar i;
	 dat<<=6;             //�����ֽ���ʮ��λ��ȥ������λʣ��ʮλ��Чλ
	 CLK=0;
	 CS=0;                //��ʼ��
	 for(i=0;i<12;i++)  //����PDF�ĵ���֪,Ҫ�͵�ֻ��ʮλ�����Ǻ���Ҫ���Ŷ����λ����ܽ�һ�������ͳ�ȥ
	 {
		  DA=(bit)(dat&0x8000); //��dat��0x8000����Ȼ��ǿ��ת������һλ,��ʱֻ��ֻ����ֱ���͸�DA�˽��з���
		  CLK=1;     //ǰ��һ�������DA=CY������,�������˳��Ҫ��������
		  dat<<=1;
		  CLK=0;
	 }
	 CS=1;
	 CLK=0;       
}
void main()
{
	uint a;
	uint A_MONI=500;
	timer0_init();
	while(1)
	{
		TLC5615(A_MONI);//���1023
		a=A_MONI*0.464;//450����1024����׼��ѹ��2.25V,���������ѹ��2.25*2,��4.5V�ֳ�1024�ȷݣ���������ĵ�ѹ����A_MONI*450/1024
		ge=a%10;	   //��ȡ�����ѹ��λ��ֵ
		shi=a/10%10;
		bai=a/100;		
	}
}