#include <STC89C5xRC.H>
sbit PIN_RxD = P3^0;			//接收引脚
sbit PIN_TxD = P3^1;			//发射引脚
bit RxdEnd = 0;					//状态显示
bit RxdOrTxd = 0;				//接受结束
bit TxdEnd = 0;					//发射结束
unsigned char RxdBuf = 0;
unsigned char TxdBuf = 0;	    //缓冲器

void ConfigUART(unsigned int baud);
void StartRxD();				//函数声明
void StartTxD(unsigned char dat);

void main()
{
	EA = 1;					    //开启总中断
	ConfigUART(9600);		    //设置波特率
	while(1)
	{
		while(PIN_RxD);		    //低电平接受启动，数据加一后发送回去
		StartRxD();
		while(!RxdEnd);
		StartTxD(RxdBuf+1);
		while(!TxdEnd);
	}
	
}
/**********串口配置函数*********/
void ConfigUART(unsigned int baud)
{
	TMOD &= 0xF0;
	TMOD |= 0x02;				     //配置定时器为模式二，计算T0重载值
	TH0 = 256 - (331772/12)/baud;
}
/***********串行接收启动**********/
void StartRxD()
{
	TL0 = 256 - ((256 - TH0)>>1);    //接受启动时的T0定时为半个波特率周期
	ET0 = 1;
	TR0 = 1;
	RxdEnd = 0;				    	 //清零接收结束
	RxdOrTxd = 0;					 //设置当前状态为接收
}
/*********串行发送********/
void  StartTxD(unsigned char dat)
{
	TxdBuf = dat;					  //待发送数据保存到发送缓冲器
	TL0 = TH0;
	ET0 = 1;
	TR0 = 1;
	PIN_TxD = 0;
	TxdEnd = 0;
	RxdOrTxd = 1;
}
/*************T0中断服务函数*********/

void InterruptTimer0() interrupt 1
{
	static unsigned char cnt = 0;	  //计数

	if(RxdOrTxd)					  //串行发送处理
	{
		cnt++;
		if(cnt <= 8)
		{
			PIN_TxD = TxdBuf & 0x01;
			TxdBuf >>= 1;
		}
		else if(cnt == 9)			  //发送停止
		{
		 	PIN_TxD = 1;
		}
		else
		{
			cnt = 0;
			TR0 = 0;
			TxdEnd = 1;			      //置发送结束标志
		}
	}
	else							  //串行接收处理
	{
		if(cnt == 0)
		{
			if(!PIN_RxD)			  //起始位为零，清零接收器缓冲器，准备接收数据位
			{
				RxdBuf = 0;
				cnt++;
			}
			else				       //起始位不为零，终止接收
			{
				TR0 = 0;
			}
		}
		else if(cnt <= 8)
		{
			RxdBuf >>= 1;	     	   //低位在先，右移
			if(PIN_RxD)				   //接收脚为一 ，缓冲器最高位致一
			{					       //接收位为零，不处理
				RxdBuf |= 0x80;
			}
			cnt++;
		}
		else						    //停止处理
		{
			cnt = 0;				    //关闭bit
			TR0 = 0;
			if(PIN_RxD)
			{
				RxdEnd = 1;		        //停止位为一，数据有效，
			}
		}
	}
}
