#ifndef __1602_H__
#define __1602_H__

#define uchar unsigned char 
#define uint unsigned int
	
void Init_DS18B20(void);
void delay(uint x);
void Init_DS18B20(void);
uchar Read_OneChar(void);
void Write_OneChar(uchar dat);
uchar Read_Temperature(void);
void write_temprom(uchar temp_up_set,uchar temp_down_set );
void read_temprom(void);