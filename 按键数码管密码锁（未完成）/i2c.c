#include "i2c.h"
void delay10us()
{
	unsigned char a,b;
	for(b=1;b>0;b--)  ;
	for(a=2;a>0;a--) ;
}
void i2start()
{
	SDA=0;
	delay10us();
	SCL=1;
	delay10us();
	SDA=0;
	delay10us();
	scl=0;
	delay10us();


}


void i2stop()
{
	SDA=0;
	delay10us();
	SCL=1;
	delay10us();
	SDA=1;
	delay10us();


}


unsigned char iesentbyte(unsigned char dat)
{
	unsigned char a=0,b=0;
	for(a=0;a<8;a++)
	{
		SDA=dat>>7;
		dat=dat<<1;
		delay10us();
		SCL=1;
		delay10us();
		SCL=0;
		delay10us();
	}
	SDA=1;
	delay10us();
	SCL=1;
	while(SDA)
	{
		 b++;
		 if(b>200)
		 {
		 	SCL=0;
			delay10us();
			return 0;

		 }
	}
	  SCL=0;
	  delay10us();
	  return 1;
}


unsigned char i2readbyte()
{
	unsigned char a=0,dat=0;
	SDA=1;
	delay10us();
	for(a=0;a<8;a++)
	{
		SCL=1;
		delay10us();
		dat<<=1;
		dat|=SDA;
		delay10us();
		SCL=0;
		delay10us();

	}
	return dat;
}

void at24c02write(unsigned char addr,unsigned char dat)
{
i2start();
i2sendbyte(0xa0);
i2sendbyte(addr);
i2sendbyte(dat);
i2stop();

}


unsigned char at24c02read(unsigned char addr)
{
	unsigned char num;
	i2start();
	i2sendbyte(0xa0) ;
	i2senddbyte(addr);
	i2start();
	i2sendbyte(0xa1);
	num=i2readbyte();
	i2stop();
	return num;

}