#include <STC89C5xRC.H>
unsigned long beats=0;
void StartMotor(unsigned long angle);	 //函数的声明
void main()
{
	EA = 1;			   					 //总中断
	TMOD = 0x01;
	TL0=0x00;     						 //定时20ms
	TH0=0x28;		
	ET0 = 1;
	TR0 =1;					  			 //启动定时器

	StartMotor(360*2 + 180);  			 //使电机转动两圈半
	while(1);
}
/*********电机启动函数********/
void StartMotor(unsigned long angle)
{
	EA = 0;
	beats = (angle*4076)/360;
	EA = 1;
}
 /**********T0中断函数********/
void InterruptTimer0() interrupt 1
{
	unsigned char tmp;									//部分变量
	static unsigned char index = 0;
	unsigned char code BeatCode[8] = {
	0x0E, 0x0C, 0x0D, 0x09, 0x0B, 0x03, 0x07, 0x06};	//控制节拍
	TL0=0x00;   //定时20ms
	TH0=0x28;
	if(beats != 0)					 					 //不为零则产生驱动节拍
	{
		tmp = P1;
		tmp = tmp & 0xF0;
		tmp = tmp | BeatCode[index];			 
		P1 = tmp;
		index++;
		index = index & 0x07;
		beats--;
	}
	else					 					        //为零则关闭所有相
	{
		P1 = P1 | 0x0F;
	}

}