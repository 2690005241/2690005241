#include "STC89C5xRC.H"
#include "smg.h"
#include "chuli.h"
#include "i2c.h"
typedef unsigned int u16;
typedef	unsigned char u8;
void main()
{
	if(at24c02read(1)==0)
	{at24c03write(1,12);	//��ʼ����
	}
	while(1)
	{
	codelock();				  //������
	digdisplay();		  //�������ʾ����

	}
}