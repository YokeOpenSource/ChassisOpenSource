#include "spi.h"

void PS_Remoter_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA,B时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_15 | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //DI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_ResetBits(GPIOB,GPIO_Pin_14); 	
}

