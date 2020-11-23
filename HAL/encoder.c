#include "encoder.h"
#include "stm32f4xx.h"

/**
* @brief configure the io for encoder 1 
* @param [in]  none  
* @param [out] none 
*/
void Moto1_Encoder_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PB4 ch1  A,PB5 ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ��TIM3ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

    TIM_DeInit(TIM3);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1024*4;  //�趨��������װֵ   TIMx_ARR = 1024*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM3�ĸ��±�־λ
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//���и����ж�
    //Reset counter
    TIM3->CNT = 0x7fff;//

    TIM_Cmd(TIM3, ENABLE);   //����TIM3��ʱ��	
}

/**
* @brief configure the io for encoder 2 
* @param [in]  none  
* @param [out] none 
*/
void Moto2_Encoder_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM2ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��

	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1024*4;  //�趨��������װֵ   TIMx_ARR = 1024*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM1

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM1�ĸ��±�־λ
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
    //Reset counter
    TIM2->CNT = 0x7fff;//

    TIM_Cmd(TIM2, ENABLE);   //����TIM1��ʱ��	
}

/**
* @brief configure the io for encoder 3 
* @param [in]  none  
* @param [out] none 
*/
void Moto3_Encoder_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PB6 ch1  A,PB7 ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1024*4;  //�趨��������װֵ   TIMx_ARR = 359*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM4

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM4�ĸ��±�־λ
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//���и����ж�
    //Reset counter
    TIM4->CNT = 0x7fff;//

    TIM_Cmd(TIM4, ENABLE);   //����TIM3��ʱ��	
}

/**
* @brief configure the io for encoder 4
* @param [in]  none  
* @param [out] none 
*/
void Moto4_Encoder_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PB0 ch1  A,PA1 ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʹ��TIM5ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

    TIM_DeInit(TIM5);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1024*4;  //�趨��������װֵ   TIMx_ARR = 359*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
    TIM_ICInit(TIM5, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM5�ĸ��±�־λ
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//���и����ж�
    //Reset counter
    TIM5->CNT = 0x7fff;//

    TIM_Cmd(TIM5, ENABLE);   //����TIM5��ʱ��
}

/**
* @brief configure the io for encoder 
* @param [in]  none  
* @param [out] none 
*/
void Encoder_Init(void)
{
	Moto1_Encoder_Init();
	Moto2_Encoder_Init();
	Moto3_Encoder_Init();
	Moto4_Encoder_Init();
}

/**
* @brief update the encoder value
* @param [in]  none  
* @param [out] none 
*/
int  Get_Encoder_CNT(uint8_t driver_number)
{
	int cnt = 0;
	
	if(driver_number == 1)
	{
		 cnt = 0x7fff - TIM3->CNT;
		 TIM3->CNT = 0x7fff;
	}
    else if(driver_number == 2)
	{
		 cnt = (0x7fff - TIM2->CNT);
		 TIM2->CNT = 0x7fff;
	}
	else if(driver_number == 3)
	{
		 cnt = 0x7fff - TIM4->CNT;
		 TIM4->CNT = 0x7fff;
	}
	else if(driver_number == 4)
	{
		 cnt = 0x7fff - TIM5->CNT;
		 TIM5->CNT = 0x7fff;
	}
	else 
	{
		return 0;
	}
	
	return cnt;	
}


