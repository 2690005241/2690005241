

#ifndef _DS1302_H
#define _DS1302_H

struct sTime {  //����ʱ��ṹ
    uint16 year; //��
    uint8 mon;   //��
    uint8 day;   //��
    uint8 hour;  //ʱ
    uint8 min;   //��
    uint8 sec;   //��
    uint8 week;  //����
};

#ifndef _DS1302_C

#endif

void InitDS1302();
void GetTime(struct sTime *time);
void SetTime(struct sTime *time);

#endif
