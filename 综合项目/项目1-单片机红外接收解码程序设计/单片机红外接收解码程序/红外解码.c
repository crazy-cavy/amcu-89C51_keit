#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar code bit_ser[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf}; 
uchar code seven_seg[] = {0xf9,0x8e,0xc0};	//1,f,0���α�ʾ�ߵ�ƽ�����գ��͵�ƽ
uchar code address[]={1,1,1,1,1,1};	//���Ԥ��ĵ�ַ��ߵ�ƽΪ2���͵�ƽΪ0������Ϊ1
uchar disbuffer[6];	   //������ʾ������
uchar DATEA[25];	   //���յ���Ϣ
uchar DATE[12];		   //����õ�������
bit dateflag; 		   //��������ʱ�ı��
char num,i,j;		   //���ڽ��պ���ʾʱ����
sbit DATE_IN = P1^0;   //���ݵĽ��ն�
//��ʱ����ʼ������
void Timer_init()
{
	TMOD = 0x11;
	TH0 = 0xFA;
	TL0 = 0x00;
	EA = 1;
	ET0 = 1; 
	TL1 = (65536-5000) % 256;
	TH1 = (65536-5000) / 256;
	ET1 = 1;
	TR1 = 1;
}
//Timer1�жϴ�������������ʾ���յ�������
void time1_interrupt() interrupt 3
{
	TL1 = (65536-5000) % 256;
	TH1 = (65536-5000) / 256;
	P0 = 0xff;
	P2 = bit_ser[i];
	P0 = seven_seg[disbuffer[i]];
	i ++;
	if(i >= 6)
		i = 0;
}
//Timer0�жϴ�����
void timer0_interrupt() interrupt 1
{
	if(dateflag)		//���dateflagΪ�ߣ�˵������Ҫ��������ˣ�����˵����ͬ��������루���ڹ�����
	{
		dateflag = 0;
		DATEA[num] = DATE_IN;
		num ++;
		TR0 = 0;
		TH0 = 0xFA;
		TL0 = 0x00;
		TR0 = 1;
	}
	else			 
	{
		num = 0;
	}
}
//���ݽ��պ���
void jieshou_date()
{
	if(!dateflag)
	{
		TR0 = 0;
		TH0 = 0xFA;
		TL0 = 0x90;																									  
		TR0 = 1;
		dateflag = 1; 					
	}	
}
//�ӽ��յ�����Ϣ����ȡ����
void huoqu_date()
{
	for(j = 0;j < 12;j ++)	//�����յ���24λ��ƽ�ź�ת��������
	{
		DATE[j] = DATEA[j + j] + DATEA[j + j + 1];
		if(DATE[j] && ! DATEA[j + j])	 //������յ�����λ������0��1˵��������
		{
			break;
		}
	}
	num = 0;
	if(j == 12)
	{
		if(DATE[0] == address[0] && DATE[1] == address[1]&& DATE[2] == address[2] &&
			DATE[3] == address[3] && DATE[4] == address[4] && DATE[5] == address[5])
		{		 //�������õ��ĵ�ַ���Ԥ��ĵ�ַ����ͬ���ͽ�����õ������ݷ�����ʾ������
			for(j = 0;j < 6; j++)
			{
				disbuffer[j] = DATE[6 + j];
			}	
		}
	}	
}
//������
void main()
{
	Timer_init();
	while(1)
	{
		if(!DATE_IN)		 //˵�����ź�����
		{
			jieshou_date();	 //������Ϣ
		} 
		if(num >= 25)		 //˵���Ѿ����յ���һ����������
		{
			huoqu_date();	 //�ӽ��յ���Ϣ����ȡ����			
		}					
	}
}