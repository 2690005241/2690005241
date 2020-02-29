#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;


void ShiftLeft(unsigned char *p);

void main()
{
    unsigned int  i;
    unsigned char buf = 0x01;
    
P11=0;						   
	ADDR2=1;			   
	ADDR1=1;
	ADDR0=0; 
    
    while (1)
    {
		P0 = ~buf;
		for(i=0; i<50000; i++);
		ShiftLeft(&buf);
		if(buf == 0)
		{
			buf = 0x01;
		}
        
    }
}

void ShiftLeft(unsigned char *p)
{
  	*p = *p << 1;  
}