#ifndef _smg_h_
#define _smg_h_


#define IO_dig P0;
sbit LSA=P2^0;
sbit LSB=P2^1;
sbit LSC=P2^2;

extern char display[8];
void delay(unsigned int i);
void digdisplay();


#endif