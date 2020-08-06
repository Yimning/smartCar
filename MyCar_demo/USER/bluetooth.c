#include "stm32f10x.h"  
#include "bluetooth.h"  
#include "sys.h"

u8 i;
int flag;  //�����־λ
char flag2='W';  //�����־λ2
int speed=75;//�����ٶ�speed   PWMռ�ձ�
void USART2_IRQHandler(void)  
  
{  
     if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)      //USART2 �жϷ������
      { 
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //����жϴ�����λ
			i=  USART_ReceiveData(USART2);                    //���ݴ���
		  if(i=='0')
				{ flag=0;}
			if(i=='1')
				{ flag=1;}
			if(i=='2')
			  { flag=2;}
			if(i=='3')
			  { flag=3;}
			if(i=='4')
			  { flag=4;}  			    //�ƶ�ָ��
				
			if(i=='W')
			  { flag2='W';}
			if(i=='B')
			  { flag2='B';}
			if(i=='U') 
			  { flag2='U';}         //״ָ̬��
			
			if(i=='H')
			  { speed=100;
					TIM_SetCompare1(TIM1,speed);
					TIM_SetCompare4(TIM1,speed);}
			if(i=='L')
			  { speed=75;
					TIM_SetCompare1(TIM1,speed);
					TIM_SetCompare4(TIM1,speed);}          //����
				
			if(i=='J')
			{ PAout(5)=!PAout(5);}  //�̵�������
				
		  }
}
	  
			
	void BluetoothInit(void)
	{
		
	//�����жϽ����������շ�
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );//ʹ��USART3��GPIOCʱ��
		
  //USART1_TX   PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
  //USART1_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3     
	
  //GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);

		USART_InitStructure.USART_BaudRate = 38400;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�

  //Usart1 NVIC ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
     
		USART_Cmd(USART2, ENABLE);  //ʹ�ܴ��� 
	
	}
	
