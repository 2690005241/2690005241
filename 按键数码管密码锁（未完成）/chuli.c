 #include"chuli.h"
sbit led = P1^1;
unsigned int code,lock,chi;
long num;
char display[8];
char keynum[17]={0x3f,0x06,0x5b,0x4f,
				 0x66,0x6d,0x7d,0x07,
				 0x7f,0x6f,0x77,0x7c,
				 0x39,0x5e,0x79,0x71};
void codejudge()
{
	code=at24c02read(1);
	if(code=num)
	{
	lock=1;
	num=0;
	keyvalue=0;
	chi=0;
	}	

}
void codechange()
{
int i;
if(lock==1)
{								//12清除、10保存
	if(keyvalue==10)
	{
		at24c02erite(1,num);
		for(i=0;i<8;i++)
		{
			display[i]
		}
	}
}
}
void codein()
{
long z,y,x,w,v,u;
keydown();		   //按键处理
if(flag==1)
{
	if(keyvalue!=10&&keyvalue!=11&&keyvalue!=12&&keyvalue!=13&&keyvalue!=14&&keyvalue!=15)
	{
		switch(chi)		 //做加法处理
		{
			case 0:num=z=keyvalue;break;
			case (1):num=y=z+keyvalue;break;
			case (2):num=x=y+keyvalue;break;
			case (3):num=w=x+keyvalue;break;
			case (2):num=v=w+keyvalue;break;
			case (3):num=u=v+keyvalue;break;

		}
		chi++;
		display[7]=display[6];
		display[6]=display[5];
		display[5]=display[4];
		display[4]=display[3];
		display[3]=display[2];
		display[2]=display[1];	//按键在数码管显示
		display[1]=display[0];
		display[0]=keynum[keyvalue];
		flog=0;;
	}
}


}
void  codelock()
{
	codein();			  //code输入
	codejudge();					//code判断
	codechange();							  //修改code
	if (keyvalue==11)lock=0;
	if(keyvalue==12)						  //清除
	{
	keyvalue=0;chi=0;
	}
	if(lock==0)										//指示灯
	{
		led=1;
	}
	if(lock==1)
	led=0;
}
