#include<reg51.h>
code unsigned char seven_seg[18] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40,0xff};
void delay (void)                          //ʱ���ӳٺ��� 
{              
	unsigned char i,j;
	for(i = 0;i < 255;i++)
	for (j = 0;j < 255;j++);
}
void main (void)
{
	unsigned char i;         
	P0  =  0x81;				//P0^0 λѡ��һ������� P0^7��������
	while (1)
	{ 
		for  (i = 0; i < 17; i++)
		{
			P2 = seven_seg[i];        //���0~9�����������
			delay();delay();          //����ʱ���ӳٺ���delay
		}
	}
}
/***************Ҳ���Բ���������Ʒ���ʵ��*********************/
/*void main (void)
{
	unsigned char i;            
	P0  =  0x81;				//P0^0 λѡ��һ������� P0^7��������
	while (1)
	{ 
		P2 = seven_seg[i];			  ///���0~9�����������
		delay();delay();              //����ʱ���ӳٺ���delay				
		i++;	
		if(i >= 18)
		i = 0;
	}
}*/
