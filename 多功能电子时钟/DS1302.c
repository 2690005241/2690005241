///*/*ʵʱʱ��ģ��
//  /* ����һ���ֽڵ�DS1302ͨ�������� */
//void BWDS1302(uint8 dat)
// /* ��DS1302ͨ�������϶�ȡһ���ֽ� */
//uint8 BRDS1302()
///* �õ���д������ĳһ�Ĵ���д��һ���ֽڣ�reg-�Ĵ�����ַ��dat-��д���ֽ� */
//void SWDS1302(uint8 reg, uint8 dat)
///* �õ��ζ�������ĳһ�Ĵ�����ȡһ���ֽڣ�reg-�Ĵ�����ַ������ֵ-�������ֽ� */
//uint8 SRDS1302(uint8 reg)
// /* ��ͻ��ģʽ����д��8���Ĵ������ݣ�dat-��д������ָ�� */
//void BURSTWDS1302(uint8 *dat)
// /* ��ͻ��ģʽ������ȡ8���Ĵ��������ݣ�dat-��ȡ���ݵĽ���ָ�� */
//void BURSTRDS1302(uint8 *dat)
 ///* ��ȡʵʱʱ�䣬����ȡDS1302��ǰʱ�䲢ת��Ϊʱ��ṹ���ʽ */
//void GetTime(struct sTime *time)
///* �趨ʵʱʱ�䣬ʱ��ṹ���ʽ���趨ʱ��ת��Ϊ���鲢д��DS1302 */
//void SetTime(struct sTime *time)
///* DS1302��ʼ�����緢���������������ó�ʼʱ�� */
//void InitDS1302()	*/							*/

#define  _DS1302_C
#include "config.h"
#include "DS1302.h"

/* ����һ���ֽڵ�DS1302 */
void BWDS1302(uint8 dat)
{
    uint8 m;
    
    for (m=0x01; m!=0; m<<=1)  //Low position in front, displacement out
    {
        if ((m&dat) != 0) 
            DS1302_IO = 1;
        else
            DS1302_IO = 0;
        DS1302_CK = 1;       
        DS1302_CK = 0;       //Pull the clock higher and lower to complete a bit operation
    }
    DS1302_IO = 1;           //Make sure to release IO pins
}


/* ��DS1302�϶�ȡһ���ֽ� */
uint8 BRDS1302()
{
    uint8 m;
    uint8 dat = 0;
    
    for (m=0x01; m!=0; m<<=1)  
    {
        if (DS1302_IO != 0)  
        {
            dat |= m;
        }
        DS1302_CK = 1;       
        DS1302_CK = 0;       
    }
    return dat;              //Returns the bytes of data read
}


/* �õ���д������ĳһ�Ĵ���д��һ���ֽڣ� */
void SWDS1302(uint8 reg, uint8 dat)			
{
    DS1302_CE = 1;                   
    BWDS1302((reg<<1)|0x80);  //write instruction
    BWDS1302(dat);            //write in
    DS1302_CE = 0;                
}


/* ���ζ�����*/
uint8 SRDS1302(uint8 reg)
{
    uint8 dat;
    
    DS1302_CE = 1;                   
    BWDS1302((reg<<1)|0x81);  //Read register instruction
    dat = BRDS1302();          
    DS1302_CE = 0;                 
    
    return dat;
}


/* ��ͻ��ģʽ����д��8���Ĵ������� */
void BURSTWDS1302(uint8 *dat)
{
    uint8 i;
    
    DS1302_CE = 1;
    BWDS1302(0xBE);          //Burst write instruction
    for (i=0; i<8; i++)     //Write 8 bytes of data in a row
    {
        BWDS1302(dat[i]);
    }								
    DS1302_CE = 0;
}
/* ��ͻ��ģʽ������ȡ8���Ĵ��������� */
void BURSTRDS1302(uint8 *dat)
{
    uint8 i;
    
    DS1302_CE = 1;
    BWDS1302(0xBF);  //Burst read register instruction
    for (i=0; i<8; i++)     //Read 8 bytes in a row
    {
        dat[i] = BRDS1302();
    }
    DS1302_CE = 0;
}
/* ��ȡʵʱʱ�� */
void GetTime(struct sTime *time)
{
    uint8 buf[8];
    
    BURSTRDS1302(buf);
    time->year = buf[6] + 0x2000;
    time->mon  = buf[4];
    time->day  = buf[3];
    time->hour = buf[2];
    time->min  = buf[1];
    time->sec  = buf[0];
    time->week = buf[5];
}
/* �趨ʵʱʱ�䣬 */
void SetTime(struct sTime *time)
{
    uint8 buf[8];
    
    buf[7] = 0;
    buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->mon;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->min;
    buf[0] = time->sec;
    BURSTWDS1302(buf);
}
/* DS1302��ʼ�� */
void InitDS1302()
{
    uint8 dat;
    struct sTime code InitTime[] = {  					   //tacitly approve
        0x2020,0x04,0x03, 0x23,0x59,0x59, 0x03
    };
    
    DS1302_CE = 0;  //  initialization
    DS1302_CK = 0;
    dat = SRDS1302(0);  
    if ((dat & 0x80) != 0)       //Determine whether DS1302 has been stopped
    {
        SWDS1302(7, 0x00);  
        SetTime(&InitTime);      // set DS1302 as the default initial time
    }
}
