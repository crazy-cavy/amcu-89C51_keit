#include<reg51.h>                    //����ͷ�ļ�               
sbit led1 = P0^0;     //����P0�ڵĵ�һλ Ϊled
void delay(unsigned char x)          //��ʱ����
{
	unsigned char i,j;
	for(i = 0;i < x;i++)
	for(j = 0;j < 200;j++);
}
void main(void)                     //������
{
	P0 = 0xFF;					  	//��P0����ߵ�ƽ��8��ledȫ��
	while(1)                        //��������ѭ��
	{
		led1=~led1;           //LED1ȡ����
		delay(250);           //ʵ�β��ܴ���255����Ϊ�β�xΪchar 
	}
}
