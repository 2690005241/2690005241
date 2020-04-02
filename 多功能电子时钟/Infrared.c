 /*红外接收





 
 */

#define  _INFRARED_C
#include "config.h"
#include "Infrared.h"
#include "Time.h"

const uint8 code IrCodeMap[][2] = {     //红外键码到标准PC键码的映射表
    {0x45,0x00}, {0x46,0x00}, {0x47,0x1B}, //开关->无  Mode->无   静音->ESC
    {0x44,0x00}, {0x40,0x25}, {0x43,0x27}, //播放->无  后退->向左 前进->向右
    {0x07,0x00}, {0x15,0x28}, {0x09,0x26}, // EQ->无   减号->向下 加号->向上
    {0x16, '0'}, {0x19,0x1B}, {0x0D,0x0D}, //'0'->'0'  箭头->ESC  U/SD->回车
    {0x0C, '1'}, {0x18, '2'}, {0x5E, '3'}, //'1'->'1'  '2'->'2'   '3'->'3'
    {0x08, '4'}, {0x1C, '5'}, {0x5A, '6'}, //'4'->'4'  '5'->'5'   '6'->'6'
    {0x42, '7'}, {0x52, '8'}, {0x4A, '9'}, //'7'->'7'  '6'->'8'   '9'->'9'
};

bit irflag = 0;   //红外接收标志
uint8 ircode[4];  //接收缓冲区

/* 红外接收驱动 */
void InfraredDriver()
{
    uint8 i;
    
    if (irflag)
    {
        irflag = 0;
        for (i=0; i<sizeof(IrCodeMap)/sizeof(IrCodeMap[0]); i++) 
        {
            if (ircode[2] == IrCodeMap[i][0])  
            {                                 
                KeyAction(IrCodeMap[i][1]);    
                break;
            }
        }
    }
}
/* 初始化红外接收功能 */
void InitInfrared()
{
    IR_INPUT = 1;  //引脚被释放
    TMOD &= 0x0F;  //清零T1
    TMOD |= 0x10;  //配置T1为模式1
    TR1 = 0;      
    ET1 = 0;       
    IT1 = 1;       //设置INT1为负边沿触发
    EX1 = 1;       
}
/* 获取当前高电平的持续时间 */
uint16 GetHighTime()
{
    TH1 = 0; 
    TL1 = 0;
    TR1 = 1;  
    while (IR_INPUT)  
    {
        if (TH1 >= 0x40)
        {            
            break;   //强制退出循环，避免程序假死
        }
    }
    TR1 = 0;  

    return (TH1*256 + TL1);  
}
/* 获取当前低电平的持续时间 */
uint16 GetLowTime()
{
    TH1 = 0;  
    TL1 = 0;
    TR1 = 1;  
    while (!IR_INPUT)  
    {
        if (TH1 >= 0x40)
        {            //低电平持续时间超过约18ms时，
            break;  
        }
    }
    TR1 = 0; 

    return (TH1*256 + TL1);  
}
/* 中断 */
void EXINT1_ISR() interrupt 2
{
    uint8 i, j;
    uint8 byt;
    uint16 time;
    
   
    time = GetLowTime();
    if ((time<23500) || (time>26265))  //8.5～9.5ms，
    {                               
        IE1 = 0;   
        return;
    }
    //接收并判定4.5ms高电平
    time = GetHighTime();
    if ((time<11059) || (time>13824))  //4.0～5.0ms，
    {                              
        IE1 = 0;
        return;
    }
   
    for (i=0; i<4; i++) 
    {
        for (j=0; j<8; j++)  
        {
            //接收判定每bit的560us低电平
            time = GetLowTime();
            if ((time<940) || (time>2156)) //340～780us，
            {                            
                IE1 = 0;
                return;
            }
            
            time = GetHighTime();
            if ((time>940) && (time<2156)) //340～780us，
            {                           
                byt >>= 1;  
            }
            else if ((time>4036) && (time<5253)) //1460～1900us，
            {                                   
                byt >>= 1;   
                byt |= 0x80; //高位置1
            }
            else 
            {
                IE1 = 0;
                return;
            }
        }
        ircode[i] = byt;  
    }
    irflag = 1; 
    IE1 = 0;     
}
