
#define  _MAIN_C
#include "config.h"
#include "Lcd1602.h"
#include "LedBuzzer.h"
#include "keyboard.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "Infrared.h"
#include "Time.h"
#include "main.h"

bit flag2s = 0;    
bit flag200ms = 0; 
uint8 T0RH = 0;   
uint8 T0RL = 0;    
enum eStaSystem staSystem = E_NORMAL;  

void main()
{
    EA = 1;           
    ConfigTimer0(1);  
    InitLed();                  //初始化LED模块
    InitDS1302();                      //初始化实时时钟模块
    InitInfrared();                         //初始化红外接收模块
    InitLcd1602();                  //初始化液晶模块
    Start18B20();                   //启动温度转换
    
    while (!flag2s);  
    flag2s = 0;
    RefreshTime();    
    RefreshDate(1);   
    RefreshTemp(1);   
    RefreshAlarm();  
    
    while (1) 
    {
        KeyDriver();      //按键驱动
        InfraredDriver(); //红外接收驱动
        if (flag200ms)    
        {
            flag200ms = 0;
            RefreshTime();  
            AlarmMonitor(); 
            if (staSystem == E_NORMAL)  //正常运行时刷新日期显示
            {
                RefreshDate(0);
            }
        }
        if (flag2s) 
        {
            flag2s = 0;
            if (staSystem == E_NORMAL)  
            {
                RefreshTemp(0);
            }
        }
    }
}

/* 温度刷新函数 */
void RefreshTemp(uint8 ops)
{
    int16 t;
    uint8 pdata str[8];
    static int16 backup = 0;
    
    Get18B20T(&t); //获取当前温度值
    Start18B20();       
    t >>= 4;          //舍弃小数位
    if ((backup!=t) || (ops!=0)) 
    {
        str[0] = (t/10) + '0';  //转为ASCII码
        str[1] = (t%10) + '0'; 
        str[2] = '\'';             
        str[3] = 'C';
        str[4] = '\0';             //结束符
        LcdShowStr(12, 0, str);    
        backup = t;             
    }
}
/* T0 */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; 
    tmp = 65536 - tmp;       
    tmp = tmp + 34;           //补偿 
    T0RH = (uint8)(tmp>>8);  
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;  
    TMOD |= 0x01;  
    TH0 = T0RH;    
    TL0 = T0RL;
    ET0 = 1;        
    TR0 = 1;        
}
/* T0中断 */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr2s = 0;
    static uint8 tmr200ms = 0;
    
    TH0 = T0RH;  
    TL0 = T0RL;
    tmr200ms++;  
    if (tmr200ms >= 200)
    {
        tmr200ms = 0;
        flag200ms = 1;
        tmr2s++;  
        if (tmr2s >= 10)
        {
            tmr2s = 0;
            flag2s = 1;
        }
    }
    KeyScan();   //执行按键扫描
}
