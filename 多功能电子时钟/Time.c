
#define  _TIME_C
#include "config.h"
#include "DS1302.h"
#include "LedBuzzer.h"
#include "Lcd1602.h"
#include "Time.h"
#include "main.h"

bit staMute = 0;         //silence
uint8 AlarmHour = 0x00;  
uint8 AlarmMin  = 0x01;  
struct sTime CurTime;   

uint8 SetIndex = 0;     //Set bit index
uint8 pdata SetAlarmHour;    //����Сʱ�����û���
uint8 pdata SetAlarmMin;     //���ӷ��������û���
struct sTime pdata SetTimeBUF;  //����ʱ�����û�����

/* ��ȡ��ǰ����ʱ�䣬��ˢ��ʱ������ڵ���ʾ */
void RefreshTime()
{
    GetTime(&CurTime);                  //��ȡ��ǰ����ʱ��
    ShowLedNumber(5, CurTime.hour>>4, 0);   //ʱ
    ShowLedNumber(4, CurTime.hour&0xF,1);
    ShowLedNumber(3, CurTime.min>>4,  0);   //��
    ShowLedNumber(2, CurTime.min&0xF, 1);
    ShowLedNumber(1, CurTime.sec>>4,  0);   //��
    ShowLedNumber(0, CurTime.sec&0xF, 0);

}
/* ����ˢ�º�����ops-ˢ��ѡ�Ϊ0ʱֻ�����ڱ仯��ˢ�£���0������ˢ�� */
void RefreshDate(uint8 ops)
{
    uint8 pdata str[12];
    static uint8 backup = 0;
    
    if ((backup!=CurTime.day) || (ops!=0))
    {
        str[0] = ((CurTime.year>>12) & 0xF) + '0';  
        str[1] = ((CurTime.year>>8) & 0xF) + '0';
        str[2] = ((CurTime.year>>4) & 0xF) + '0';
        str[3] = (CurTime.year & 0xF) + '0';
        str[4] = '-';                        
        str[5] = (CurTime.mon >> 4) + '0';   //�·�
        str[6] = (CurTime.mon & 0xF) + '0';
        str[7] = '-';                        
        str[8] = (CurTime.day >> 4) + '0';   //����
        str[9] = (CurTime.day & 0xF) + '0';
        str[10] = '\0';         
        LcdShowStr(0, 0, str);  
        backup = CurTime.day;   
    }
}
/* ˢ������ʱ�����ʾ */
void RefreshAlarm()
{
    uint8 pdata str[8];
    
    LcdShowStr(0, 1, "Alarm at ");    
    str[0] = (AlarmHour >> 4) + '0';  
    str[1] = (AlarmHour & 0xF) + '0';
    str[2] = ':';                      
    str[3] = (AlarmMin >> 4) + '0';    
    str[4] = (AlarmMin & 0xF) + '0';
    str[5] = '\0';                     
    LcdShowStr(9, 1, str);             
}
/* ���Ӽ�غ������ִ��趨������ʱ��ʱִ������ */
void AlarmMonitor()
{
    if ((CurTime.hour==AlarmHour) && (CurTime.min==AlarmMin)) 
    {
        if (!staMute)                //����Ƿ���
            staBuzzer = ~staBuzzer;     //ʵ�ַ�������������
        else
            staBuzzer = 0;
    }
    else
    {
        staMute = 0;
        staBuzzer = 0;
    }
}
/* ������ʱ�估������ʾ��ʾ��Һ���� */
void ShowSetTime()
{
    uint8 pdata str[18];
    
    str[0]  = ((SetTimeBUF.year>>4) & 0xF) + '0';  //2λ�����
    str[1]  = (SetTimeBUF.year & 0xF) + '0';
    str[2]  = '-';
    str[3]  = (SetTimeBUF.mon >> 4) + '0';  
    str[4]  = (SetTimeBUF.mon & 0xF) + '0';
    str[5]  = '-';
    str[6]  = (SetTimeBUF.day >> 4) + '0';   
    str[7]  = (SetTimeBUF.day & 0xF) + '0';
    str[8]  = '-';
    str[9]  = (SetTimeBUF.week & 0xF) + '0'; 
    str[10] = ' ';
    str[11] = (SetTimeBUF.hour >> 4) + '0';  
    str[12] = (SetTimeBUF.hour & 0xF) + '0';
    str[13] = ':';
    str[14] = (SetTimeBUF.min >> 4) + '0';   
    str[15] = (SetTimeBUF.min & 0xF) + '0';
    str[16] = '\0';
    LcdShowStr(0, 0, "Set Date Time");  //��ʾ��ʾ����
    LcdShowStr(0, 1, str);             
}
/* ���������Ӽ�������ʾ��ʾ��Һ���� */
void ShowSetAlarm()
{
    uint8 pdata str[8];
    
    str[0] = (SetAlarmHour >> 4) + '0';   
    str[1] = (SetAlarmHour & 0xF) + '0';
    str[2] = ':';
    str[3] = (SetAlarmMin >> 4) + '0';    
    str[4] = (SetAlarmMin & 0xF) + '0';
    str[5] = '\0';
    LcdShowStr(0, 0, "Set Alarm");  
    LcdShowStr(0, 1, str);          
}
/* ȡ����ǰ���ã�������������״̬ */
void CancelCurSet()
{
    staSystem = E_NORMAL;
    LcdCloseCursor();  //�رչ��
    LcdClearScreen();  //Һ������
    RefreshTime();   
    RefreshDate(1);  
    RefreshTemp(1);  
    RefreshAlarm();  
}
/* ʱ�����������ʱzuoyi*/
void SetRightShift()
{
    if (staSystem == E_SET_TIME)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=1;  LcdSetCursor(1, 1); break;
            case 1: SetIndex=2;  LcdSetCursor(3, 1); break;
            case 2: SetIndex=3;  LcdSetCursor(4, 1); break;
            case 3: SetIndex=4;  LcdSetCursor(6, 1); break;
            case 4: SetIndex=5;  LcdSetCursor(7, 1); break;			  //����ʱ��������ͷ����
            case 5: SetIndex=6;  LcdSetCursor(9, 1); break;
            case 6: SetIndex=7;  LcdSetCursor(11,1); break;
            case 7: SetIndex=8;  LcdSetCursor(12,1); break;
            case 8: SetIndex=9;  LcdSetCursor(14,1); break;
            case 9: SetIndex=10; LcdSetCursor(15,1); break;
            default: SetIndex=0; LcdSetCursor(0, 1); break;
        }
    }
    else if (staSystem == E_SET_ALARM)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=1;  LcdSetCursor(1,1); break;
            case 1: SetIndex=2;  LcdSetCursor(3,1); break;
            case 2: SetIndex=3;  LcdSetCursor(4,1); break;
            default: SetIndex=0; LcdSetCursor(0,1); break;
        }
    }
}
/* ʱ�����������youyi*/
void SetLeftShift()
{															 //youyi
    if (staSystem == E_SET_TIME)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=10; LcdSetCursor(15,1); break;
            case 1: SetIndex=0;  LcdSetCursor(0, 1); break;
            case 2: SetIndex=1;  LcdSetCursor(1, 1); break;
            case 3: SetIndex=2;  LcdSetCursor(3, 1); break;
            case 4: SetIndex=3;  LcdSetCursor(4, 1); break;
            case 5: SetIndex=4;  LcdSetCursor(6, 1); break;
            case 6: SetIndex=5;  LcdSetCursor(7, 1); break;
            case 7: SetIndex=6;  LcdSetCursor(9, 1); break;
            case 8: SetIndex=7;  LcdSetCursor(11,1); break;
            case 9: SetIndex=8;  LcdSetCursor(12,1); break;
            default: SetIndex=9; LcdSetCursor(14,1); break;
        }
    }
    else if (staSystem == E_SET_ALARM)
    {
        switch (SetIndex)
        {
            case 0: SetIndex=3;  LcdSetCursor(4,1); break;
            case 1: SetIndex=0;  LcdSetCursor(0,1); break;
            case 2: SetIndex=1;  LcdSetCursor(1,1); break;
            default: SetIndex=2; LcdSetCursor(3,1); break;
        }
    }
}
/* ���������޸Ķ�Ӧ������λ */
void InputSetNumber(uint8 ascii)
{
    uint8 num;
    
    num = ascii - '0';
    if (num <= 9)  
    {
        if (staSystem == E_SET_TIME)
        {
            switch (SetIndex)
            {
                case 0: SetTimeBUF.year = (SetTimeBUF.year&0xFF0F)|(num<<4);
                        LcdShowChar(0, 1, ascii);  break;             
                case 1: SetTimeBUF.year = (SetTimeBUF.year&0xFFF0)|(num);
                        LcdShowChar(1, 1, ascii);  break;                
                case 2: SetTimeBUF.mon = (SetTimeBUF.mon&0x0F)|(num<<4);
                        LcdShowChar(3, 1, ascii);  break;                   
                case 3: SetTimeBUF.mon = (SetTimeBUF.mon&0xF0)|(num);
                        LcdShowChar(4, 1, ascii);  break;                      
                case 4: SetTimeBUF.day = (SetTimeBUF.day&0x0F)|(num<<4);
                        LcdShowChar(6, 1, ascii);  break;                  
                case 5: SetTimeBUF.day = (SetTimeBUF.day&0xF0)|(num);
                        LcdShowChar(7, 1, ascii);  break;                 
                case 6: SetTimeBUF.week = (SetTimeBUF.week&0xF0)|(num);
                        LcdShowChar(9, 1, ascii);  break;     
                case 7: SetTimeBUF.hour = (SetTimeBUF.hour&0x0F)|(num<<4);
                        LcdShowChar(11,1, ascii);  break;      
                case 8: SetTimeBUF.hour = (SetTimeBUF.hour&0xF0)|(num);
                        LcdShowChar(12,1, ascii);  break;      
                case 9: SetTimeBUF.min = (SetTimeBUF.min&0x0F)|(num<<4);
                        LcdShowChar(14,1, ascii);  break;      
                default:SetTimeBUF.min = (SetTimeBUF.min&0xF0)|(num);
                        LcdShowChar(15,1, ascii);  break;     
            }
            SetRightShift();                  //�Զ�����
        }
        else if (staSystem == E_SET_ALARM)
        {
            switch (SetIndex)
            {
                case 0: SetAlarmHour = (SetAlarmHour&0x0F) | (num<<4);
                        LcdShowChar(0,1, ascii); break;      
                case 1: SetAlarmHour = (SetAlarmHour&0xF0) | (num);
                        LcdShowChar(1,1, ascii); break;      
                case 2: SetAlarmMin = (SetAlarmMin&0x0F) | (num<<4);
                        LcdShowChar(3,1, ascii); break;     
                default:SetAlarmMin = (SetAlarmMin&0xF0) | (num);
                        LcdShowChar(4,1, ascii); break;      
            }
            SetRightShift();  
        }
    }
}
/* �л�ϵͳ����״̬ */
void SwitchSystemSta()
{
    if (staSystem == E_NORMAL)  
    {
        staSystem = E_SET_TIME;
        SetTimeBUF.year = CurTime.year;  
        SetTimeBUF.mon  = CurTime.mon;
        SetTimeBUF.day  = CurTime.day;
        SetTimeBUF.hour = CurTime.hour;
        SetTimeBUF.min  = CurTime.min;
        SetTimeBUF.sec  = CurTime.sec;
        SetTimeBUF.week = CurTime.week;
        LcdClearScreen();  
        ShowSetTime();     //��ʾ����ʱ��
        SetIndex = 255;    
        SetRightShift();
        LcdOpenCursor();   //�������
    }
    else if (staSystem == E_SET_TIME)  //ʱ�������л�����������
    {
        staSystem = E_SET_ALARM;
        SetTimeBUF.sec = 0;         
        SetTime(&SetTimeBUF);    
        SetAlarmHour = AlarmHour; 
        SetAlarmMin  = AlarmMin;
        LcdClearScreen();  
        ShowSetAlarm();    
        SetIndex = 255;    //�������������һ�𽫹����������ߵ�λ����
        SetRightShift();
    }
    else  
    {
        staSystem = E_NORMAL;
        AlarmHour = SetAlarmHour; 
        AlarmMin  = SetAlarmMin;
        LcdCloseCursor();  
        LcdClearScreen();  
        RefreshTime();   
        RefreshDate(1); 
        RefreshTemp(1);  
        RefreshAlarm();  
    }
}
/* ������������*/
void KeyAction(uint8 keycode)
{
    if  ((keycode>='0') && (keycode<='9'))  
    {
        InputSetNumber(keycode);
    }
    else if (keycode == 0x25)  
    {
        SetLeftShift();
    }
    else if (keycode == 0x27) 
    {
        SetRightShift();
    }
    else if (keycode == 0x0D)           //�س���
    {
        SwitchSystemSta();
    }
    else if (keycode == 0x1B)          //Esc��������/ȡ����ǰ����
    {
        if (staSystem == E_NORMAL) 
        {
            staMute = 1;
        }
        else                       
        {
            CancelCurSet();
        }
    }
}
