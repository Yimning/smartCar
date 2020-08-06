#include "stm32f10x.h"   
#include "motor.h"   


void MotorInit(void)
{

	GPIO_InitTypeDef             GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef      TIM1_TimeBase;
  TIM_OCInitTypeDef            TIM1_OC;
		
	//��ʼ��GPIO PA12 13 14 15 ���ӵ��  //PA8 PA11 PWM
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);//����SW��JTAG ��ʹ��GPIO����

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_ResetBits(GPIOA, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//��ʼ����ʱ��
	TIM1_TimeBase.TIM_Period = 99; //�����Զ���װ�ؼĴ��� ������5000Ϊ500ms
	TIM1_TimeBase.TIM_Prescaler =720-1; //����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM1_TimeBase.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM1_TimeBase.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM1_TimeBase); 
	
	//����Ƚ�
  TIM1_OC.TIM_OCMode=TIM_OCMode_PWM1;
	TIM1_OC.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM1_OC.TIM_OutputState=TIM_OutputState_Enable;
	TIM1_OC.TIM_Pulse=0;
  TIM_OC1Init(TIM1,&TIM1_OC); 
	TIM_OC4Init(TIM1,&TIM1_OC); 
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
	
	//ʹ��Ԥװ�ؼĴ���
  TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE); 

	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}


//ֹͣ
void Stop(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}

//ǰ��
void Turnfront(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	GPIO_SetBits(GPIOA,GPIO_Pin_14); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}

//����
void Turnback(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_SetBits(GPIOA,GPIO_Pin_13);
	GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}

//��ת
void Turnleft(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_12); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}

//��ת
void Turnright(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_SetBits(GPIOA,GPIO_Pin_13);
	GPIO_SetBits(GPIOA,GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}




