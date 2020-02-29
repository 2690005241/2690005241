#include <STC89C5xRC.H>
unsigned long beats=0;
void StartMotor(unsigned long angle);	 //����������
void main()
{
	EA = 1;			   					 //���ж�
	TMOD = 0x01;
	TL0=0x00;     						 //��ʱ20ms
	TH0=0x28;		
	ET0 = 1;
	TR0 =1;					  			 //������ʱ��

	StartMotor(360*2 + 180);  			 //ʹ���ת����Ȧ��
	while(1);
}
/*********�����������********/
void StartMotor(unsigned long angle)
{
	EA = 0;
	beats = (angle*4076)/360;
	EA = 1;
}
 /**********T0�жϺ���********/
void InterruptTimer0() interrupt 1
{
	unsigned char tmp;									//���ֱ���
	static unsigned char index = 0;
	unsigned char code BeatCode[8] = {
	0x0E, 0x0C, 0x0D, 0x09, 0x0B, 0x03, 0x07, 0x06};	//���ƽ���
	TL0=0x00;   //��ʱ20ms
	TH0=0x28;
	if(beats != 0)					 					 //��Ϊ���������������
	{
		tmp = P1;
		tmp = tmp & 0xF0;
		tmp = tmp | BeatCode[index];			 
		P1 = tmp;
		index++;
		index = index & 0x07;
		beats--;
	}
	else					 					        //Ϊ����ر�������
	{
		P1 = P1 | 0x0F;
	}

}