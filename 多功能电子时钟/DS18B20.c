///*/*�¶ȴ�����
//������
///* �����ʱ��(temp*10)us */void Delay10x(uint8 t)
///* ��λ���ߣ���ȡ�������壬������һ�ζ�д���� */bit Get18B20Ack()
///* ��DS18B20д��һ���ֽڣ�dat-��д���ֽ� */void W18B20(uint8 dat)
///* ��DS18B20��ȡһ���ֽ�*/uint8 R18B20()
///* ����һ��18B20�¶�ת���� */bit Operat18B20()
///* ��ȡDS18B20ת�����¶�ֵ������ֵ-��ʾ�Ƿ��ȡ�ɹ� */bit Get18B20T(int16 *t)	 



#define  _DS18B20_C
#include "config.h"						
#include "DS18B20.h"

/* �����ʱ��(t*10)us */
void Delay10x(uint8 t)	  //unit��ʾ8λ�з�������
{
    do {
        _nop_();		  //ÿ��nop��ʱ10um
        _nop_();			  //software delay
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--t);			   //t������ʱT*10um
}


/* ��λ���ߣ���ȡ�������壬������һ�ζ�д���� */
bit Get18B20Ack()
{
    bit ack;
    
    EA = 0;          
    IO_18B20 = 0;     //500us��λ����
    Delay10x(50);		
    IO_18B20 = 1;
    Delay10x (6);    
    ack = IO_18B20;     
    while(!IO_18B20);             //�������������
    EA = 1;                  
    
    return ack;
}


/* ��DS18B20д��һ���ֽڣ�dat-��д���ֽ� */
void W18B20(uint8 dat)	 
{
    uint8 mask;
    
    EA = 0; 
    for (mask=0x01; mask!=0; mask<<=1) 
    {
        IO_18B20 = 0;         //����2us�͵�ƽ
        Delay10x(2);
        if ((mask&dat) == 0)  
            IO_18B20 = 0;
        else
            IO_18B20 = 1;
        Delay10x(6);        //��ʱ60us
        IO_18B20 = 1;         //����
    }
    EA = 1;
}


/* ��DS18B20��ȡһ���ֽ�*/
uint8 R18B20()
{
    uint8 dat;
    uint8 mask;
    
    EA = 0;                         
    for (mask=0x01; mask!=0; mask<<=1)  //�ɼ��װ�λ
    {
        IO_18B20 = 0;         //����2us�ĵ͵�ƽ
        Delay10x(2);
        IO_18B20 = 1;         //��ʱ�����룬�ȴ�18B20�������
         Delay10x(2);
        if (!IO_18B20)        
            dat &= ~mask;
        else
            dat |= mask;
        Delay10x(6);       
    }
    EA = 1;                  

    return dat;							  
}


/* ����һ��18B20�¶�ת���� */
bit Operat18B20()						
{
    bit ack;
    
    ack = Get18B20Ack();   
    if (ack == 0)          //��18B20��ȷӦ��
    {
        W18B20(0xCC);  //����ROM
        W18B20(0x44);  //�¶�ת��
    }
    return ~ack;   //ack=0��ʾ�����ɹ�������ֵȡ������ֵ��ʾ�Ƿ������ɹ�
}

				  
/* ��ȡDS18B20ת�����¶�ֵ */
bit Get18B20T(int16 *t)	   //ָ��t
{
    bit ack;
    uint8 LSB, MSB; //16bit�¶�ֵ�ĵ��ֽں͸��ֽ�
    
    ack = Get18B20Ack();    
    if (ack == 0)           //��ȷӦ��
    {
        W18B20(0xCC);           //����ROM
        W18B20(0xBE);            
        LSB = R18B20();           //���ֽ�
        MSB = R18B20();             //���ֽ�
        *t = ((int16)MSB << 8) + LSB;  
    }
	return ~ack;  
}
