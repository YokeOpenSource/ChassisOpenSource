#include "module_ps2_remoter.h"
#include "delay.h"
#include "string.h"
#include "stdbool.h"

u16 Handkey;
u8 Comd[2]={0x01,0x42};	//开始命令。请求数据
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组

u16 MASK[]={ 
    PSB_SELECT, 
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明

//向手柄发送命令
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出以为控制位
		}
		else DO_L;

		CLK_H;                        //时钟拉高
		delay_us(10);
		CLK_L;
		delay_us(10);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	
	delay_us(16);
}

//判断是否为红灯模式
//返回值；0，红灯模式
//		  其他，其他模式
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}

//读取手柄数据
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据

	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			delay_us(10);
			CLK_L;
			delay_us(10);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;	
}

//对读出来的PS2的数据进行处理      只处理了按键部分         默认数据是红灯模式  只有一个按键按下时
//按下为0， 未按下为1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}

//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}

//进入配置
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}

//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0xee); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}

//振动设置
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}

//完成并保存配置
void PS2_ExitConfing(void)
{
  CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置
}

int16_t angle;

double remoter_x_persent = 0.0,remoter_y_persent = 0.0f,remoter_z_persent = 0.0f;

double Filter_Data(double data)
{
	if(data > -0.01 && data < 0.01)
		return 0;
	else 
		return data;
}

int PS2_KEY;
uint8_t isY_Enbale = 0;
uint8_t last_ch4 = 0;
uint8_t isDisable_Avoidance = 0;
uint8_t UpdatePSRemoterData(void)
{
	uint8_t ps_remoter_online = 0;
	//清除缓存
	PS2_ClearData();   
	//读数据
	PS2_ReadData();	
	
	if(Data[1] == 0x73) //红灯模式
	{
		if((Data[4] & (1 << 4)) == 0)
		{
			isY_Enbale = 1;
		}
		
		if((Data[4] & (1 << 6)) == 0)
		{
			isY_Enbale = 0;
		}
		
		
		if((Data[4] & (1 << 7)) == 0)
		{
			isDisable_Avoidance = 1;
		}
		
		if((Data[4] & (1 << 5)) == 0)
		{
			isDisable_Avoidance = 0;
		}
		
		ps_remoter_online = 1;
			
		remoter_x_persent = - Filter_Data((PS2_AnologData(8) - 127.0f) / 127.0F);
		
		if(isY_Enbale)		
			remoter_y_persent =  Filter_Data((PS2_AnologData(7) - 127.0f) / 127.0F);  
		else
			remoter_y_persent = 0;
		
			remoter_z_persent =  Filter_Data((PS2_AnologData(5) - 127.0f) / 127.0F);

		
			
		PS2_Vibration(0,0);	

	}
	else
	{
		ps_remoter_online = 0;
		remoter_x_persent = 0;
		remoter_y_persent = 0;
		remoter_z_persent = 0;
	}
	
	return ps_remoter_online;
}

double PS2_GetX_Percent(void)
{
	return remoter_x_persent;
}

double PS2_GetY_Percent(void)
{
	return remoter_y_persent;
}

double PS2_GetZ_Percent(void)
{
	return remoter_z_persent;
}
