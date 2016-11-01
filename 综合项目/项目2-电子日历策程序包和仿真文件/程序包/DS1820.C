/*#include <reg52.h>
#include <intrins.h>

#define uchar unsigned  char 
#define uint  unsigned  int

sbit DQ=P1^6;
uchar ucDat;

void  Delay15(uchar n)
 { 
	while(n)
	{	
		_nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
		_nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
		n--;
    }
  }
 //毫秒延时
 void DelayHM(uchar n)
  {
  	uchar i,j;
	while(n--)
	{
		for(i=0;i<10;i++)
		for(j=0;j<100;j++);
	}
  }		 

//该函数返回为0，表示有DS1820，否则没有
bit Reset(void) 
 { 
   bit bErr;
   DQ=1;
   _nop_();
   DQ=0;
   Delay15(40);
   DQ=1;
   Delay15(3);
   bErr=DQ;
   Delay15(14);
   return(bErr);
 }

void WrByte(uchar ucDat)
 { 
   uchar i;
   for(i=8;i>0;i--)
     { 
	   if(ucDat & 0x01)
	   {
	   		DQ=0;
       		_nop_();
	   		_nop_();
	   		_nop_();
	   		DQ=1;
	   }
	   else
	   {
	   		DQ=0;
       		_nop_();
	   		_nop_();
	   		_nop_();
	   }
	   Delay15(2);
       DQ=1;
	   ucDat=ucDat>>1;
      }
 }

 uchar  RdByte(void)
  {  
	 uchar i;
     ucDat=0;
     for(i=8;i>0;i--)
     {  
		ucDat=ucDat>>1;
		DQ=0;
		_nop_();
        DQ=1;
        Delay15(1);
		if(DQ) ucDat|=0x80;
        //dat7=DQ;
      }
     return(ucDat);
  }    
   
void Convert(void)
 { 
   bit bErr;
   bErr=Reset();
   WrByte(0xcc);  //跳过多传感器识别
   WrByte(0x44);  //启动温度转换
 }
 
int Readt(void)
 {  
	uchar h,l;
    bit bErr;
    bErr=Reset();
    WrByte(0xcc); //跳过多传感器识别
    WrByte(0xbe); //读暂存器
    l=RdByte(); //温度低位
    h=RdByte(); //温度高位
    
    return(h*256+l);
 }		 */

#define uchar unsigned  char 
#define uint  unsigned  int
#include <reg51.h>
#include <intrins.h>


extern uchar dbuf[4];

sbit DQ=P1^6;
bdata  uchar dat;
sbit   dat0=dat^0;
sbit   dat7=dat^7;

//uchar code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c};
uchar dbuf[4]={0,0,0,0};// 显示缓存



void  Delay15(uchar n)
 { do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
		n--;
        }while(n);
  }
//2*x+5
void Delay_Xus(uchar x)		//x取值1～255；
{
	while(--x);
}

//该函数返回为0，表示有DS1820，否则没有
bit Reset(void) 
 { bit err;
   DQ=0;
   Delay15(40);
   DQ=1;
   Delay15(6);
   err=DQ;
   Delay15(18);
   return(err);
 }

void WrByte(uchar d)
 { uchar i;
   dat=d;
   for(i=8;i>0;i--)
     { DQ=0;
       Delay15(1);
	   DQ=dat0;
	   dat=dat>>1;
	   Delay15(1);
       DQ=1;
      }
 }

/* uchar  RdByte(void)
  {  uchar i;
     dat=0;
     for(i=8;i>0;i--)
     {  dat=dat>>1;
		DQ=0;
		_nop_();
        DQ=1;
        Delay15(1);
        dat7=DQ;
      }
     return(dat);
  }    		 */

uchar RdByte(void)
{
	uchar i;
	uchar tmp = 0;
	DQ = 1;							//准备读；
	_nop_();
	for(i=0;i<8;i++)
	{
		tmp >>= 1;				//低位先发；
		DQ = 0;						//Read init；
		_nop_();					//2ms；
		DQ = 1;						//必须写1，否则读出来的将是不预期的数据；
		Delay_Xus(2);			//延时9us；
		_nop_();
		if(DQ)						//在12us处读取数据；
			tmp |= 0x80;
		Delay_Xus(2);		//延时55us；
		DQ = 1;						//恢复One Wire Bus；
		_nop_();;
	}
	return tmp;	
} 

void Convert(void)
 { bit err;
   err=Reset();
   WrByte(0xcc);  //跳过多传感器识别
   WrByte(0x44);  //启动温度转换
 }
 
int Readt(void)
 {  uchar h,l;
    bit err;
    err=Reset();
    WrByte(0xcc); //跳过多传感器识别
    WrByte(0xbe); //读暂存器
    l=RdByte(); //温度低位
    h=RdByte(); //温度高位
    
   // return(h*256+l);
	return l;
 }
