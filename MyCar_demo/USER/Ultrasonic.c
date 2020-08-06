#include "Ultrasonic.h"
#include "stm32f10x.h"  
#include "delay.h"

#define SoundSpeed 340    // ���� ��λΪm/s

void UltrasonicInit(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef TIM3_TimeBase;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
    //PA6--Trig
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    //TIM3_CH2--PA7--Echo
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //����Ϊ1MHz
    TIM3_TimeBase.TIM_ClockDivision = 0;
    TIM3_TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_TimeBase.TIM_Period = 8999;
    TIM3_TimeBase.TIM_Prescaler = 72-1;
		TIM3_TimeBase.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3, &TIM3_TimeBase);
}


float UltrasonicDetect(void)
{
    float time_s = 0;
	  int i;
	  float p[5];
		for(i=0;i<5;i++)
		{
				//����һ��10us�ĸߵ�ƽ����������
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				delay_us(10);
				GPIO_ResetBits(GPIOA, GPIO_Pin_6);			
		//  delay_ms(SONAR_MAX_WAIT_TIME);
			
				while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == RESET )
					{	}
				//��SONAR_MAX_WAIT_TIMEʱ���ڵȴ��Ƿ��л���

				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
				{ //���ܵ�����
						TIM3->CNT = 0;
						TIM_Cmd(TIM3, ENABLE); //��ʼ��ʱ
						while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == SET)
						{  }                      //��ѯ
						TIM_Cmd(TIM3, DISABLE); //������ʱ
						p[i] = TIM_GetCounter(TIM3)*1e-6; //����λ��usת��Ϊs				
				}
				time_s+=p[i];
				delay_ms(10);
			}
    time_s=time_s/5;
    return ( 100*time_s * SoundSpeed / 2); //���ؾ��� ��λcm
}

