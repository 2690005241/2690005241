#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;
unsigned char code LedChar[] = {
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
void main()
{
	unsigned char cnt = 0;	//记录T0中断次数
	unsigned char sec = 0;  //经过秒数
	ADDR2=0;
	ADDR1=0;
	ADDR0=0;
	P11=0;
	TMOD=0x01;	//设为模式一
	TH0=0x28;
 	TL0=0x00;   //定时20ms
	TR0=1;      //启动定时器
	while(1)
	{
		if(TF0==1)		 //判断T0是否溢出
		{				 
			TF0=0;		 //T0溢出后清零标志
			TH0=0x28;
			TL0=0x00;
			cnt++;
			if(	cnt>=50) //判断溢出是否达到50次
			{
				cnt=0;
				P0=LedChar[sec];
				sec++;
				if(sec>=10)
				{
					sec=0;
				}
			}
		}
	}

}
