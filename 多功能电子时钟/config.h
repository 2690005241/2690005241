

#ifndef _CONFIG_H
#define _CONFIG_H

#include <STC89C5xRC.h>
#include <intrins.h>

/* 数据类型定义 */
typedef  signed    char    int8;    // 8位有符号整型数
typedef  signed    int     int16;   //16位有符号整型数
typedef  signed    long    int32;   //32位有符号整型数
typedef  unsigned  char    uint8;   // 8位无符号整型数
typedef  unsigned  int     uint16;  //16位无符号整型数
typedef  unsigned  long    uint32;  //32位无符号整型数

/* 全局运行参数定义 */
#define SYS_MCLK   (33177600/12)  //主时钟晶振/12

sbit KEY_IN_1  = P2^0;  //矩阵按键的扫描输入引脚1
sbit KEY_IN_2  = P2^1;  //矩阵按键的扫描输入引脚2
sbit KEY_IN_3  = P2^2;  //矩阵按键的扫描输入引脚3
sbit KEY_IN_4  = P2^3;  //矩阵按键的扫描输入引脚4
sbit KEY_OUT_1 = P2^4;  //矩阵按键的扫描输出引脚1
sbit KEY_OUT_2 = P2^5;  //矩阵按键的扫描输出引脚2
sbit KEY_OUT_3 = P2^6;  //矩阵按键的扫描输出引脚3
sbit KEY_OUT_4 = P2^7;  //矩阵按键的扫描输出引脚4

sbit ADDR0 = P2^0;  //LED位选译码地址引脚0
sbit ADDR1 = P2^1;  //LED位选译码地址引脚1
sbit ADDR2 = P2^2;  //LED位选译码地址引脚2
sbit ENLED = P1^1;  //LED显示部件的总使能引脚

#define LCD1602_DB  P0   //1602液晶数据端口
sbit LCD1602_RS = P2^0;  //1602液晶指令/数据选择引脚
sbit LCD1602_RW = P2^1;  //1602液晶读写引脚
sbit LCD1602_E  = P1^2;  //1602液晶使能引脚

sbit DS1302_CE = P1^7;  //DS1302片选引脚
sbit DS1302_CK = P3^5;  //DS1302通信时钟引脚
sbit DS1302_IO = P3^4;  //DS1302通信数据引脚

sbit I2C_SCL = P3^6;  //I2C总线时钟引脚
sbit I2C_SDA = P3^7;  //I2C总线数据引脚

sbit BUZZER = P1^3;  //蜂鸣器控制引脚

sbit IO_18B20 = P3^5;  //DS18B20通信引脚

sbit IR_INPUT = P3^3;  //红外接收引脚


#endif
