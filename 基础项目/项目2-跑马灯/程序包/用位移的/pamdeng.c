#include<reg51.h>
#define uint unsigned int
void delay(uint x)
{
	while(x--);
}
void main(void)
{
	uint aa;
	aa = 0x01;			   //8λֻ��һλΪ�ߣ�����Ϊ��
	while(1)
	{
		delay(10000);	   //������ʱ����
		P0 = ~aa;        //P0Ϊaaȡ��ֵ��8λֻ��һλΪ�ߣ�����Ϊ��   ��ԭ��ͼ���� IO������͵�ƽled��
		aa = aa << 1;	   //��һ��ѭ����led1��������������
		if(aa == 0x00)     //ȫ���Ƴ�
		aa = 0x01;
	}
}