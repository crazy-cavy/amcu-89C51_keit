#include<reg51.h>                    //����ͷ�ļ�
#include<intrins.h>                  
void delay(unsigned char x)          //��ʱ����
{
	unsigned char i,j;
	for(i = 0;i < x;i++)
	for(j = 0;j < 200;j++);
}
void main(void)                     //������
{
	unsigned int a = 0xfe ;
	P0 = 0x00;					  	//��P0����͵�ƽ��8��ledȫ��
	while(1)                        //��������ѭ��
	{
		delay(250);                 //ʵ�β��ܴ���255����Ϊ�β�xΪchar 
		a=_crol_(a,1);
	}
}
