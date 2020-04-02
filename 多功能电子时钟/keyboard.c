// void KeyScan()����ɨ��

//	void KeyDriver()����ִ�в���
//
//
//
//
//
//
//
//
#define  _KEY_BOARD_C
#include "config.h"
#include "keyboard.h"
#include "Time.h"
const uint8 code KeyCodeMap[4][4] = {  //���󰴼�����׼�����ӳ���
    { '1',  '2',  '3', 0x26 },     //1��2��3������
    { '4',  '5',  '6', 0x25 },    //4��5��6������
    { '7',  '8',  '9', 0x28 },    //7��8��9������
    { '0', 0x1B, 0x0D, 0x27 }     //0��ESC���س��� ����
};
uint8 pdata KeySta[4][4] = {  
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

void KeyDriver()
{
    uint8 i, j;
    static uint8 pdata backup[4][4] = {              //����ֵ����
        {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
    };
    
    for (i=0; i<4; i++)  
    {
        for (j=0; j<4; j++)
        {
            if (backup[i][j] != KeySta[i][j])    
            {
                if (backup[i][j] != 0)           
                {
                    KeyAction(KeyCodeMap[i][j]); 
                }
                backup[i][j] = KeySta[i][j];     //ˢ�±���ֵ
            }
        }
    }
}

void KeyScan()
{
    uint8 i;
    static uint8 keyout = 0;  
    static uint8 keybuf[4][4] = {  
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
    };

    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;	//����ɨ��
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
   
    for (i=0; i<4; i++)                           // ɨ��ÿ�е��ĸ�����
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {  
            KeySta[keyout][i] = 0;										   //��������
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   
            KeySta[keyout][i] = 1;
        }
    }
  
    keyout++;                 
    keyout &= 0x03;         
    switch (keyout)         //�ͷŵ�ǰ������ţ������´ε��������
    {
        case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
        case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
        case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
        case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
        default: break;
    }
}
