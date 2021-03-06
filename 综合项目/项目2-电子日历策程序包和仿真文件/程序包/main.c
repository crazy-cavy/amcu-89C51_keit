/*********************************************************/
/*主要设计者：											      		*/
/*完成日期：2010年5月15日											*/
/*********************************************************/
#include <reg52.h>

#include "12864.h"
#include "model.h"
#include "ds1302.h"
#include "one-wire.h"
#include "lunar.h"
#include "input_080627.h"
//#include "yinli.h"

#define uchar unsigned char
#define uint unsigned int

#define NoUpLine 	1
#define UpLine   	0
#define NoUnderLine 1
#define UnderLine	0

#define FALSE	0
#define TRUE    1


uchar bdata gTmp;
sbit t0 = gTmp^0;
sbit t1 = gTmp^1;
sbit t2 = gTmp^2;
sbit t3 = gTmp^3;
sbit t4 = gTmp^4;
sbit t5 = gTmp^5;
sbit t6 = gTmp^6;
sbit t7 = gTmp^7;


uchar dispBuf[7];
uchar T0_Count=0,Tmp_Count=0;
bit T0_Flag,Tmp_Flag,Flash_Flag;

SYSTIME sys;		   //系统日期
SPDATE SpDat;		   //农历日期

//设置时间的标志
bit Hour_Flag=TRUE,Min_Flag=TRUE,Sec_Flag=TRUE;
bit Year_Flag=TRUE,Mon_Flag=TRUE,Day_Flag=TRUE;

uchar State_Set=0;     //设置状态 (用来区分此时是在设置时，分，秒，日，月，年)   

bit   State_Flag=FALSE,Inc_Flag=FALSE,Dec_Flag=FALSE;	//用来标志三个按键是否按下。

uchar code Mon2[2][13]={0,31,28,31,30,31,30,31,31,30,31,30,31,
				  0,31,29,31,30,31,30,31,31,30,31,30,31};

/***********************************************************
	cDat:		要显示的数
	X:			行数 0~7
	Y:          列数 0~127
	show_flag:  是否反白显示，0反白，1不反白 
	upline: 	上划线, 0表示带上划线
	underline:  下划线, 0表示带下划线
	返回:       无
************************************************************/
void LCD_ShowTime(char cDat,uchar X,uchar Y,bit show_flag,bit up,bit under)
{
	uchar s[2];
   	s[0]=cDat/10+'0';
	s[1]=cDat%10+'0';
	en_disp(X,Y,2,Asc,s,show_flag,up,under);
}

/***********************************************************
// 函数名:      Show_YMD()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        显示公历与农历的年、月、日、星期
// 说明:        普通模式每天刷新一次
//				设置模式每次循环刷新一次

***********************************************************/
void Show_YMD()		//显示年月日星期
{
	uchar uiTempDat;
   	uiTempDat=RDS1302(0x88|0x01);
	sys.cMon=((uiTempDat&0x1f)>>4)*10+(uiTempDat&0x0f);	
	LCD_ShowTime(sys.cMon,2,5,Mon_Flag,NoUpLine,NoUnderLine);		    //月
	hz_disp(4,5,1,uMod[1],1,NoUpLine,NoUnderLine);
	Show16X32(2,27,ucNum3216[sys.cDay/10],Day_Flag);
	Show16X32(2,43,ucNum3216[sys.cDay%10],Day_Flag);  				    //年

	/////////////////////////////////////////////////////////////////////

	hz_disp(6,8,2,ucLunar[13],1,UpLine,UnderLine);
	if(sys.cWeek==7)
	hz_disp(6,40,1,uMod[2],1,UpLine,UnderLine);					 //星期日
	else
    hz_disp(6,40,1,ucLunar[sys.cWeek],1,UpLine,UnderLine);		 //星期

  	/////////////////////////////////////////////////////////////////////				
										   
    LCD_ShowTime(20,0,9,1,UpLine,UnderLine);
  	LCD_ShowTime(sys.cYear,0,25,Year_Flag,UpLine,UnderLine);
    hz_disp(0,41,1,uMod[0],1,UpLine,UnderLine);					  //年
				   

	////////////////////////////////////////////////////

	SpDat=GetSpringDay(sys.cYear,sys.cMon,sys.cDay);	//得到农历日期

//显示农历日期
    if(SpDat.cMon==1)											   //农历月
	  hz_disp(4,64,1,ucLunar[15],1,UpLine,NoUnderLine);			   //"正"
	else if(SpDat.cMon==11)
	  hz_disp(4,64,1,ucLunar[16],1,UpLine,NoUnderLine);			   //"冬"
	else if(SpDat.cMon==12)
	  hz_disp(4,64,1,ucLunar[17],1,UpLine,NoUnderLine);			   //"腊"
	else
	  hz_disp(4,63,1,ucLunar[SpDat.cMon],1,UpLine,NoUnderLine);    //"二"~"十"



	 if(SpDat.cDay/10==1 && SpDat.cDay%10>0)					   //显示"十" 例如"十四"而不是"一四"
	 hz_disp(4,95,1,ucLunar[10],1,UpLine,NoUnderLine);
	 else if(SpDat.cDay/10==2 && SpDat.cDay%10>0)                  //显示"廿" 例如"廿三"而不是"二四"
	 hz_disp(4,95,1,ucLunar[19],1,UpLine,NoUnderLine);
	 else
	 hz_disp(4,95,1,ucLunar[SpDat.cDay/10],1,UpLine,NoUnderLine);  //正常显示数字

	if(!(SpDat.cDay%10))	   //								   //"十"
	 hz_disp(4,111,1,ucLunar[10],1,UpLine,NoUnderLine);
	else															//正常数字
	 hz_disp(4,111,1,ucLunar[SpDat.cDay%10],1,UpLine,NoUnderLine);

	 ////////////////////////////////////////////////////
//显示生肖
	 hz_disp(0,104,1,SX[(uint)(2000+SpDat.cYear)%12],1,UpLine,UnderLine);   //生肖

	 ////////////////////////////////////////////////////
//显示天干地支
	 hz_disp(2,95,1,TianGan[(uint)(2000+SpDat.cYear)%10],1,NoUpLine,NoUnderLine); //天干
	 hz_disp(2,111,1,DiZhi[(uint)(2000+SpDat.cYear)%12],1,NoUpLine,NoUnderLine);  //地支

}

/**************************************************************
// 函数名:      LCD_ShowTemp()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        采集温度并显示
// 说明:        30秒更新一次

***************************************************************/
void LCD_ShowTemp()
{
	uchar datTm[2];
	
	float temp;
	uchar buftmp;
	
	 	 	
	if(!Tmp_Flag) return;
	Tmp_Flag=FALSE;

	DS18B20_Init();
	DS18B20_RomCmd(0xcc);	//Skip Rom command;
	Delay_Xms(20);
	DS18B20_MemCmd(0x44);	//Convert T command;
	
	DS18B20_Init();
	DS18B20_RomCmd(0xcc);
	DS18B20_MemCmd(0xbe);
	DS18B20_ReadDat(2,datTm);
		
	gTmp = datTm[0];
	temp = (uchar)t0 * 0.0625 + (uchar)t1 * 0.125 + (uchar)(t2) * 0.25 + (uchar)(t3) * 0.5;
	buftmp = (uchar)(temp*10);
	dispBuf[2] = '.';
	dispBuf[3] = buftmp + '0';
	
	buftmp = (uchar)t4 + (uchar)t5*2 + (uchar)t6*4 + (uchar)t7*8;
	gTmp = datTm[1];
	gTmp &= 0x07;
	buftmp += (gTmp * 16);
	dispBuf[0] = (buftmp/10) + '0';
	dispBuf[1] = (buftmp%10) + '0';
	
	dispBuf[4] = 0xdf;		//摄氏度；
	dispBuf[2] = 'C';
	en_disp(0,65,2,Asc,dispBuf,1,UpLine,UnderLine); 
}


/***********************************************************
// 函数名:      LCD_ShowWNL()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        显示万年历
// 说明:        

***********************************************************/
void LCD_ShowWNL()
{
	

	LCD_ShowTime(sys.cSec,6,111,Sec_Flag,UpLine,UnderLine);		 //秒   每次循环钟刷新一次

	if(!sys.cSec || State_Set)								     //分   在普通模式时每分钟刷新一次
	LCD_ShowTime(sys.cMin,6,87,Min_Flag,UpLine,UnderLine);		 //     在设置模式时每次循环刷新一次
	 
	if(!sys.cSec && !sys.cMin || State_Set)						 //时   在普通模式时每小时刷新一次
    LCD_ShowTime(sys.cHour,6,63,Hour_Flag,UpLine,UnderLine);     //     在设置模式时每次循环刷新一次
	
	if(!sys.cSec && !sys.cMin && !sys.cHour || State_Set )       //公历农历的年、月、日、星期
	{															 //普通模式每天刷新一次
		Show_YMD(); 											 //设置模式每次循环刷新一次
		if(State_Set==7) State_Set=0;
	}	
}

/***********************************************************
// 函数名:      CAL_Init()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        日期初始化
// 说明:        星期通过计算得到

***********************************************************/
void CAL_Init()
{
	sys.cYear=0x09;
	sys.cMon=0x05;
	sys.cDay=0x05;
	sys.cHour=0x10;
	sys.cMin=0x27;
	sys.cSec=0x00;
	sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);
}


/***********************************************************
// 函数名:      SFR_Init()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        寄存器初始化
// 说明:        定时器0控制温度采集，定时器1控制按键扫描

***********************************************************/
void SFR_Init()
{
	T0_Flag=FALSE;
	Tmp_Flag=TRUE;
	Flash_Flag=FALSE;
	TMOD=0x11;
	ET0=1;
	ET1=1;
	TH0= (-50000)/256;
	TL0= (-50000)%256;
	TH1= (-10000)/256;
	TL1= (-10000)%256;
	EA=1;
}

/***********************************************************
// 函数名:      GUI_Init()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        图形界面初始化
// 说明:        一些永远不耍更新的图形(如线条，"年"、"月"、"日"等)
				只是在初始化时写入，中间过程不再更新。

***********************************************************/
void GUI_Init()
{
	 uint i,j;
	 ////////进入界面///////////
	 LCD12864_init();
	 ClearLCD();
	 LCD_ShowTemp();		//在显示图片之前启动温度转换
	 img12864_disp(uBmp);	//显示图片
	 
	 for(i=0;i<10;i++)
	 for(j=0;j<32767;j++);
								 
	 ClearLCD();

	 /////////描绘框架//////////
	 Rect(0,0,127,63,1);
	 Line(62,0,62,62,1);
	 Line(0,48,127,48,1);
	 Line(0,15,127,15,1);
	 Line(24,15,24,48,1);

	 Line(63,32,128,32,1);
	 Line(97,0,97,15,1);
	 //Line(24,15,24,48,1);

	 ////////时间初始化//////////
	 SetTime(sys);				   //设置时间
	 GetTime(&sys);				   //获得时间
	 
	 Show_YMD();

	 LCD_ShowTime(sys.cSec,6,111,Sec_Flag,UpLine,UnderLine);
	 en_disp(6,103,1,Asc,":",1,UpLine,UnderLine);
	 LCD_ShowTime(sys.cMin,6,87,Min_Flag,UpLine,UnderLine); 
	 en_disp(6,79,1,Asc,":",1,UpLine,UnderLine);	 
     LCD_ShowTime(sys.cHour,6,63,Hour_Flag,UpLine,UnderLine);

	 hz_disp(2,64,1,ucLunar[11],1,NoUpLine,NoUnderLine);	   //"农"
	 hz_disp(2,80,1,ucLunar[12],1,NoUpLine,NoUnderLine);	   //"历"
	 hz_disp(4,79,1,uMod[1],1,UpLine,NoUnderLine);			   //"月"
	 ////////显示初始化温度//////
	 en_disp(0,65,2,Asc,dispBuf,1,UpLine,UnderLine); 
	 hz_disp(0,81,1,ucLunar[18],1,UpLine,UnderLine); 
}


/***********************************************************
// 函数名:      DecToBCD()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        把时间由10进制转为BCD
// 说明:        DS1302以BCD存储

***********************************************************/
void DecToBCD()
{
	sys.cHour=(((sys.cHour)/10)<<4)+((sys.cHour)%10);
	sys.cMin=(((sys.cMin)/10)<<4)+((sys.cMin)%10);
	sys.cSec=((sys.cSec/10)<<4)+((sys.cSec)%10);
  	sys.cYear=((sys.cYear/10)<<4)+((sys.cYear)%10);
	sys.cMon=((sys.cMon/10)<<4)+((sys.cMon)%10);
	sys.cDay=((sys.cDay/10)<<4)+((sys.cDay)%10);
}

/***********************************************************
// 函数名:      Time_Set()
// 入口参数:    void 
// 出口参数：　 void 
// 功能:        时间设置
// 说明:        具体的时间设置

***********************************************************/
void Time_Set()
{
   	
	if(State_Flag)					//设置键按下
	{
		State_Flag=FALSE;
		State_Set++;
		//State_Set%=7;
		if(State_Set==8) State_Set=0;
	}

	Hour_Flag=TRUE;Min_Flag=TRUE;Sec_Flag=TRUE;
    Year_Flag=TRUE;Mon_Flag=TRUE;Day_Flag=TRUE;

	switch(State_Set)               //设置类型
	{
			case 0:                 //无设置
			
			break;
			 
			case 1:					//设置时
			Hour_Flag=FALSE;
			break;

			case 2:	   				//设置分
			Min_Flag=FALSE;
			break;
	
	
			case 3:					 //设置秒
			Sec_Flag=FALSE;
			break;
	
			case 4:					 //设置天
			 Day_Flag=FALSE;
			break;
	
			case 5:
			 Mon_Flag=FALSE;         //设置月
			break;
	
			case 6:
			 Year_Flag=FALSE;		 //设置年
		    break;

			case 7:					 //无动作，设置此值为让"年"的反白消失。

			break;
	  }	   
	  
	if(Inc_Flag)					  //加键被按下
	{   
		Inc_Flag=FALSE;
		switch(State_Set)
		{
			case 0:
			
			break;
	
			case 1:	   				 //小时加
			sys.cHour++;
			(sys.cHour)%=24;
			break;
	
			case 2:		   			 //分加
			sys.cMin++;
			sys.cMin%=60;
			break;
	
			case 3:					 //秒加
		    sys.cSec++;
			sys.cSec%=60;
			break;
	
			case 4:					 //天加
			(sys.cDay)=(sys.cDay%Mon2[YearFlag(sys.cYear)][sys.cMon])+1;		   
			break;
	
			case 5:					 //月加
			sys.cMon=(sys.cMon%12)+1;
			break;
	
			case 6:
			sys.cYear++;			 //年加
			sys.cYear=sys.cYear%100;
			break;
		  }	
		sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);    //算出星期
		DecToBCD();			//转为BCD
		SetTime(sys);	    //存入DS1302
	}

	if(Dec_Flag)					 //减键按下
	{   
		Dec_Flag=FALSE;
		switch(State_Set)
		{
			case 0:
			
			break;
	
			case 1:	   
			sys.cHour=(sys.cHour+23)%24;  			//时减
			break;
	
			case 2:		  						    //分减
			sys.cMin=(sys.cMin+59)%60;
			break;
	
			case 3:									//秒减
			sys.cSec=(sys.cSec+59)%60;
			break;
	
			case 4:									//天减
			sys.cDay=((sys.cDay+Mon2[YearFlag(sys.cYear)][sys.cMon]-1)%Mon2[YearFlag(sys.cYear)][sys.cMon]);		   
			if(sys.cDay==0) sys.cDay=Mon2[YearFlag(sys.cYear)][sys.cMon];
			break;
	
			case 5:									//月减
			sys.cMon=(sys.cMon+11)%12;
			if(sys.cMon==0) sys.cMon=12;
			break;
	
			case 6:									//年减
			sys.cYear=(sys.cYear+99)%100;
			break;
		  }	
		sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);     	 
		DecToBCD();
		SetTime(sys);	
	}



}
void  main()
{
	 SFR_Init();
	 CAL_Init();
	 GUI_Init();
	 TR0=1;
	 TR1=1;
	 while(1)	 
	 {
		GetTime(&sys);		//获得时间
		LCD_ShowTemp();		//显示温度
		LCD_ShowWNL();		//显示万年历
		Time_Set();         //时间设置
	 }	
}

void timer0() interrupt  1     //控制温度采集
{
	TH0= (-50000)/256;
	TL0= (-50000)%256;
	T0_Count++;
	if(T0_Count==20)
	{	
		Tmp_Count++;
		T0_Count=0;
	}

	if(Tmp_Count==33)
	{
		Tmp_Count=0;
		Tmp_Flag=TRUE;
	}
}


void timer1() interrupt  3     //按键扫描
{
	TH1= (-10000)/256;
	TL1= (-10000)%256;
	keyinput();						// 设置
	if (keyvalue&0x10)
	{	
		State_Flag=TRUE;
		keyvalue &= 0xef;		//清键值，保证一直按下只执行一次按键动作。
	}	
//精度键检测  //080616
	
	if (keyvalue&0x20 )				//加
	{	
		Inc_Flag=TRUE;
		keyvalue &= 0xdf;		//清键值，保证一直按下只执行一次按键动作。
	}	
	
//蜂鸣器键检测  //080616
	if (keyvalue&0x40)			   //减
	{	
		Dec_Flag=TRUE;
		keyvalue &= 0xbf;		//清键值，保证一直按下只执行一次按键动作。
	}
	
}