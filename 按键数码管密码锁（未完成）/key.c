#include "key.h"

typened unsigned int u16;
typened unsigned char char u8;

unsigned char keyvalue;
undigned char flog;

void delay(u16 i)
{
	
}
void keydown(void)
{
	char a=0;
	IO_key=0x0f;
	if(IO_key!=0x0f)
	{
		IO_key=0x0f;
		flog=1;
		switch(IO_key)
		{
			case(0x07):keyvalue=0;break;
			case(0x0b):keyvalue=1;break;
			case(0x0d):keyvalue=2;break;
			case(0x0e):keyvalue=3;break;

		}
		while((a<50)&&(IO_key!=0xf0))
		{
			delay(1000);
			a++
		}

	}

}