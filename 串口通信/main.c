#include <STC12C5A60S2.H>

void ConfigUART(unsigned int baud);

void main()
{
    EA = 1;  
    ConfigUART(9600);     //���ò�����Ϊ9600
    while(1);
}
void ConfigUART(unsigned int baud)
{
    SCON  = 0x50;          //���ô���Ϊģʽ1
    TMOD &= 0x0F; 
    TMOD |= 0x20;          //����T1Ϊģʽ2
    TH1 = 256 - (33177600/12/32)/baud; 
    TL1 = TH1;     //��ֵ��������ֵ
    ET1 = 0;       
    ES  = 1;       
    TR1 = 1;       
}

void InterruptUART() interrupt 4
{
	if(RI)
	{
		RI = 0;
		SBUF = SBUF + 1;		  //�Է��͵����ݼ�һ�󷵻�
	}
	if(TI)
	{
		TI = 0;
	}
}
