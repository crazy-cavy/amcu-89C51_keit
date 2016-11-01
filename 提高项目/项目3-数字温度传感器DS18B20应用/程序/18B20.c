/*********************************************************************/
//��ȡDS18B20�¶ȣ�ͨ���������ʾ�����¶ȳ���35��ʱ�̵�������																						
/*********************************************************************/
#include<reg51.h>		
#define uchar unsigned char 
#define uint unsigned int
uchar a,b,c;
sbit DQ = P1^0;
void delay(uint x)
{
	while(x--); 
 
}
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
/******************************��18B20�ж�һ���ֽ�****************************/
uchar Read_OneChar(void)
{
	uchar i = 0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	{
  		DQ = 0;          // �������ź�
  		dat >>= 1;
  		DQ = 1;          // �������ź�
  		if(DQ)
  		dat |= 0x80;
  		delay(8);
	}
	return(dat);
}
/******************************��18B20��дһ���ֽ�****************************/
void Write_OneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		delay(10);
		DQ = 1;
		dat >>= 1;
	}
	delay(8);
}
/***********************************��ȡ�¶�**********************************/
uchar Read_Temperature(void)
{
	uchar i = 0,t = 0;
	Init_DS18B20();
	Write_OneChar(0xcc); 	// ����������кŵĲ���
	Write_OneChar(0x44); 	// �����¶�ת��
	Init_DS18B20();
	Write_OneChar(0xcc); 	//����������кŵĲ���
	Write_OneChar(0xbe); 	//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	a = Read_OneChar();   	//��ȡ�¶�ֵ��λ
	b = Read_OneChar();   	//��ȡ�¶�ֵ��λ
	c = 0x0f & a;			//�õ�С������
	a = a >> 4;				//��λ����4λ
	b = b << 4;             //��λ����4λ
    t = a | b;				//�õ�8λ�¶ȵ��������֣����Ϊ����λ
	return(t);
}


void write_temprom(uchar temp_up_set,uchar temp_down_set )//�����¶�������temp_up_set,temp_down_set;
{
 	Init_DS18B20();
	Write_OneChar(0xcc);      		//����������кŵĲ���
	Write_OneChar(0x4e); 	  		//д׼�� 
	Write_OneChar(temp_up_set);	    //��18B20���ݴ�д����		 
	Write_OneChar(temp_down_set);   //��18B20���ݴ�д����
	Write_OneChar(0x7f);
	Init_DS18B20();
	Write_OneChar(0xcc); 
	Write_OneChar(0x48); 	  		//��18B20��rom��д����
	Init_DS18B20();
	Write_OneChar(0xcc);
	Write_OneChar(0xb8);
	;;
}
uchar  temp_up,temp_down;//�����洢��DS18B20�ڵ��¶�������ֵ
void read_temprom(void)//��ȡ�¶��¶�������
{
   	uchar i;
	Init_DS18B20();
	Write_OneChar(0xcc);	            // ����������кŵĲ���        	
	Write_OneChar(0xbe); 				// ��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	i = Read_OneChar();	  				//��0��ַ�Ĵ������ӵ�
	i = Read_OneChar();  				//��1��ַ�Ĵ������ӵ�
	temp_up = Read_OneChar();			//��2��ַ�Ĵ���
	temp_down = Read_OneChar();			//��3��ַ�Ĵ���	
}	
