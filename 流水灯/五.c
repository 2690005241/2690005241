#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;
void main()
{
	unsigned int i=0;
	unsigned char cnt=0;
	P11=0;						   
	ADDR2=1;			   
	ADDR1=1;
	ADDR0=0; 
	while(1)					   //Ö÷Ñ­»·
		{
		P0=~(0x01<<cnt);
		for(i=0;i<60000;i++);
		cnt++;
		if(cnt>=8)
		{
		cnt=0;
		}
	}

}