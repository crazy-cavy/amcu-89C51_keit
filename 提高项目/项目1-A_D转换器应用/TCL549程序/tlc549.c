/*
*程 序 最 后 修 改 时 间  06-8-9 22:08
*软 件 标 题：TLC549 AD采样子程序
*软 件 说 明：从TLC549读取采样值
*_________________________________________
*
*/

#include "intrins.h"

#define    Wait1us        _nop_();
#define    Wait2us        {_nop_();_nop_();}
#define    Wait4us        {Wait2us;Wait2us;}
#define    Wait8us        {Wait4us;Wait4us;}
#define    Wait10us      {Wait8us;Wait2us;}
#define    Wait30us      {Wait10us;Wait8us;Wait4us;Wait2us;}


/************定义接口总线**************/
sbit Clock = P1 ^ 5; //时钟口线
sbit DataOut = P1 ^ 4; //数据输出口线
sbit ChipSelect = P1 ^ 3; //片选口线
/************接口总线定义结束**********/


/*********函数名称：ADCSelChannel********
*
*功能：转换指定通道的模拟量为数字量
*
*说明：
*
*位置：1
*
****************************************/
unsigned char ADCSelChannel(void)
{
      unsigned char ConvertValue = 0;
      unsigned char i;
      ChipSelect = 1; //芯片复位
      ChipSelect = 0;
      ChipSelect = 1;
      Clock = 0;
      Wait4us;
      ChipSelect = 0; //芯片起始
      Wait4us; //等待延时
      for (i = 0; i < 8; i ++) //输入采样转换时钟
      {
            Clock = 1;
            Clock = 0;
      }
      ChipSelect = 1; //开始转换
      Wait10us; //等待转换结束
      ChipSelect = 0; //读取转换结果
      Wait4us;
      for (i = 0; i < 8; i ++) //
      {
            Clock = 1;
            ConvertValue <<= 1;
            if (DataOut)
            {
                   ConvertValue = 0x1;
            }
            Clock = 0;
      }
      ChipSelect = 1;
      return (ConvertValue); //返回转换结果
}



