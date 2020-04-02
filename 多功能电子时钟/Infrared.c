 /*�������





 
 */

#define  _INFRARED_C
#include "config.h"
#include "Infrared.h"
#include "Time.h"

const uint8 code IrCodeMap[][2] = {     //������뵽��׼PC�����ӳ���
    {0x45,0x00}, {0x46,0x00}, {0x47,0x1B}, //����->��  Mode->��   ����->ESC
    {0x44,0x00}, {0x40,0x25}, {0x43,0x27}, //����->��  ����->���� ǰ��->����
    {0x07,0x00}, {0x15,0x28}, {0x09,0x26}, // EQ->��   ����->���� �Ӻ�->����
    {0x16, '0'}, {0x19,0x1B}, {0x0D,0x0D}, //'0'->'0'  ��ͷ->ESC  U/SD->�س�
    {0x0C, '1'}, {0x18, '2'}, {0x5E, '3'}, //'1'->'1'  '2'->'2'   '3'->'3'
    {0x08, '4'}, {0x1C, '5'}, {0x5A, '6'}, //'4'->'4'  '5'->'5'   '6'->'6'
    {0x42, '7'}, {0x52, '8'}, {0x4A, '9'}, //'7'->'7'  '6'->'8'   '9'->'9'
};

bit irflag = 0;   //������ձ�־
uint8 ircode[4];  //���ջ�����

/* ����������� */
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
/* ��ʼ��������չ��� */
void InitInfrared()
{
    IR_INPUT = 1;  //���ű��ͷ�
    TMOD &= 0x0F;  //����T1
    TMOD |= 0x10;  //����T1Ϊģʽ1
    TR1 = 0;      
    ET1 = 0;       
    IT1 = 1;       //����INT1Ϊ�����ش���
    EX1 = 1;       
}
/* ��ȡ��ǰ�ߵ�ƽ�ĳ���ʱ�� */
uint16 GetHighTime()
{
    TH1 = 0; 
    TL1 = 0;
    TR1 = 1;  
    while (IR_INPUT)  
    {
        if (TH1 >= 0x40)
        {            
            break;   //ǿ���˳�ѭ��������������
        }
    }
    TR1 = 0;  

    return (TH1*256 + TL1);  
}
/* ��ȡ��ǰ�͵�ƽ�ĳ���ʱ�� */
uint16 GetLowTime()
{
    TH1 = 0;  
    TL1 = 0;
    TR1 = 1;  
    while (!IR_INPUT)  
    {
        if (TH1 >= 0x40)
        {            //�͵�ƽ����ʱ�䳬��Լ18msʱ��
            break;  
        }
    }
    TR1 = 0; 

    return (TH1*256 + TL1);  
}
/* �ж� */
void EXINT1_ISR() interrupt 2
{
    uint8 i, j;
    uint8 byt;
    uint16 time;
    
   
    time = GetLowTime();
    if ((time<23500) || (time>26265))  //8.5��9.5ms��
    {                               
        IE1 = 0;   
        return;
    }
    //���ղ��ж�4.5ms�ߵ�ƽ
    time = GetHighTime();
    if ((time<11059) || (time>13824))  //4.0��5.0ms��
    {                              
        IE1 = 0;
        return;
    }
   
    for (i=0; i<4; i++) 
    {
        for (j=0; j<8; j++)  
        {
            //�����ж�ÿbit��560us�͵�ƽ
            time = GetLowTime();
            if ((time<940) || (time>2156)) //340��780us��
            {                            
                IE1 = 0;
                return;
            }
            
            time = GetHighTime();
            if ((time>940) && (time<2156)) //340��780us��
            {                           
                byt >>= 1;  
            }
            else if ((time>4036) && (time<5253)) //1460��1900us��
            {                                   
                byt >>= 1;   
                byt |= 0x80; //��λ��1
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
