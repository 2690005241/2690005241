#ifndef _i2c_h_
#define _i2c_h_


#include <STC89C5xRC.H>

sbit SCL=P3^6;
sbit SDA=P3^7;
void i2start();
void i2stop();
unsigned char i2sendbyte(unsigned char dat);
unsigned char i2readbyte();
void at24c02write(unsigned char addr,unsigned char dat);
unsigned char at24c02read(unsigned char addr);

#endif;

