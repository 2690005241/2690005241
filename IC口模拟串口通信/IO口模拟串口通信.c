#include <STC89C5xRC.H>
sbit PIN_RxD = P3^0;			//��������
sbit PIN_TxD = P3^1;			//��������
bit RxdEnd = 0;					//״̬��ʾ
bit RxdOrTxd = 0;				//���ܽ���
bit TxdEnd = 0;					//�������
unsigned char RxdBuf = 0;
unsigned char TxdBuf = 0;	    //������

void ConfigUART(unsigned int baud);
void StartRxD();				//��������
void StartTxD(unsigned char dat);

void main()
{
	EA = 1;					    //�������ж�
	ConfigUART(9600);		    //���ò�����
	while(1)
	{
		while(PIN_RxD);		    //�͵�ƽ�������������ݼ�һ���ͻ�ȥ
		StartRxD();
		while(!RxdEnd);
		StartTxD(RxdBuf+1);
		while(!TxdEnd);
	}
	
}
/**********�������ú���*********/
void ConfigUART(unsigned int baud)
{
	TMOD &= 0xF0;
	TMOD |= 0x02;				     //���ö�ʱ��Ϊģʽ��������T0����ֵ
	TH0 = 256 - (331772/12)/baud;
}
/***********���н�������**********/
void StartRxD()
{
	TL0 = 256 - ((256 - TH0)>>1);    //��������ʱ��T0��ʱΪ�������������
	ET0 = 1;
	TR0 = 1;
	RxdEnd = 0;				    	 //������ս���
	RxdOrTxd = 0;					 //���õ�ǰ״̬Ϊ����
}
/*********���з���********/
void  StartTxD(unsigned char dat)
{
	TxdBuf = dat;					  //���������ݱ��浽���ͻ�����
	TL0 = TH0;
	ET0 = 1;
	TR0 = 1;
	PIN_TxD = 0;
	TxdEnd = 0;
	RxdOrTxd = 1;
}
/*************T0�жϷ�����*********/

void InterruptTimer0() interrupt 1
{
	static unsigned char cnt = 0;	  //����

	if(RxdOrTxd)					  //���з��ʹ���
	{
		cnt++;
		if(cnt <= 8)
		{
			PIN_TxD = TxdBuf & 0x01;
			TxdBuf >>= 1;
		}
		else if(cnt == 9)			  //����ֹͣ
		{
		 	PIN_TxD = 1;
		}
		else
		{
			cnt = 0;
			TR0 = 0;
			TxdEnd = 1;			      //�÷��ͽ�����־
		}
	}
	else							  //���н��մ���
	{
		if(cnt == 0)
		{
			if(!PIN_RxD)			  //��ʼλΪ�㣬�����������������׼����������λ
			{
				RxdBuf = 0;
				cnt++;
			}
			else				       //��ʼλ��Ϊ�㣬��ֹ����
			{
				TR0 = 0;
			}
		}
		else if(cnt <= 8)
		{
			RxdBuf >>= 1;	     	   //��λ���ȣ�����
			if(PIN_RxD)				   //���ս�Ϊһ �����������λ��һ
			{					       //����λΪ�㣬������
				RxdBuf |= 0x80;
			}
			cnt++;
		}
		else						    //ֹͣ����
		{
			cnt = 0;				    //�ر�bit
			TR0 = 0;
			if(PIN_RxD)
			{
				RxdEnd = 1;		        //ֹͣλΪһ��������Ч��
			}
		}
	}
}
