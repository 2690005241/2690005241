#include <STC12C5A60S2.H>

sbit ADDR0 = P2^0; 
sbit ADDR1 = P2^1;
sbit ADDR2 = P2^2;
sbit ENLED = P1^1;

sbit keyout1 = P2^4;
sbit keyout2 = P2^5;
sbit keyout3 = P2^6;
sbit keyout4 = P2^7;

sbit keyin1 = P2^0;
sbit keyin2 = P2^1;
sbit keyin3 = P2^2;
sbit keyin4 = P2^3;

unsigned char code ledchar [] = {				 //数码管真值表
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E	
};
unsigned char ledbuff [6] ={								//数码管缓存区
	0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF
} ;
unsigned char keystate [4][4]={						   //按键状态
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
};
void keyscan (); 
void ledscan ();
void keycontrol ();
void interrupttimer0() ;
  


void main()
{
	TMOD =0x01;		 //定时器模式为T0
	TH0 = 0xFB;	   
	TL0 = 0xAE;
	TR0 = 1;		   //打开定时器
	EA = 1;		   //打开总中断
	ET0 = 1;			   //打开T0中断

	while(1)
	{
		keycontrol();
	}
}

/*按键扫描函数*/
void keyscan ()   
{
	unsigned char i ;
	static unsigned char out = 0;
	static unsigned char keybuff [4][4] = {	 //矩阵按键缓存区
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF}
	};
	keybuff[out][0] = (keybuff[out][0]<<1)|keyin1;	//将一行四个按键值移入缓存区
	keybuff[out][1] = (keybuff[out][1]<<1)|keyin2;
	keybuff[out][2] = (keybuff[out][2]<<1)|keyin3;
	keybuff[out][3] = (keybuff[out][3]<<1)|keyin4;
	
	for (i=0;i<4;i++)								   	//四行按键依次扫描
	{
		if((keybuff[out][i]&0x0F)==0x00 )	  //按键消抖，四位扫描，如果全部为零则按键已经按下
		{
			keystate[out][i] = 0;		  //存储按键状态
		}
		else if((keybuff[out][i]&0x0F)==0x0F)		 //如果四位扫描值全部为一，则按键已经抬起
		{
			keystate[out][i]=1;		//保存按键状态
		}
		else
		{
													  //按键抖动则不执行操作
		}
	}
	out++;										//进行下一次扫描
	if(out>=4)								  //扫描完四次后重新开始扫描
	{
		out = 0;
	}
	switch (out)							   //按键一次扫描
	{
	case 0:keyout4=0;keyout3=1;keyout2=1;keyout1=1;break;
	case 1:keyout4=1;keyout3=0;keyout2=1;keyout1=1;break;
	case 2:keyout4=1;keyout3=1;keyout2=0;keyout1=1;break;
	case 3:keyout4=1;keyout3=1;keyout2=1;keyout1=0;break;
	default :break;
	}

}

/*数码管扫描函数*/
/*
void ledscan ()
{
	static unsigned char i;
	P0=0xFF;   //数码管消影

	switch (i)
	{
	case 0:ADDR0=1;ADDR1=0;ADDR2=1;P0=ledbuff[5];break;	  
	case 1:ADDR0=0;ADDR1=0;ADDR2=1;P0=ledbuff[4];break;
	case 2:ADDR0=1;ADDR1=1;ADDR2=0;P0=ledbuff[3];break;
	case 3:ADDR0=0;ADDR1=1;ADDR2=0;P0=ledbuff[2];break;
	case 4:ADDR0=1;ADDR1=0;ADDR2=0;P0=ledbuff[1];break;
	case 5:ADDR0=0;ADDR1=0;ADDR2=0;P0=ledbuff[0];break;
	default :break;
	}
}
  */

/*按键控制函数*/

void keycontrol ()
{
	unsigned char i = 0,j = 0;
	static unsigned char backup[4][4]=
	{
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1}
	};
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			if(keystate[i][j]!=backup[i][j])
			{
				if(backup[i][j]==1)
				{
				ledbuff[5] =ledchar [(i*4+j+1)%10];
				ledbuff[4] =ledchar [(i*4+j+1)/10];
				}
			}
		}
		backup[i][j]=keystate [i][j];

	}
}


/*中断函数*/
void interrupttimer0() interrupt 1
{
	static bit x = 0;			//标志位

	TH0=0xFB;						//定时器重新加载初值
	TL0=0xAE;

	if(x)					  //一次扫描一个
	{
		ENLED =1;
		keyscan();
		ENLED =0;
	}
	else
	{
		ledscan();
	}
		x =~x;			  //取反 下次扫描另一个
}

