#include <REG51.H>
#include "intrins.h"
#include "AT24C04.h"

#define  uchar unsigned char
#define  uint unsigned int

uchar code  LED[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code Bit_sel[4] = {0x08,0x04,0x02,0x01 }; //��������ܶ�Ӧ��λѡ����

sbit DIO = P1^0;	 	//������
sbit CS = P1^1;		 	//Ƭѡ
sbit CLK = P1^2;		//io��ʱ��

sbit SCS = P1^4;
sbit SDATA = P1^5;
sbit SCLK = P1^3;

sbit ADD = P3^0;
sbit SUB = P3^1;
sbit Pre_read = P3^2;
sbit Store = P3^3;

uint qian,bai,shi,ge;  //������ʾ����ܵ� ǧ���٣�ʮ���� ����λ����ʾ
uint val,num;		   //val ����������DA  �����ݣ�num�������ж��ǲ��ǳ�����
uint cp;			   //�����ı���
uchar key_stat;
uchar add_stat,sub_stat;
uchar st_flag,pre_flag;

void delay(uint x)	 	//΢�����ʱ����
{
	while(x--)
		_nop_();
}

/**********************************************
	�������ƣ�deal
	�������ã�����AD�ķ���ֵ
	������������
	��������ֵ����
**********************************************/											   
void deal(uint num)	  //��ʾ����
{	
	qian=num/1000;	      //ǧ���٣�ʮ��������
	bai=num/100%10;
	shi=num/10%10;
	ge=num%10;
} 

/**********************************************
	�������ƣ�TL549_AD()
	�������ã�����AD�ķ���ֵ
	������������
	��������ֵ��data_ad
**********************************************/	
uint TL549_AD()		    //TLC549����
{
	uchar i;
	uint data_ad = 0;
	CS = 1;				//��ʼ��������
	CLK = 0;
	CS = 0;
	_nop_();		    
	for(i = 0;i < 8;i++)	//��ȡ�ɼ����ݣ���ȡ������һ�βɼ�����
	{
		CLK = 1;	 
		if(DIO)data_ad |= 0x01;
		CLK = 0;  
		data_ad = data_ad << 1;  
	} 	
	CS = 1;
	data_ad = data_ad  * (500/ 256);   
	return(data_ad); 
}

/**********************************************
	�������ƣ�TLC5615_DA(uint da)
	�������ã�TLC5615_DA ��daת����ģ�����
	����������da
	��������ֵ����
**********************************************/	
void TLC5615_DA(uint da) //	TLC5615 ��DA ת������
{
	uchar i;
	da<<=6;

	SCS=0;
	SCLK=0;

	for (i=0;i<12;i++)
	{
		SDATA=(bit)(da&0x8000);
		SCLK=1;
		da<<=1;
		SCLK=0;
	}

	SCS=1;
	SCLK=0;
	for (i=0;i<12;i++);
}
/**********************************************
	�������ƣ�key_scan()
	�������ã�������Щ��������
	������������
	��������ֵ����
**********************************************/	
void key_scan()
{
	if (ADD == 0)						   // ADD �����ļ��̴�����
	{
		delay(10);
		if (ADD == 0)
		{
			add_stat = 1;
			num ++;
		}
		else
		{
			add_stat = 0;
			num = 0;
		}
		//�˴��ж��ǲ��ǳ����������Ļ� ʹ�䲽��ֵ �Ӵ�
		if (ADD == 0 && add_stat == 1 && num >= 300)  
		{
			val += 5;
			num = 0;
		}
		if (ADD == 1 && add_stat == 1)
		{
			val ++;
			num = 0;
			add_stat = 0;
		}
		if (val >= 1024)
		{
			val = 1023;
		}
	}

	if (SUB == 0)							//SUB �����ļ��̴�����
	{
		delay(10);
		if (SUB == 0)
		{
			sub_stat = 1;
			num ++;
		}
		if (SUB == 0 && sub_stat == 1 && num >= 300)
		{
			val -= 5;
			num = 0;
		}
		if (SUB == 1 && sub_stat == 1)
		{
			val --;
			num = 0;
			sub_stat = 0;
		}
		if (val <= 0)
			val = 0;
	}
									
	if (Pre_read == 0)			   			//Ԥ�����ݵ� ���̴�����
	{
		delay(100);
		if (Pre_read == 0)
		{
			pre_flag = 1;
		}
		if (Pre_read == 1 &&pre_flag == 1 )
		{
			pre_flag = 0;
			val = read_24C04(20);			//��AT24C04�еĵ�ַ20 �ж���Ԥ�洢������
		}
	}

	if (Store == 0)						//������ֵ �����ļ��̴�����
	{
		delay(100);
		if (Store == 0)
		{
			st_flag = 1;
		}
		if (Store == 1 && st_flag == 1)
		{
			st_flag	= 0;
			write_24C04(20,val);		 //��AT24C04�еĵ�ַ20 д��洢������
		}
	}
}
/**********************************************
	�������ƣ�timer0_init (void)
	�������ã���ʼ����ʱ��0��������
	������������
	��������ֵ����
**********************************************/	
void timer0_init (void)            		// timer0�жϳ�ʼ������
{
	EA = 0;                      
	TMOD = 0x01;             
	TR0 = 0;                      
	TL0 = (65536-5000)%256;		 //���ü�������ֵ
	TH0 = (65536-5000)/256;                  
	PT0 = 1;                      
	ET0 = 1;                     
	EA = 1;                      
	TR0 = 1;                      
}
/**********************************************
	�������ƣ�main(void)
	�������ã�main���������
	������������
	��������ֵ����
**********************************************/							
void main(void)				 //������
{  
	timer0_init();			//��ʼ����ʱ��0
	init_24C04();			//��ʼ��AT24C04
	while(1)
	{ 
		key_scan();			//���ü���ɨ�躯��
		TLC5615_DA(val);	//������̷��͹�����ֵ
	} 
}

/**********************************************
	�������ƣ�timer0_isr(void)  interrupt 1
	�������ã���ʱ��0����ʽ1�����жϷ����ӳ���
	������������
	��������ֵ����
**********************************************/	
void timer0_isr(void)  interrupt 1       	// timer0�жϷ�����
{
                         					//����ܵ�λѡ����
	TR0 = 0;                       		 	//ֹͣ����
	TL0 = (65536-5000)%256;					//���������������ֵ
	TH0 = (65536-5000)/256; 
	cp++;                                	//λѭ��������1
	if(cp >= 4)
		cp = 0; 
	deal(TL549_AD());                       //ѭ����ʾ1�Σ�j����
   	TR0 = 1;

	P0=0xff;  //��j��Ӧ��P2�������ܵ�λѡ�ź�
	switch(cp)
	{
		case 0: P0 = LED[ge]; break;   	
		case 1: P0 = LED[shi]; break;   	
		case 2: P0 = LED[bai]&0x7f; break;
		case 3: P0 = LED[qian]; break;  	
	} 
	P2 = Bit_sel[cp]; 
}  