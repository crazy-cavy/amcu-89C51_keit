
#define int8 unsigned char
/*---------------------------------------------------------------------
��ֵ������
                  
   RB4(KEY_OUT1)   KEY_ONOFF
   RB5(KEY_OUT2)   KEY_ACCU
   RB6(KEY_OUT3)   KEY_BUZZER
   RB7(KEY_OUT4)   KEY_BACKLED
------------------------------------------------------------------------*/

#define  KEY1     4
#define  KEY2     5
#define  KEY3     6
#define  KEY4     7


//��ֵ����
#define   KEY_NONE       	0x00
#define   KEY_ONOFF       	0x10
#define   KEY_ACCU       	0x20
#define   KEY_BUZZER      	0x40     
#define   KEY_BACKLED      	0x80     

#define   KEYCOUNT_DEF 		1             	//10ms*1=10ms  ȥ��ʱ��

int8  keyvalue=KEY_NONE;
int8  key1count_dn =0,key1count_up =0;		//����ȥ��������
int8  key2count_dn =0,key2count_up =0;
int8  key3count_dn =0,key3count_up =0;
int8  key4count_dn =0,key4count_up =0;

void keyinput(void)
{
	int8  portb_temp;
    portb_temp=P2;
	//Power
    if(!(portb_temp&0x10))  //�����£�dn������
    {
    	if (key1count_up<KEYCOUNT_DEF) key1count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key1count_dn++;
     	if (key1count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x10;key1count_up= 0; }  
     	if (key1count_dn> KEYCOUNT_DEF) 	key1count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else //������up������
	{
		if (key1count_dn<KEYCOUNT_DEF) key1count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key1count_up++;
	 	if (key1count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xef;key1count_dn=0;}
     	if (key1count_up>KEYCOUNT_DEF)		key1count_up = KEYCOUNT_DEF;	
	}   
    // ACCU KEY
    if(!(portb_temp&0x20))  //�����£�dn������
    {
    	if (key2count_up<KEYCOUNT_DEF) key2count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key2count_dn++;
     	if (key2count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x20;key2count_up= 0; }  
     	if (key2count_dn> KEYCOUNT_DEF) 	key2count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else //������up������
	{
		if (key2count_dn<KEYCOUNT_DEF) key2count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key2count_up++;
	 	if (key2count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xdf;key2count_dn=0;}
     	if (key2count_up>KEYCOUNT_DEF)		key2count_up = KEYCOUNT_DEF;	
	}      
     
    // BUZZER KEY
    if(!(portb_temp&0x40))  //�����£�dn������
    {
    	if (key3count_up<KEYCOUNT_DEF) key3count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key3count_dn++;
     	if (key3count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x40; key3count_up= 0;}  
     	if (key3count_dn> KEYCOUNT_DEF) 	key3count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else //������up������
	{
		if (key3count_dn<KEYCOUNT_DEF) key3count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key3count_up++;
	 	if (key3count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xbf;key3count_dn=0;}
     	if (key3count_up>KEYCOUNT_DEF)		key3count_up = KEYCOUNT_DEF;	
	}      
	//�ƹ�����
    if(!(portb_temp&0x80))  //�����£�dn������
    {
    	if (key4count_up<KEYCOUNT_DEF) key4count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key4count_dn++;
     	if (key4count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x80; key4count_up= 0;}  
     	if (key4count_dn> KEYCOUNT_DEF) 	key4count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else //������up������
	{
		if (key4count_dn<KEYCOUNT_DEF) key4count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key4count_up++;
	 	if (key4count_up==KEYCOUNT_DEF) 	{ keyvalue&=0x7f;key4count_dn=0;}
     	if (key4count_up>KEYCOUNT_DEF)		key4count_up = KEYCOUNT_DEF;	
	}   	
}

