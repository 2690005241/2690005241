

#ifndef _CONFIG_H
#define _CONFIG_H

#include <STC89C5xRC.h>
#include <intrins.h>

/* �������Ͷ��� */
typedef  signed    char    int8;    // 8λ�з���������
typedef  signed    int     int16;   //16λ�з���������
typedef  signed    long    int32;   //32λ�з���������
typedef  unsigned  char    uint8;   // 8λ�޷���������
typedef  unsigned  int     uint16;  //16λ�޷���������
typedef  unsigned  long    uint32;  //32λ�޷���������

/* ȫ�����в������� */
#define SYS_MCLK   (33177600/12)  //��ʱ�Ӿ���/12

sbit KEY_IN_1  = P2^0;  //���󰴼���ɨ����������1
sbit KEY_IN_2  = P2^1;  //���󰴼���ɨ����������2
sbit KEY_IN_3  = P2^2;  //���󰴼���ɨ����������3
sbit KEY_IN_4  = P2^3;  //���󰴼���ɨ����������4
sbit KEY_OUT_1 = P2^4;  //���󰴼���ɨ���������1
sbit KEY_OUT_2 = P2^5;  //���󰴼���ɨ���������2
sbit KEY_OUT_3 = P2^6;  //���󰴼���ɨ���������3
sbit KEY_OUT_4 = P2^7;  //���󰴼���ɨ���������4

sbit ADDR0 = P2^0;  //LEDλѡ�����ַ����0
sbit ADDR1 = P2^1;  //LEDλѡ�����ַ����1
sbit ADDR2 = P2^2;  //LEDλѡ�����ַ����2
sbit ENLED = P1^1;  //LED��ʾ��������ʹ������

#define LCD1602_DB  P0   //1602Һ�����ݶ˿�
sbit LCD1602_RS = P2^0;  //1602Һ��ָ��/����ѡ������
sbit LCD1602_RW = P2^1;  //1602Һ����д����
sbit LCD1602_E  = P1^2;  //1602Һ��ʹ������

sbit DS1302_CE = P1^7;  //DS1302Ƭѡ����
sbit DS1302_CK = P3^5;  //DS1302ͨ��ʱ������
sbit DS1302_IO = P3^4;  //DS1302ͨ����������

sbit I2C_SCL = P3^6;  //I2C����ʱ������
sbit I2C_SDA = P3^7;  //I2C������������

sbit BUZZER = P1^3;  //��������������

sbit IO_18B20 = P3^5;  //DS18B20ͨ������

sbit IR_INPUT = P3^3;  //�����������


#endif
