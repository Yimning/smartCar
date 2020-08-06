#include "stm32f10x.h"    
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include <stdio.h>
//#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "motor.h"
#include "bluetooth.h"
#include  "servo.h"
#include "Ultrasonic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h"
#include "MpuDisplay.h"

// Copyright (c) Zheng Zehan. All rights reserved.

extern int flag;
extern char flag2;

float Distance;
float DistanceLeft;
float DistanceRight;
float DistanceSafe=25;
char  Dist[10];

void boot(void)
{ 
    GPIO_InitTypeDef     GPIO_InitStructure;
	
	  //PC5  ��������
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	
  	//PA5  �̵���
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_5);

}

int main(void)
{
   delay_init();
	 boot();
	 MotorInit();
	while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == RESET)  //�ȴ�ֱ���������ؼ���
	{
		delay_ms(50);  
	}

//	GPIO_SetBits(GPIOA,GPIO_Pin_5); �̵�����ͨ
	
//	 uart_init(9600); ����Դ���ͨ��ʱ����
	 LED_Init();
	 LCD_Init();
	 MotorInit();
   ServoInit();
	 BluetoothInit();
	 UltrasonicInit();
	 MPU_Init();
	
	//LCD��ʾ��������
   LCD_Clear(WHITE);	
	 POINT_COLOR=BLUE;
	 LCD_ShowString(30,10,200,24,24,"STM32  ZZH's Car");

	 LCD_ShowString(30,40,200,16,16," State    :     ");	
	 LCD_ShowString(30,60,200,16,16," BlueTooth:  On");	
	 LCD_ShowString(30,80,200,16,16," Camera   :  Off");	
	 LCD_ShowString(30,100,200,16,16," Servo    :  On");
	 LCD_ShowString(30,120,200,16,16," Distance :        cm");	
	 LCD_ShowString(30,140,200,16,16," Temp:    .  C");	
   LCD_ShowString(30,160,200,16,16," AccX:    .  mm/s^2");	
	 LCD_ShowString(30,180,200,16,16," AccY:    .  mm/s^2");	
 	 LCD_ShowString(30,200,200,16,16,"GyroZ:    .  Degree/s");	
   LCD_ShowString(30,220,200,16,16,"Pitch:    .  Degree");	
   LCD_ShowString(30,240,200,16,16," Roll:    .  Degree");	  
	 LCD_ShowString(30,260,200,16,16," Yaw :    .  Degree");	 

	  while(1)
	  {

			while(mpu_dmp_init())  //���ٶȴ������Լ�
			{
				LCD_ShowString(130,40,200,16,16,"MPU6050 Error");
				delay_ms(200);
				LCD_Fill(130,40,239,130+16,WHITE);
				delay_ms(200);
			}  

			while(flag2=='W')   //�ȴ�״̬
				{
					 int j;
					 Stop();  
					 LCD_ShowString(135,40,200,16,16,"Stop");	
					 LCD_ShowString(30,290,300,16,16,"Waiting For Command...");	 //��˸��ʾ
					 Distance= UltrasonicDetect();
				   sprintf(Dist,"%3.3f",Distance);
    		   LCD_ShowString(130,120,100,16,16,Dist);
					for(j=0;j<4;j++)   //������ÿ��50ms����
					 {
						 MpuDisplay();
					   delay_ms(50);
					 }
					 LCD_Fill(30,290,200,300+16,WHITE);
					for(j=0;j<4;j++) 
					 {
						 MpuDisplay();
					   delay_ms(50);
					 }
				}
					
					
			while(flag2=='B')   //��������״̬
				{
          LCD_ShowString(135,40,200,16,16,"Controlled");	

					switch(flag)
           {
					case 0:  Stop();           break  ;
					case 1:  Turnleft(); 	     break  ;
					case 2:  Turnright(); 	   break  ;
					case 3:  Turnback();       break  ;
					case 4:  Turnfront();      break  ;

          default: Stop();           break  ;                                                                                                
	          }		  
				  delay_ms(50);
						
					MpuDisplay();  //��ʾ����������
				}


			while(flag2=='U')  //����������״̬
			{
				int j;
				LCD_ShowString(135,40,200,16,16,"Avoidence");	
		    Distance= UltrasonicDetect();
				sprintf(Dist,"%3.3f",Distance);
    		LCD_ShowString(130,220,100,16,16,Dist);     //��ʾ����
				
        if (Distance >= DistanceSafe)   //���ڰ�ȫ����
        {
            Turnfront(); //ǰ��
						MpuDisplay();
        }
        else
        {
            Stop(); 
            delay_ms(10);
            //��ȡ����ϰ��������Ϣ
            ServoTurn(SERVO_LEFT);
            DistanceLeft= UltrasonicDetect();
 
            //��ȡ�ұ��ϰ��������Ϣ
            ServoTurn(SERVO_RIGHT);
            DistanceRight = UltrasonicDetect();

            ServoTurn(SERVO_FRONT);         
					
            if(DistanceLeft>DistanceRight)
						{
						   Turnleft();
						}
						else
						{
						   Turnright();
						}				
					for(j=0;j<4;j++)  //ת����ʱ��ͬʱ���д�����������ʾ
					 {
						 MpuDisplay();
					   delay_ms(40);
					 } 
						Stop();   //ֹͣ �ٴν���ѭ��
						MpuDisplay();
						delay_ms(50);
        }				
			}
			
	  }
}


