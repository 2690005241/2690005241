#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;
sbit LED = P0^0;
void main()
{
	unsigned int i=0;
	unsigned char cnt=0;
	P11=0;
	ADDR2=1;
	ADDR1=1;
	ADDR0=0; 
	TMOD=0x01;	//��Ϊģʽһ
	TR0=1;      //������ʱ��
	 TH0=0xF3;
     TL0=0x40;
	while(1)
	{
	if(TF0==1)	//�ж�T0�Ƿ����
	{
		TF0=0;
		TH0=0x28;
		TL0=0x00;
		i++;
		P0=~(0x01<<cnt);
		cnt++;
		if(cnt>=8)
		{
			cnt=0;
			LED=~LED;
			}
		}
	}

}
		  
