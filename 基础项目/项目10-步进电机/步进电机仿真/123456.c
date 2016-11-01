#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int 
uchar code FFW[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};//ת������
uchar code REV[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1};
uchar key1_down,key2_down,key3_down,key1_flag,key2_flag,key3_flag;//��������
sbit key1 = P3^0;//��������
sbit key2 = P3^1;
sbit key3 = P3^2;
/************************��������****************************/
/****************�����ӳٺ���**********/
void delay()
{
	uchar a,b;
	for(a=0;a<25;a++)
	for(b=0;b<255;b++);
}
void key_scan()
{
	/************����1*************/
	if(key1 == 0)
	{
		delay();
		if(key1 == 0)key1_down = 1;	
	}
	if(key1_down == 1 && key1 == 1)
	{
		key1_flag = 1;
		key2_flag = 0;
		key3_flag = 0;
		key1_down = 0;	
	}
	/************����2*************/
	if(key2 == 0)
	{
		delay();
		if(key2 == 0)key2_down = 1;	
	}
	if(key2_down == 1 && key2 == 1)
	{
		key1_flag = 0;
		key2_flag = 1;
		key3_flag = 0;
		key2_down = 0;	
	}
	/************����3*************/
	if(key3 == 0)
	{
		delay();
		if(key3 == 0)key3_down = 1;	
	}
	if(key3_down == 1 && key3 == 1)
	{
		key1_flag = 0;
		key2_flag = 0;
		key3_flag = 1;
		key3_down = 0;	
	}
}
/****************ת���ӳٺ���**********/
void delayB(uint t)
{                           
   uchar k;
   while(t--)
   {
    for(k=0; k<125; k++)
    { }
   }
}
/************************�����ת****************************/
void motor_ffw()
{ 
   	uchar i;
	for (i=0; i<8; i++)       //һ������ת30��
    {
       	P1 = FFW[i];          //ȡ����
        delayB(100);            //����ת��
    }
 }
/************************�����ת****************************/
void motor_rev()
{
 	uchar i;
	for (i=0; i<8; i++)     //һ������ת30��
    {
       	P1 = REV[i];          //ȡ����
        delayB(100);            //����ת��
    }
}
/************************������****************************/
main()
{ 
   	while(1)
    {  
      	key_scan();
	if(key1_flag == 1)motor_ffw();       //�����ת
	if(key2_flag == 1)motor_rev();		//�����ת
      	if(key3_flag == 1) P1 = 0x0f;			//���ֹͣ
	}
}