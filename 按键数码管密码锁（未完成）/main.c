#include "STC89C5xRC.H"
#include "smg.h"
#include "chuli.h"
#include "i2c.h"
typedef unsigned int u16;
typedef	unsigned char u8;
void main()
{
	if(at24c02read(1)==0)
	{at24c03write(1,12);	//初始密码
	}
	while(1)
	{
	codelock();				  //密码锁
	digdisplay();		  //数码管显示函数

	}
}