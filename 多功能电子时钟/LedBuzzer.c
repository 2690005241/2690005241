

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

uint8 code LedChar[] = {  
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

bit staBuzzer = 0; 
struct sLedBuff ledBuff; //LED显示缓冲区

/* LED初始化函数 */
void InitLed()
{
    
    P0 = 0xFF;
    ENLED = 0;
    
    T2CON = 0x00;  //配置T2工作在16位自动重载定时器模式
    RCAP2H = ((65536-SYS_MCLK/1500)>>8);  //配置重载值，每秒产生1500次中断，
    RCAP2L = (65536-SYS_MCLK/1500);       
    TH2 = RCAP2H; 
    TL2 = RCAP2L;
    ET2 = 1;       
    PT2 = 1;       						 //高优先级
    TR2 = 1;       
}


/* 数码管上显示一位数字 */
void ShowLedNumber(uint8 index, uint8 num, uint8 point)
{
    ledBuff.number[index] = LedChar[num];  //输入数字转换为数码管字符0～F
    if (point != 0)
    {
        ledBuff.number[index] &= 0x7F;  //point不为0时点亮当前位的小数点
    }
}



/* T2中断 */
void InterruptTimer2() interrupt 5
{
    static uint8 i = 0;  //LED位选索引
    
    TF2 = 0;  
   
    if (ENLED == 0)  
    {
        P0 = 0xFF;                       //xiaoying
        P1 = (P1 & 0xF0) | i;            
        P0 = *((uint8 data*)&ledBuff+i);
        if (i < (sizeof(ledBuff)-1))     
            i++;
        else
            i = 0;
    }

    if (staBuzzer == 1)
        BUZZER = ~BUZZER;  	   //on
    else
        BUZZER = 1;       //off
}
