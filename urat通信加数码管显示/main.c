#include <STC12C5A60S2.H>

sbit ENLED = P1^1;
unsigned char code LedChar[] = {  
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[7] = {  
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
unsigned char T0RH = 0; 
unsigned char T0RL = 0; 
unsigned char RxdByte = 0;  //串口接收到的字节

void ConfigTimer0(unsigned int ms);
void ConfigUART(unsigned int baud);

void main()
{
    EA = 1;       
    ENLED = 0;   
  
    ConfigTimer0(1);   //配置T0定时1ms
    ConfigUART(9600);  
    
    while (1)
    {   												 //转换成十六进制显示出来
        LedBuff[1] = LedChar[RxdByte & 0x0F];
        LedBuff[0] = LedChar[RxdByte >> 4];				//在前两个数码管显示
    }
}


/* 配置并启动T0*/
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 33177600 / 12;     
    tmp = (tmp * ms) / 1000; 
    tmp = 65536 - tmp;        
    tmp = tmp + 18;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   
    TMOD |= 0x01;   
    TH0 = T0RH;    
    TL0 = T0RL;
    ET0 = 1;       
    TR0 = 1;        
}



/* 串口配置*/
void ConfigUART(unsigned int baud)
{
    SCON  = 0x50;  //模式1
    TMOD &= 0x0F;  
    TMOD |= 0x20;  //T1为模式2
    TH1 = 256 - (33177600/12/32)/baud;  //计算T1重载值
    TL1 = TH1;     
    ET1 = 0;       
    ES  = 1;       
    TR1 = 1;       
}


/* 按键扫描函数 */
void LedScan()
{
    static unsigned char i = 0;  
    
    P0 = 0xFF;             //消隐
    P2 = (P2 & 0xF8) | i;  
    P0 = LedBuff[i];       
    if (i < 6)            
        i++;
    else
        i = 0;
}


/* T0中断服务函数 */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;  
    TL0 = T0RL;
    LedScan();  
}


/* UART */
void InterruptUART() interrupt 4
{
    if (RI)  
    {
        RI = 0;  
        RxdByte = SBUF; 
        SBUF = RxdByte; 
                        
    }
    if (TI) 
    {
        TI = 0;  
    }
}
