#ifndef _COLINLUAN_LUNAR_H_
#define _COLINLUAN_LUNAR_H_

#define uchar unsigned char
 
typedef struct spdate
{
	char 	cYear;
	char	cMon;
	char	cDay;
}SPDATE;

//extern SPDATE Spdate;



extern SPDATE GetSpringDay(uchar GreYear,uchar GreMon,uchar GreDay); 		 //���ũ������
extern bit YearFlag(uchar cYear);											 //�ж�ƽ�꣬����
extern uchar GetWeekDay(uchar cYear,uchar cMon,uchar cDay);					 //�жϵ�ǰ�������ڼ�

#endif