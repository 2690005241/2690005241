
#define  _LCD1602_C
#include "config.h"			//
#include "Lcd1602.h"

bit rADDR0; 
bit rADDR1;  

/* ��ͣLED��̬ɨ�裬�ݴ�*/
void LedScanPause()
{
    ENLED = 1;
    rADDR0 = ADDR0;
    rADDR1 = ADDR1;
}
/* �ָ���̬ɨ�裬�ָ�������ŵ�ֵ */
void LedScanContinue()
{
    ADDR0 = rADDR0;
    ADDR1 = rADDR1;
    ENLED = 0;
}
/* �ȴ�Һ��׼���� */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //��ȡ״̬��
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}
/* Һ��д������*/
void LcdWriteCmd(uint8 cmd)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* д������ */
void LcdWriteDat(uint8 dat)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* ���� */
void LcdClearScreen()
{
	LcdWriteCmd(0x01);
}

void LcdOpenCursor()	//������˸Ч�� 
{
	LcdWriteCmd(0x0F);
}

void LcdCloseCursor()
{
	LcdWriteCmd(0x0C);
}
/* ���λ�� */
void LcdSetCursor(uint8 x, uint8 y)
{
    uint8 addr;											   //������ʾRAM��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� 
    
    if (y == 0)  
        addr = 0x00 + x;  
    else
        addr = 0x40 + x;  
    LcdWriteCmd(addr | 0x80);  
}
/* ��Һ������ʾ�ַ��� */
void LcdShowStr(uint8 x, uint8 y, uint8 *str)
{
    LcdSetCursor(x, y);  //��ʼaddr
    while (*str != '\0')  //����д���ַ�������
    {
        LcdWriteDat(*str++);
    }
}
/* ��ʾһ���ַ� */
void LcdShowChar(uint8 x, uint8 y, uint8 chr)
{
    LcdSetCursor(x, y);  //��ʼ��ַ
    LcdWriteDat(chr);   
}
/* ��ʼ��Һ�� */
void InitLcd1602()
{
    LcdWriteCmd(0x38); 
    LcdWriteCmd(0x0C);  //��ʾ���������ر�
    LcdWriteCmd(0x06);     //���ֲ�������ַ�Զ�+1
    LcdWriteCmd(0x01);  //����
}
