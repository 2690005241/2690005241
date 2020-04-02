#include "smg.h"

typedef unsigned int u16;
typedef unsigned char u8;
 #define IO_dig P0;
sbit LSA=P2^0;
sbit LSB=P2^1;
sbit LSC=P2^2;
void delay(u16 i);
void digdisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
			switch(i)
			{
				case(0):LSA=0;LSB=0;LSC=0;break;
				case(1):LSA=1;LSB=0;LSC=0;break;
				case(2):LSA=0;LSB=1;LSC=0;break;
				case(3):LSA=1;LSB=1;LSC=0;break;
				case(4):LSA=0;LSB=0;LSC=1;break;
				case(5):LSA=1;LSB=0;LSC=1;break;
				case(6):LSA=0;LSB=1;LSC=1;break;
				case(7):LSA=1;LSB=1;LSC=1;break;
			}
		IO_dig=display[i];
		delay(100);
		IO_dig=0x00;

	}
}