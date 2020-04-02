///*/*实时时钟模块
//  /* 发送一个字节到DS1302通信总线上 */
//void BWDS1302(uint8 dat)
// /* 由DS1302通信总线上读取一个字节 */
//uint8 BRDS1302()
///* 用单次写操作向某一寄存器写入一个字节，reg-寄存器地址，dat-待写入字节 */
//void SWDS1302(uint8 reg, uint8 dat)
///* 用单次读操作从某一寄存器读取一个字节，reg-寄存器地址，返回值-读到的字节 */
//uint8 SRDS1302(uint8 reg)
// /* 用突发模式连续写入8个寄存器数据，dat-待写入数据指针 */
//void BURSTWDS1302(uint8 *dat)
// /* 用突发模式连续读取8个寄存器的数据，dat-读取数据的接收指针 */
//void BURSTRDS1302(uint8 *dat)
 ///* 获取实时时间，即读取DS1302当前时间并转换为时间结构体格式 */
//void GetTime(struct sTime *time)
///* 设定实时时间，时间结构体格式的设定时间转换为数组并写入DS1302 */
//void SetTime(struct sTime *time)
///* DS1302初始化，如发生掉电则重新设置初始时间 */
//void InitDS1302()	*/							*/

#define  _DS1302_C
#include "config.h"
#include "DS1302.h"

/* 发送一个字节到DS1302 */
void BWDS1302(uint8 dat)
{
    uint8 m;
    
    for (m=0x01; m!=0; m<<=1)  //Low position in front, displacement out
    {
        if ((m&dat) != 0) 
            DS1302_IO = 1;
        else
            DS1302_IO = 0;
        DS1302_CK = 1;       
        DS1302_CK = 0;       //Pull the clock higher and lower to complete a bit operation
    }
    DS1302_IO = 1;           //Make sure to release IO pins
}


/* 由DS1302上读取一个字节 */
uint8 BRDS1302()
{
    uint8 m;
    uint8 dat = 0;
    
    for (m=0x01; m!=0; m<<=1)  
    {
        if (DS1302_IO != 0)  
        {
            dat |= m;
        }
        DS1302_CK = 1;       
        DS1302_CK = 0;       
    }
    return dat;              //Returns the bytes of data read
}


/* 用单次写操作向某一寄存器写入一个字节， */
void SWDS1302(uint8 reg, uint8 dat)			
{
    DS1302_CE = 1;                   
    BWDS1302((reg<<1)|0x80);  //write instruction
    BWDS1302(dat);            //write in
    DS1302_CE = 0;                
}


/* 单次读操作*/
uint8 SRDS1302(uint8 reg)
{
    uint8 dat;
    
    DS1302_CE = 1;                   
    BWDS1302((reg<<1)|0x81);  //Read register instruction
    dat = BRDS1302();          
    DS1302_CE = 0;                 
    
    return dat;
}


/* 用突发模式连续写入8个寄存器数据 */
void BURSTWDS1302(uint8 *dat)
{
    uint8 i;
    
    DS1302_CE = 1;
    BWDS1302(0xBE);          //Burst write instruction
    for (i=0; i<8; i++)     //Write 8 bytes of data in a row
    {
        BWDS1302(dat[i]);
    }								
    DS1302_CE = 0;
}
/* 用突发模式连续读取8个寄存器的数据 */
void BURSTRDS1302(uint8 *dat)
{
    uint8 i;
    
    DS1302_CE = 1;
    BWDS1302(0xBF);  //Burst read register instruction
    for (i=0; i<8; i++)     //Read 8 bytes in a row
    {
        dat[i] = BRDS1302();
    }
    DS1302_CE = 0;
}
/* 获取实时时间 */
void GetTime(struct sTime *time)
{
    uint8 buf[8];
    
    BURSTRDS1302(buf);
    time->year = buf[6] + 0x2000;
    time->mon  = buf[4];
    time->day  = buf[3];
    time->hour = buf[2];
    time->min  = buf[1];
    time->sec  = buf[0];
    time->week = buf[5];
}
/* 设定实时时间， */
void SetTime(struct sTime *time)
{
    uint8 buf[8];
    
    buf[7] = 0;
    buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->mon;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->min;
    buf[0] = time->sec;
    BURSTWDS1302(buf);
}
/* DS1302初始化 */
void InitDS1302()
{
    uint8 dat;
    struct sTime code InitTime[] = {  					   //tacitly approve
        0x2020,0x04,0x03, 0x23,0x59,0x59, 0x03
    };
    
    DS1302_CE = 0;  //  initialization
    DS1302_CK = 0;
    dat = SRDS1302(0);  
    if ((dat & 0x80) != 0)       //Determine whether DS1302 has been stopped
    {
        SWDS1302(7, 0x00);  
        SetTime(&InitTime);      // set DS1302 as the default initial time
    }
}
