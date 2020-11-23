/**
 * @file pwm.h
 * @brief  pwm sourses file  
 */

#include "pwm.h"


void Motor1_PWM_Configuration()
{
    GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//BL1551 ENABLE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	  
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 500 -1 ;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = 0; 
    
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
    
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1	
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Motor2_PWM_Configuration()
{
    GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOA,&GPIO_InitStructure);              
	  
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
	
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 500 - 1 ;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器12
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
    
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Motor3_PWM_Configuration()
{
    GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	

    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOC,&GPIO_InitStructure);     

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	TIM_TimeBaseStructure.TIM_Prescaler = 4 -1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 500 -1 ;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器10
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
    
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
    
    TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

void Motor4_PWM_Configuration()
{
    GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8); 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOC,&GPIO_InitStructure);     

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);       
	  
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 500 -1 ;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器12
	
	//初始化TIM2 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
  
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM2
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	
}

void Motor_PWM_Configuration()
{
	Motor1_PWM_Configuration();
	Motor2_PWM_Configuration();
	Motor3_PWM_Configuration();
	Motor4_PWM_Configuration();
	
	MOTOR1_PWM_SWITCH_H;
	MOTOR2_PWM_SWITCH_H;
	MOTOR3_PWM_SWITCH_H;
	MOTOR4_PWM_SWITCH_H;
}








