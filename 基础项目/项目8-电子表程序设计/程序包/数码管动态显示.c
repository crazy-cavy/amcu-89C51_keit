#include <REG51.H>	
unsigned char code LEDDATA[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,
0x82,0xf8,0x80,0x90,0xff,0x8e,0x86};
//�������ʾ�Ĵ����������Ϊ��ơ�"F"��"E"
unsigned char code LEDBITDATA[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,}; 
//�����ɨ������
unsigned char LEDBuffer[6];			//������ʾ����������
unsigned char Second;					//�뵥Ԫ
unsigned char Minute; 				//�ֵ�Ԫ
unsigned char Hour;					//ʱ��Ԫ
unsigned char Beepflag;	 			//��ʱ�����־
unsigned char Minuterom;	 			//��ʱ�ֵ�Ԫ
unsigned char Hourrom; 				//��ʱʱ��Ԫ
unsigned char SETFlag=0;				//ģʽ��־
unsigned char second_tick;	 			//������־
unsigned char Time;					//��ʱ����
unsigned char ALMFlag;				//��ʱ������־
sbit SET_KEY=P3^3;			 	 	//ģʽ��
sbit DOWN_KEY=P3^4;				//�Ӽ�����
sbit UP_KEY=P3^5;					//��������
sbit ALM_KEY=P3^2;					//��ʾ��ʱʱ�䰴��
sbit Beep=P1^7;						//�������ӿ�����


void init()
{ 	TMOD=0x01;						//T0��ʼ����ʽ1,��ʱ
	TH0=(65536-2000)/256;				//TH0,TL0װ�붨ʱ2mS�ĳ�ֵ
	TL0=(65536-2000)%256;
	TR0=1;							//����T0����
	ET0=1;							//����T0����ж�
	EA=1;							//CPU���ж�
}

void Delay(unsigned int t)					//��ʱ�ӳ���
{ while(t)t--;
}
void key()								//���̲����ӳ���
{	unsigned char i;						//��������λ����־
	char Num;							//��ʱ���֣��洢����ϲ�ֵ
	if(SET_KEY==0)					//�ж�ģʽ���Ƿ���
	{	Delay(500);					//ȥ��������
		if(SET_KEY==0)				//���ж��Ƿ���ð�����
		{ 	SETFlag++;				//״̬�ı�
			if(SETFlag==7) SETFlag=0;		//��������ģʽ
			if(SETFlag==1) i=4;			//���ڶ�ȡ��ʾ�����λ��
			if(SETFlag==2) i=2;
			if(SETFlag==3) i=0;
			if(SETFlag==4) i=4;
			if(SETFlag==5) i=2;
			if(SETFlag==6) i=0;
		}
	while(SET_KEY==0);					//�Ȱ����ͷ�
	}

	if((UP_KEY==0)&&(SETFlag!=0))		//�жϼӼ������Ƿ���
	{
		Delay(5000);					//ȥ��������
		if(UP_KEY==0)					//���ж��Ƿ���ð�����
		{	Num=(LEDBuffer[i+1]*10+LEDBuffer[i]);
			Num++;					//ʱ��Ԫ����ֵ��1
			if(Num==60) Num=0;			//�ӵ�60��0
			if((Num==24)&&((SETFlag==1)||(SETFlag==4))) Num=0;	//�ӵ�24��0
			switch(SETFlag)				//���޸�ֵд��
			{	case 0: ;break;
				case 1: Hour=Num;break;
				case 2: Minute=Num;break;
				case 3: Second=Num;break;
				case 4: Hourrom=Num;break;
				case 5: Minuterom=Num;break;
				case 6: ALMFlag=!ALMFlag;break;		
			}
		}
	 }

	if((DOWN_KEY==0)&&(SETFlag!=0))	//�жϼ��������Ƿ���
	{
		Delay(5000);					//ȥ��������
		if(DOWN_KEY==0)				//���ж��Ƿ���ð�����
		{	Num=(LEDBuffer[i+1]*10+LEDBuffer[i]);
			Num--;					//ʱ��Ԫ����ֵ��1
			if((Num<0)&&((SETFlag==1)||(SETFlag==4))) Num=23;	//��24��0
			if((Num<0) )Num=59;			//��60��0
			switch(SETFlag)				//���޸�ֵд��
			{	case 0: ;break;
				case 1: Hour=Num;break;
				case 2: Minute=Num;break;
				case 3: Second=Num;break;
				case 4: Hourrom=Num;break;
				case 5: Minuterom=Num;break;
				case 6: ALMFlag=!ALMFlag;break;		
			}
		}
	}
	if(ALM_KEY==0)
	{		SETFlag=0;				//״̬����
			if(ALMFlag==0)LEDBuffer[0]=11;	//��������״̬��ʾF����E
			else LEDBuffer[0]=12;		//��ʱ,��,�뵥Ԫ���������ݴ���
			LEDBuffer[1]=10;			//�رո��������ʾ
			LEDBuffer[2]=Minuterom%10;
			LEDBuffer[3]=Minuterom/10;
			LEDBuffer[4]=Hourrom%10;
			LEDBuffer[5]=Hourrom/10;
			if(ALMFlag==1)
			{	Beep=1;
				Beepflag=0;
			}
	}
}			 


void display(void)							//��ʾ�ݴ������ݶ�Ӧ�Ĵ�����ʾ
{ 	unsigned char LEDScanCount,i;				//λѡɨ�������
P0=0xff;								//��Ӧ������Ҫ
if((UP_KEY==0)||(DOWN_KEY==0)) i=0x00;	//�Ӽ����в���������˸
 	else i = 0xff*second_tick;					//�趨��˸����
P2= LEDBITDATA[LEDScanCount];			//�ͳ�λѡ����
switch(SETFlag)
	{	case 0:	P0=LEDDATA[LEDBuffer[LEDScanCount]];break; 	//�ͳ���ѡ���� 
	 	case 1:	if(LEDScanCount>=4)							//�жϳ������λ
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];	//ʹСʱ��˸
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;	//��λ������ʾ
	 	case 2:	if((LEDScanCount==2)||(LEDScanCount==3))	 	//�жϳ��м�λ
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];	//ʹ������˸
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;	//����λ������ʾ
		case 3:	if(LEDScanCount<=1)
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;
		case 4:	if(LEDScanCount>=4)
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;
	 	case 5:	if((LEDScanCount==2)||(LEDScanCount==3))
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;
		case 6:	if(LEDScanCount<=1)
					 P0=i|LEDDATA[LEDBuffer[LEDScanCount]];
				else P0=LEDDATA[LEDBuffer[LEDScanCount]];break;
	}
LEDScanCount++; 					//ɨ��ָ��Ӽ���
if(LEDScanCount==6)LEDScanCount=0 ;	//ɨ�����ͷ��ʼ;
}


void timer0_isr(void) interrupt 1
{ 	unsigned int SecondCount;
	unsigned int timercp;					//�������
	TH0=(65536-2000)/256;				//TH0,TL0װ�붨ʱ2mS�ĳ�ֵ
	TL0=(65536-2000)%256;	 
	display();							//������ʾ����
	timercp++;		
	if(SETFlag!=0) 						//10�벻�����Զ�����
		{	if((SET_KEY==0)||(UP_KEY==0)||(DOWN_KEY==0)) Time=0;	//������в���������ʱ
		 	if (Time>=10){SETFlag=0;Time=0;}
		}
	if (timercp == 250)				
		{	timercp= 0; 
			second_tick = !second_tick;			//�趨0.5������һ�� 
			SecondCount++;
			if(SecondCount==2) 				//1���ʱ��
			{	SecondCount=0;				//����������
				Time++;					
	 			Second++;					//ʱ������1
				if(Second==60)
				{	Second=0;		 		//������
					Minute++;		 		//�ֽ�һ
					if(Minute==60)
					{	Minute=0;
						Hour++;
						if(Hour==24)Hour=0;	//Сʱ����
					}
				}
			}
	if(SETFlag<=3)				 	//״̬ģʽС��3�Ǽ���ʱ��ʱ��
			{ 	LEDBuffer[0]=Second%10; 
				LEDBuffer[1]=Second/10;
				LEDBuffer[2]=Minute%10;
				LEDBuffer[3]=Minute/10;
				LEDBuffer[4]=Hour%10;
				LEDBuffer[5]=Hour/10;
			}
			else							 //��������ʱ��ʱ��������ʱ��
			{	if(ALMFlag==0)LEDBuffer[0]=11; //��ʾ���Ӽ���״̬
		 		else LEDBuffer[0]=12;
				LEDBuffer[1]=10;			 //�رյ����ڶ�λ
				LEDBuffer[2]=Minuterom%10;
				LEDBuffer[3]=Minuterom/10;
				LEDBuffer[4]=Hourrom%10;
				LEDBuffer[5]=Hourrom/10;
			}
}
}



void main(void)
{	init();									//��ʼ��
while(1)
{	key();							//���ü���
		if(ALMFlag==1)
		{	if(Minute!=Minuterom) Beepflag=1;	//��ʱ�����ڲ�ͬ���رշ�����
			if((Hour==Hourrom)&&(Minute==Minuterom)&&(Beepflag==1)) Beep=0;
					//ʱ����ͬ������򿪾�����
		}
} 
}
