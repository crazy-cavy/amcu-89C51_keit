#include<reg51.h> 
#define uchar unsigned char
uchar code  seven_seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit key1 = P3^4;
sbit key2 = P3^5;
uchar key1_state,key2_state;       //�������µ�״̬����
void delay(uchar i)
{
	while(i) i--; 
}
void key_scan(void)              //����ɨ�躯��
{	
	if(key1 == 0)                //�����������
	{	
		delay(200);              //��ʱһ��ʱ�䣬��������
		while(key1 == 0);
	   key1_state = 1;           //�ٶ԰�����⣬���ȷʵ���£�����״̬����Ϊ1
	}
	if(key2 == 0)
	{	
		delay(200);
		while(key2 == 0)
 		key2_state = 1;
	}
}
void main(void)
{	
	char i;
	while(1)
	{
		key_scan();                        //���ü���ɨ�躯��
		if(key1_state == 1)                //�жϼ���״̬���������Ϊ1��˵���а�������
		{
			i++;                            //��ʾ��������
			key1_state = 0;                 //�������Ӻ󣬼���״̬��������
			if(i>9) i = 0;                  //�������ӳ���9�󣬻ص�0
		}
		if(key2_state == 1)
		{	
			i--;
			key2_state = 0; 
			if(i < 0) i = 9;
		}
		P0 = seven_seg[i];                  //��ʾ���̵�������
		P2 = 0xfe;									//�����SEG0��ʾ
	}
}
