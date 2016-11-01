/*********************************************************************/
//��ȡDS18B20�¶ȣ�ͨ���������ʾ�����¶ȳ���35��ʱ�̵�������																						
/*********************************************************************/
#include<reg51.h>
#include"18B20.c"		
#define uchar unsigned char 
#define uint unsigned int

code  uchar seven_seg[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code uchar seven_bit[] = {0x20,0x10,0x08,0x04,0x02,0x01};
uchar cp;
uint temp1,temp2;
sbit led = P0^6;    
void timer0_init(void)	 //��ʼ��
{
	TMOD = 0x01;
	TH0 = 0xec;
	TL0 = 0x78;
	TR0 = 1;
	EA  = 1;
	ET0 = 1;
}
/*****************************************************************************/
void timer0_isr(void) interrupt 1		  //�ж�
{
	uchar j;
	TR0 = 0;
	EA = 0;
	TH0 = 0xec;
	TL0 = 0x78;
	TR0 = 1;
	EA = 1;
	cp++;
	if(cp >= 200)   //�պ�1��
   {
		cp = 0;
		temp1 = Read_Temperature();//�õ��¶�
		if((temp1 & 0x80) == 0x80) //����¶�Ϊ��ֵ
		{	
		
			temp2 =0x7f & ~temp1  +  (0x0f & ~c) /15;  //��������,��Ҫ����С�����Ľ�λ
			c = 0x0f & ~c + 0x01;   //С������
		}
		else temp2 = temp1;//����¶�Ϊ��ֵ�����ô���
	}
	P0 = seven_bit[j];
	switch(j)
	{
		case 0:P0 |= 0x80;P2 = seven_seg[c * 10/16];P0 &= 0x7f;break;//��ʾС��
		case 1:P0 |= 0x80;P2 = 0x80 | seven_seg[temp2 % 10];P0 &= 0x7f;break;//��ʾ��λ������С����
		case 2:{	
							if((temp2 < 10) && (temp1 < 0x80))//����¶�С��10�ȣ���Ϊ��ֵ��ʮλ0����ʾ
							{
								P0 |= 0x80;       //��������
								P2  = 0x00;
								P0 &= 0x7f;				//�ر�������
								break;
							}
							if((temp1 > 0x80) && (temp2 < 10)){P0 |= 0x80;P2 = 0x40;P0 &= 0x7f;break;}//����¶�С��10�ȣ���Ϊ��ֵ��ʮλ0����ʾ��ֻ��ʾ��-��
								P0 |= 0x80; P2 = seven_seg[temp2 / 10];	P0 &= 0x7f;break;//����¶ȴ���10�ȣ�10λ������ʾ				
						}
		case 3:{
					if((temp1 > 0x80) && ((temp2 / 10) > 0))//����¶�Ϊ��ֵ����ʮλ��Ϊ0����λ��ʾ��-��
					P0 |= 0x80;P2 = 0x40;P0 &= 0x7f;
					break;
				}
	}
	j++;
	if(j >= 3)
	j=0;
}
void main(void)
{
	timer0_init();
	
	while(1)
	{
		if(temp2 >= 35) 
			led = 0;
		else
			led = 1;
	}
}