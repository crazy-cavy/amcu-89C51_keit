#ifndef  _COLINLUAN_DS1820_H_
#define	 _COLINLUAN_DS1820_H_

extern void  Delay15(uchar n);
extern bit Reset();
extern void WrByte(uchar d);
extern uchar  RdByte();
extern void Convert();
extern int Readt(void);

#endif