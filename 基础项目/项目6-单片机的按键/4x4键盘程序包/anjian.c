#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
code uchar key_scan[4] = {0xfe,0xfd,0xfb,0xf7};
code uchar key_value[16] = {0xee,0xde,0xbe,0x7e,
								  0xed,0xdd,0xbd,0x7d,
								  0xeb,0xdb,0xbb,0x7b,
								  0xe7,0xd7,0xb7,0x77,};
code seven_seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar k = 88;
void delay(uint x)
{
	while(x--);
}
void display(uchar x)
{
	P0 = 0xff;                 //仿真时消隐
	P0 = seven_seg[x % 10];
	P2 = 0xfe;delay(1000);
	P0 = 0xff; 	               //仿真时消隐
	P0 = seven_seg[x / 10];
	P2 = 0xfd;delay(1000);
}
void main(void)
{
	uchar i = 0,j;
	while(1)
	{
		P3 = key_scan[i];
		if(P3 != key_scan[i])
		{
			delay(200);
			for(j = 0;j <= 16;j++)
			{
				if(P3 == key_value[j])
				k = j;
			}
		}
		display(k);
		i++;
		if(i >= 4)
		i = 0;
	}
}