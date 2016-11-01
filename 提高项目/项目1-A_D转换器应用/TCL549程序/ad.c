#include <REGX51.H>
#include "intrins.h"
#define  uchar unsigned char
#define  uint	  unsigned int
uchar code  tb[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code seg_bit_scan[] = {0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; //��������ܶ�Ӧ��λѡ����
sbit data_temp = P1^5;	 //������
sbit cs = P1^4;		 	//Ƭѡ
sbit sclk = P1^1;		 //io��ʱ��
uchar qian,bai,shi,ge;
uint j;
void chuli(uint num)	  //��ʾ����
{
  
  qian=num/1000;	      //ǧ���٣�ʮ��������
  bai=num/100%10;
  shi=num/10%10;
  ge=num%10;
} 
uint ad_549()		    //TLC549����
{
  uchar i;
  uint data_ad = 0;
  cs = 1;				//��ʼ��������
  sclk = 0;
  cs = 0;
  _nop_();		    
  for(i = 0;i < 8;i++)	//��ȡ�ɼ����ݣ���ȡ������һ�βɼ�����
    {
	  sclk = 1; 
	 
	  if(data_temp)data_ad |= 0x01;
	  sclk = 0;  
	  data_ad = data_ad << 1;  
	} 	
  cs = 1;
  data_ad = data_ad  * (500/ 256);   
  return(data_ad); 
}
void timer0_isr(void)  interrupt 1       			 // timer0�жϷ�����
{
                         							//����ܵ�λѡ����
	TR0 = 0;                       		 			//ֹͣ����
	TL0 = (65536-10000)%256;
	TH0 = (65536-10000)/256; 
	j++;                                			//λѭ��������1
	if(j >= 4)
	j = 0;                          				//ѭ����ʾ1�Σ�j����
   	TR0 = 1;
}
void timer0_init (void)            		// timer0�жϳ�ʼ������
{
	EA = 0;                      
	TMOD = 0x01;             
	TR0 = 0;                      
	TL0 = (65536-10000)%256;
	TH0 = (65536-10000)/256;                  
	PT0 = 1;                      
	ET0 = 1;                     
	EA = 1;                      
	TR0 = 1;                      
} 						
void main(void)				 //������
{  

	uint i;
	timer0_init ();
	while(1)
	{  
		
		i = ad_549();
		
	   	chuli(i);	  //��ʾ���ս��
		P0=0xff;
		switch(j)
		{
			case 0: P0 = tb[ge]; break;   	
			case 1: P0 = tb[shi]; break;   	
			case 2: P0 = tb[bai]&0x7f; break;
			case 3: P0 = tb[qian]; break;  	
		} 
		P2 = seg_bit_scan[j];                    //��j��Ӧ��P2�������ܵ�λѡ�ź�
		} 

	}

  