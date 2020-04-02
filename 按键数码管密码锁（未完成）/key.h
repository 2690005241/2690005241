#ifndef _key_h_
#define _key_h_

#include "STC89C5xRC.H"

#define	IO_key P1
extern unsigned char keyvalue;
extern unsigned char flog;
void keydown(void);

#endif