

//#include "Includes.h"			//文件包含
#include 		<reg52.h>
#include    <intrins.h>
#include    <string.h>
#include 	"One-Wire.h"

#define int8u unsigned char

/*************************************************
    模块级变量申明                 
*************************************************/
/* void 	OneWire_Rst(void);
 void 	OneWire_Init(void);
 void 	OneWire_WriteByte(int8u cmd);
 int8u 	OneWire_ReadByte(void);
 void 	OneWire_Idle(void);	  */

 void Delay_Xus(int8u x)		//x取值1～255；
{
	while(--x);
//	int8u i;
//	x /= 8;
//	for(i=0;i<x;i++);
	}

 void Delay_Xms(int8u x)		//x取值1～255；
{
	int8u i,j;
	for(i=0;i<x;i++)
	for(j=0;j<200;j++)
	{
		_nop_();
		_nop_();
	}
}

/*************************************************
    子函数定义                  
*************************************************/


 void OneWire_Init(void)
{
	DQ = 1;
	_nop_();;
	DQ = 0;						//拉低数据线，准备Reset OneWire Bus；
	Delay_Xus(125);		//延时510us，Reset One-Wire Bus.
	Delay_Xus(125);
	
	DQ = 1;						//提升数据线；
	Delay_Xus(15);		//延时35us；
	
	while(DQ)					//等待Slave 器件Ack 信号；
	{
		_nop_();;
		}
	Delay_Xus(60);		//延时125us；	
	DQ = 1;						//提升数据线，准备数据传输；
	}

 void OneWire_Rst(void)
{
	DQ = 0;						//拉低数据线，准备Reset OneWire Bus；
	Delay_Xus(125);		//延时511us，Reset One-Wire Bus.
	DQ = 0;
	Delay_Xus(125);
	}
/*******************************************************************************
*** 函 数 名:  void OneWire_WriteByte(int8u cmd)
*** 功能描述: 写一个字节；
*** 全局变量: NO !
*** 输		入: cmd：8bit；
*** 输		出: NO !
*** 函数说明: cmd主要有Rom操作指令和E2Ram操作指令两种；
/******************************************************************************/
 void OneWire_WriteByte(int8u cmd)
{
	int8u i;
	for(i=0;i<8;i++)
	{
		if(cmd & 0x01)		//低位在前；
			{
				DQ = 0;				//结束Recovery time；
				_nop_();;
				_nop_();;
				_nop_();;
				DQ = 1;				//发送数据；
				}
		else
			{
				DQ = 0;				//结束Rec time；
				_nop_();;
				_nop_();;
				_nop_();;		
				//DQ = 0;			//发送数据；
				}
		Delay_Xus(30);		//等待Slave Device采样；
		DQ = 1;						//Recovery;
		_nop_();;						//Recovery Time Start;
		cmd >>= 1;
		}
	}
/*******************************************************************************
*** 函 数 名:  int8u OneWire_ReadByte(void)
*** 功能描述: 读取一个字节；
*** 全局变量: NO !
*** 输		入: NO !
*** 输		出: 8bit data；
*** 函数说明: 读之前必须首先写1；
/******************************************************************************/
 int8u OneWire_ReadByte(void)
{
	int8u i;
	int8u tmp = 0;
	DQ = 1;							//准备读；
	_nop_();;
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
		Delay_Xus(30);		//延时55us；
		DQ = 1;						//恢复One Wire Bus；
		_nop_();;
		}
	return tmp;	
	}
/*******************************************************************************
*** 函 数 名:  void OneWire_Idle(void)
*** 功能描述: 总线空闲模式；
*** 全局变量: NO !
*** 输		入: NO !
*** 输		出: NO !
*** 修 改 人：			 							日期：2006-11-28
*** 函数说明: 
/******************************************************************************/
 void OneWire_Idle(void)
{
	DQ = 1;
	_nop_();;
	}
/*******************************************************************************
*** 函 数 名:  void DS18B20_Init(void)
*** 功能描述: 器件DS18B20初始化；
*** 全局变量: NO !
*** 输		入: NO !
*** 输		出: NO !
*** 修 改 人：			 							日期：2006-11-28
*** 函数说明: 
/******************************************************************************/
 void DS18B20_Init(void)
{
	OneWire_Init();
	}
/*******************************************************************************
*** 函 数 名:  void DS18B20_RomCmd(int8u romCmd)
*** 功能描述: DS18B20Rom操作命令写；
*** 全局变量: NO !
*** 输		入: romCmd；
*** 输		出: NO !

*** 函数说明: 0x33:Read Rom; 0x55:Match Rom; 0xcc:Skip rom; 0xf0: Search rom;
							0xec:Alarm Search.
/******************************************************************************/
 void DS18B20_RomCmd(int8u romCmd)		//仅写了SkipRom例程；
{
	switch(romCmd)
	{
		case 0xcc:			//Skip Rom command;
			OneWire_WriteByte(0xcc);
			break;
		
		default:
			break;	
		}
	}
/*******************************************************************************
*** 函 数 名:  void DS18B20_MemCmd(int8u memCmd)
*** 功能描述: DS18B20Memory 操作命令；
*** 全局变量: NO !
*** 输		入: NO !
*** 输		出: NO !

*** 函数说明: 仅写了ConvertT(0x44) 和0xBE命令；
							0x44:Convert T; 0xbe:Read Scratch-pad;
/******************************************************************************/
 void DS18B20_MemCmd(int8u memCmd)		
{
	switch(memCmd)
	{
		case 0x44:		//Start to Convert temperature;
			OneWire_WriteByte(0x44);
			_nop_();;
			DS18B20_Busy();
			Delay_Xms(250);		//延时750ms；
			Delay_Xms(250);
			Delay_Xms(250);
			break;
		
		case 0xbe:		//Read scratchpad;
			OneWire_WriteByte(0xbe);
			break;
				
		default:
			break;	
		}
	}
/*******************************************************************************
*** 函 数 名:  void DS18B20_ReadDat(int8u len,int8u *buf)
*** 功能描述: 读取DS18B20内部的Scratchpad内容；
*** 全局变量: NO !
*** 输		入: len：将要读取的数据长度；
*** 输		出: buf：数据缓存；
*
*** 函数说明: 
/******************************************************************************/
 void DS18B20_ReadDat(int8u len,int8u *buf)
{
	int8u i;
	for(i=0;i<len;i++)
	{
		buf[i] = OneWire_ReadByte();
		}
	OneWire_Rst();
	OneWire_Idle();	
	}
/*******************************************************************************
*** 函 数 名:  void DS18B20_Busy(void)
*** 功能描述: 内部模数转换中，忙
*** 全局变量: NO !
*** 输		入: NO !
*** 输		出: NO !

*** 函数说明: 
/******************************************************************************/
 void DS18B20_Busy(void)
{
	OneWire_Idle();
	}


/*******************************************************************************
****                              End Of File
*******************************************************************************/
