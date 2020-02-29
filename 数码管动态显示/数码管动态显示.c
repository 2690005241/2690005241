#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;
unsigned char code LedChar[] = {
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
unsigned char LedBuff [6]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
unsigned char i = 0;
unsigned int cnt = 0;
unsigned char flag1s = 0;
void main()
{
	unsigned long sec = 0;
	EA = 1 ;
	P11=0;
	TMOD=0x01;	//设为模式一

		TH0 = 0xF3;
		TL0 = 0x40;		   //设置1ms

	ET0=1;	  
	TR0=1;      //启动定时器
	while(1)
	{
	 	if(flag1s==1)
		{
			flag1s=0;
			sec++;					
			LedBuff[5] = LedChar[sec%10];
			LedBuff[4] = LedChar[sec/10%10];
			LedBuff[3] = LedChar[sec/100%10];
			LedBuff[2] = LedChar[sec/1000%10];
			LedBuff[1] = LedChar[sec/10000%10];
			LedBuff[0] = LedChar[sec/100000%10];
		}
		
	}

}
   /*****************定时器中断函数*********************/
   void InterruptTimer0() interrupt 1
   {
	   TH0 = 0xF3; 
	   TL0 = 0x40;		   //设置定时初值
		
	    cnt++;
	    if(cnt>=1000)
	   {
		   cnt=0;
		   flag1s =1 ;
	   }
/*********************动态刷新扫描****************/
		P2=0xFF;
		switch(i)
		{
			case 0: ADDR2=0;ADDR1=0;ADDR0=0;i++;P0=LedBuff[0];break;
			case 1: ADDR2=0;ADDR1=0;ADDR0=1;i++;P0=LedBuff[1];break;
			case 2: ADDR2=0;ADDR1=1;ADDR0=0;i++;P0=LedBuff[2];break;
			case 3: ADDR2=0;ADDR1=1;ADDR0=1;i++;P0=LedBuff[3];break;
			case 4: ADDR2=1;ADDR1=0;ADDR0=0;i++;P0=LedBuff[4];break;
			case 5: ADDR2=1;ADDR1=0;ADDR0=1;i=0;P0=LedBuff[5];break;
			default:break;			
		}
   }