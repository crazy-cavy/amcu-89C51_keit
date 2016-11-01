/********�ó�����Ҫ������DS18B20�ɼ��¶ȣ�Ȼ��ͨ���������ʾ�¶�*************/
/*�������յ���λ�����͵�����֮�󣬸ó���Ὣ��ʱ���¶�ֵͨ�����ڷ��͸���λ��*/
#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit DQ = P1^0;		//����DS18B20���ź��߶˿�
uchar i,j;
uchar dis_buffer[4];//�������ݻ�������
uchar bit_ser[]={0xfe,0xfd,0xfb,0xf7}; //���������Ƭѡ����
uchar seven_seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
									   //��������ܶ�ѡ����
/****************************��ʱ����*********************************/
void delay(uint x)
{
	while(x)   
	x--;
}
/*************************DS18B20��ʼ������***************************/
void Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1;         //DQ��λ
	delay(8);       //������ʱ
	DQ = 0;    	    //��Ƭ����DQ����
	delay(80);   	//��ȷ��ʱ ���� 480us
	DQ = 1;      	//��������
	delay(14);
	x=DQ;      	    //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	delay(20);
}
/***************************��18B20�ж�һ���ֽ�************************/
uchar ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
 	{
  		DQ = 0; 	// �������ź�
  		dat>>=1;
  		DQ = 1; 	// �������ź�
  		if(DQ)
   		dat|=0x80;  
  		delay(8);
 	}
 	return(dat);
}
/***************************��18B20��дһ���ֽ�************************/
Write_OneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;	  //�������ź�
		DQ = dat & 0x01;
		delay(5); 
		DQ = 1;	  //�������ź�
		dat >>= 1;
	}
	delay(4);
}
/**************************��18B20�ж�ȡһ���ֽ�***********************/
int Read_Temperature(void)
{
	uchar i = 0,t = 0,a,b;
	int temp;
	Init_DS18B20();
	Write_OneChar(0xcc); 	// ����������кŵĲ���
	Write_OneChar(0x44); 	// �����¶�ת��
	Init_DS18B20();
	Write_OneChar(0xcc); 	//����������кŵĲ���
	Write_OneChar(0xbe); 	//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	i = ReadOneChar();   	//��ȡ�¶�ֵ��λ
	t = ReadOneChar();   	//��ȡ�¶�ֵ��λ
	a = i & 0x0f;
	b = t;
	i = i >> 4;             //��λ����4λ������С������
	t = t << 4;             //��λ����4λ����������λ
	t = t | i;
	temp = (t + a * 0.0625) * 100; //�õ�һ����ʵ���¶�����100����ֵ����Ҫ��Ϊ�˸��õ���ʾ�ʹ���          
	return(temp);			//�����¶�ֵ
}

/***************************��ʼ����ʱ��0******************************/
void timer0_init(void)			 //
{
	TMOD = 0x21; //���ڴ���ͨ����Ҫʹ�ö�ʱ��1�����TMOD��ֵ��0x21
	TL0 = (65536-5000) % 256;
	TH0 = (65536-5000) / 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
/*************************�������ݵĺ���********************************/
void txd_data(char send_data)	
{
	SBUF = send_data; //����Ҫ���͵����ݷ��뷢�ͻ�����
	while(!TI);		  //�ȴ���������
	TI = 0;
}
/********************T0�жϴ���������Ҫ������ʾ��ǰ�¶�***************/
void timer0_isr(void) interrupt 1
{
	int temp; 
	TR0 = 0;
	TL0 = (65536-5000) % 256;
	TH0 = (65536-5000) / 256;
	TR0 = 1;
	switch(i)
	{
		case 0:
			P2 = bit_ser[0];
			P0 = seven_seg[dis_buffer[0]];				
			break;
		case 1:
			P2 = bit_ser[1];
			P0 =seven_seg[dis_buffer[1]] & 0x7f;
			break;
		case 2:
			P2 = bit_ser[2];
			P0 =seven_seg[dis_buffer[2]];
			break;
		case 3:
			P2 = bit_ser[3];
			P0 =seven_seg[dis_buffer[3]];
			break;
	} 
	i++;
	if(i >= 4) 
	{
		i = 0;
		j++;
		if(j >= 10)	//�����200ms�ͻ��ȡһ���¶ȣ������¶�ֵ������ʾ������
		{
			j = 0;
			temp = Read_Temperature();
			dis_buffer[0] = temp / 1000;
		   	dis_buffer[1] = temp % 1000 / 100;
			dis_buffer[2] = temp % 100 / 10;
			dis_buffer[3] = temp % 10;
		}
	}
}

/***************************����ͨ�ų�ʼ������***************************/
void uart_init(void)
{ 
	SCON = 0x50;         //��ʽ1,�������
	TMOD = 0x21;         //T1��ʽ2��ʱ��T0��ʽ��1          
	TH1 = 0xFd;          //������9600,Fosc=11.0592MHz
	TL1 = 0xFd;
	TR1 = 1;
	ES = 1;           	 // �򿪴����ж�
}
/**************************�����жϴ�����*****************************/
void uart() interrupt 4			  
{
	char y,i;
	while(!RI);
	y = SBUF;			 //��ȡ���յ���Ϣ��Ȼ���ж��Ƿ�Ϊ�����¶�����
	if(y == '0')		 //���Ϊ��������ͽ���ǰ���ݻ��������ڵ����ݷ��͸���λ��
	{
		for(i = 0;i <= 3;i++)
		{
			txd_data(dis_buffer[i] + 48);
		}
	}
	RI = 0;		
}
/*******************************������*********************************/
void main()
{
	timer0_init();		//����T0��ʼ������
	uart_init();		//���ô��ڳ�ʼ������
	while(1)
	{}
}