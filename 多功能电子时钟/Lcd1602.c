
#define  _LCD1602_C
#include "config.h"			//
#include "Lcd1602.h"

bit rADDR0; 
bit rADDR1;  

/* 暂停LED动态扫描，暂存*/
void LedScanPause()
{
    ENLED = 1;
    rADDR0 = ADDR0;
    rADDR1 = ADDR1;
}
/* 恢复动态扫描，恢复相关引脚的值 */
void LedScanContinue()
{
    ADDR0 = rADDR0;
    ADDR1 = rADDR1;
    ENLED = 0;
}
/* 等待液晶准备好 */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}
/* 液晶写入命令*/
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
/* 写入数据 */
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
/* 清屏 */
void LcdClearScreen()
{
	LcdWriteCmd(0x01);
}

void LcdOpenCursor()	//光标的闪烁效果 
{
	LcdWriteCmd(0x0F);
}

void LcdCloseCursor()
{
	LcdWriteCmd(0x0C);
}
/* 光标位置 */
void LcdSetCursor(uint8 x, uint8 y)
{
    uint8 addr;											   //设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 
    
    if (y == 0)  
        addr = 0x00 + x;  
    else
        addr = 0x40 + x;  
    LcdWriteCmd(addr | 0x80);  
}
/* 在液晶上显示字符串 */
void LcdShowStr(uint8 x, uint8 y, uint8 *str)
{
    LcdSetCursor(x, y);  //起始addr
    while (*str != '\0')  //连续写入字符串数据
    {
        LcdWriteDat(*str++);
    }
}
/* 显示一个字符 */
void LcdShowChar(uint8 x, uint8 y, uint8 chr)
{
    LcdSetCursor(x, y);  //起始地址
    LcdWriteDat(chr);   
}
/* 初始化液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38); 
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);     //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}
