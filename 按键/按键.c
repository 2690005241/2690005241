#include <STC89C5xRC.H>
sbit ADDR0=P2^0;
sbit ADDR1=P2^1;
sbit ADDR2=P2^2;
unsigned char code LedChar[]={
0xC0,0xF9,0xA4,0xB0,0x99,0x92,
0x82,0xF8,0x80,0x90,0x88,0x83,
0xC6,0xA1,0x86,0x8E};				//数码管显示字符             
bit keysta=1;		   				//当前按键状态
        void main()
{
        bit backup=1;		        //按键值备份
        unsigned char cnt=0;		//计数		  
        
        TMOD=0x01;
        TR0=1;					    //定时器
        TH0=0xF5;
        TL0=0x32;
        EA=1; ET0=1;
        P11=0;
        ADDR0=1;
        ADDR1=0;
        ADDR2=1;
        P0=LedChar[cnt];
        
        P27=0;							   
        P23=1;
        
        while(1)
        {
                if(keysta!=backup)
                {
                        backup=keysta;
                        if(backup==0)
                        {
                                cnt++;
                                if(cnt>9)
                                {
                                        cnt=0;
                                }
                                P0=LedChar[cnt];
                        }
                }
        }
}
/******中断函数***********/
void interrupttime() interrupt 1
{
        static unsigned char compare=0xFF;
        TH0=0xF5;//1ms
        TL0=0x32;
        compare=(compare<<1)|P23;
        if(compare==0xFF)
        {
                keysta=1;
        }
        else if(compare==0x00)
        {
                keysta=0;
        }
        else
   {}
}