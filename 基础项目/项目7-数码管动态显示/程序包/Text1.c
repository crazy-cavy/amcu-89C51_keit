/**************************************************************************/
//            ͨ����ʱ���жϣ�ʵ����λ����ܵĶ�̬��ʾ
/**************************************************************************/
#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
uchar j = 0, sec,min,hour;
uint i;
bit flash;
sbit dop = P0^7;   //С�����
code uchar seven_seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
code uchar seven_bit[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf};
void timer0_init(void)	  //��ʼ��
{
	TMOD = 0x01;
	TH0 = 0xf8;           //(65536 - 2000)/ 256;
	TL0 = 0x30;           //(65536 - 2000)% 256;
	TR0 = 1;
	EA  = 1;
	ET0 = 1;
}
void timer0_isr(void) interrupt 1		  //�ж�
{
	TR0 = 0;
	EA = 0;
	TH0 = 0xf8;
	TL0 = 0x30;
	TR0 = 1;
	EA = 1;
	P0 = 0xff;		//������ܲ���ʾ���ݣ���ֹ�źŴ���
	i++;
	if(i >= 500)   	//�պ�1��
   	{
		i = 0;
		sec++;
		flash = !flash;	//2��ı�һ��״̬���������flash1��ı�һ��״̬��������
	}
	if(sec >= 60)
 	{
 		sec = 0;
 		min++;
 	}
 	if(min >= 60)
	{
		min = 0;
		hour++;
	}
	if(hour >= 24)
		hour = 0; 
	switch(j)
	{
		case 0: P0 = seven_seg[sec % 10]; break;	//��ʾ���λ
		case 1: P0 = seven_seg[sec / 10]; break;
		case 2: P0 = seven_seg[min % 10];dop = flash;break; 
		case 3: P0 = seven_seg[min / 10]; break;
		case 4: P0 = seven_seg[hour % 10];dop = flash; ;break;
		case 5: P0 = seven_seg[hour / 10]; break;	
	}
	P2 = seven_bit[j];	//ÿ�ж�һ��ֻ��ʾһλ����ܣ���ʾͣ��ʱ��Ϊ�жϼ��
	j ++;
	if(j >= 6)	
	j=0;  //ѭ����������
}
void main(void)
{
	sec = 30;
	min = 30;
	hour = 8;
	P2 = 0xff;
	timer0_init();
	while(1);
}