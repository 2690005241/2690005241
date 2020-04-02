

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

uint8 code LedChar[] = {  
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

bit staBuzzer = 0; 
struct sLedBuff ledBuff; //LED��ʾ������

/* LED��ʼ������ */
void InitLed()
{
    
    P0 = 0xFF;
    ENLED = 0;
    
    T2CON = 0x00;  //����T2������16λ�Զ����ض�ʱ��ģʽ
    RCAP2H = ((65536-SYS_MCLK/1500)>>8);  //��������ֵ��ÿ�����1500���жϣ�
    RCAP2L = (65536-SYS_MCLK/1500);       
    TH2 = RCAP2H; 
    TL2 = RCAP2L;
    ET2 = 1;       
    PT2 = 1;       						 //�����ȼ�
    TR2 = 1;       
}


/* ���������ʾһλ���� */
void ShowLedNumber(uint8 index, uint8 num, uint8 point)
{
    ledBuff.number[index] = LedChar[num];  //��������ת��Ϊ������ַ�0��F
    if (point != 0)
    {
        ledBuff.number[index] &= 0x7F;  //point��Ϊ0ʱ������ǰλ��С����
    }
}



/* T2�ж� */
void InterruptTimer2() interrupt 5
{
    static uint8 i = 0;  //LEDλѡ����
    
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
