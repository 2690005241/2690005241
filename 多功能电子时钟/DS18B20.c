///*/*温度传感器
//包含：
///* 软件延时，(temp*10)us */void Delay10x(uint8 t)
///* 复位总线，获取存在脉冲，以启动一次读写操作 */bit Get18B20Ack()
///* 向DS18B20写入一个字节，dat-待写入字节 */void W18B20(uint8 dat)
///* 从DS18B20读取一个字节*/uint8 R18B20()
///* 启动一次18B20温度转换， */bit Operat18B20()
///* 读取DS18B20转换的温度值，返回值-表示是否读取成功 */bit Get18B20T(int16 *t)	 



#define  _DS18B20_C
#include "config.h"						
#include "DS18B20.h"

/* 软件延时，(t*10)us */
void Delay10x(uint8 t)	  //unit表示8位有符号整数
{
    do {
        _nop_();		  //每个nop延时10um
        _nop_();			  //software delay
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--t);			   //t决定延时T*10um
}


/* 复位总线，获取存在脉冲，以启动一次读写操作 */
bit Get18B20Ack()
{
    bit ack;
    
    EA = 0;          
    IO_18B20 = 0;     //500us复位脉冲
    Delay10x(50);		
    IO_18B20 = 1;
    Delay10x (6);    
    ack = IO_18B20;     
    while(!IO_18B20);             //脉冲结束，非零
    EA = 1;                  
    
    return ack;
}


/* 向DS18B20写入一个字节，dat-待写入字节 */
void W18B20(uint8 dat)	 
{
    uint8 mask;
    
    EA = 0; 
    for (mask=0x01; mask!=0; mask<<=1) 
    {
        IO_18B20 = 0;         //产生2us低电平
        Delay10x(2);
        if ((mask&dat) == 0)  
            IO_18B20 = 0;
        else
            IO_18B20 = 1;
        Delay10x(6);        //延时60us
        IO_18B20 = 1;         //拉高
    }
    EA = 1;
}


/* 从DS18B20读取一个字节*/
uint8 R18B20()
{
    uint8 dat;
    uint8 mask;
    
    EA = 0;                         
    for (mask=0x01; mask!=0; mask<<=1)  //采集底八位
    {
        IO_18B20 = 0;         //产生2us的低电平
        Delay10x(2);
        IO_18B20 = 1;         //延时两毫秒，等待18B20输出数据
         Delay10x(2);
        if (!IO_18B20)        
            dat &= ~mask;
        else
            dat |= mask;
        Delay10x(6);       
    }
    EA = 1;                  

    return dat;							  
}


/* 启动一次18B20温度转换， */
bit Operat18B20()						
{
    bit ack;
    
    ack = Get18B20Ack();   
    if (ack == 0)          //如18B20正确应答
    {
        W18B20(0xCC);  //跳过ROM
        W18B20(0x44);  //温度转换
    }
    return ~ack;   //ack=0表示操作成功，返回值取反返回值表示是否启动成功
}

				  
/* 读取DS18B20转换的温度值 */
bit Get18B20T(int16 *t)	   //指针t
{
    bit ack;
    uint8 LSB, MSB; //16bit温度值的低字节和高字节
    
    ack = Get18B20Ack();    
    if (ack == 0)           //正确应答
    {
        W18B20(0xCC);           //跳过ROM
        W18B20(0xBE);            
        LSB = R18B20();           //低字节
        MSB = R18B20();             //高字节
        *t = ((int16)MSB << 8) + LSB;  
    }
	return ~ack;  
}
