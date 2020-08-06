#include "stm32f10x.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h"
#include "lcd.h"
#include "led.h"
#include "MpuDisplay.h"

float pitch,roll,yaw; 		//ŷ����
short accx,accy,accz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;					//�¶�	
short num;

void MpuDisplay(void)
{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&accx,&accy,&accz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			
				if(temp<0)
				{
					LCD_ShowChar(30+48,140,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,140,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,140,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,140,temp%10,1,16);		//��ʾС������ 
				
				accx=accx*0.598; //  mm/s^2
				if(accx<0)
				{
					LCD_ShowChar(30+48,160,'-',16,0);		
					accx=-accx;	
				}else LCD_ShowChar(30+48,160,' ',16,0);	
				LCD_ShowNum(30+48+8,160,accx/100,3,16);		 
				LCD_ShowNum(30+48+40,160,accx%10,1,16);		
				
				accy=accy*0.598; //  mm/s^2
				if(accy<0)
				{
					LCD_ShowChar(30+48,180,'-',16,0);		
					accy=-accy;		
				}else LCD_ShowChar(30+48,180,' ',16,0);		
				LCD_ShowNum(30+48+8,180,accy/100,3,16);	  
				LCD_ShowNum(30+48+40,180,accy%10,1,16);	
				
				gyroz=gyroz/16.4;
				if(gyroz<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		
					gyroz=-gyroz;	
				}else LCD_ShowChar(30+48,200,' ',16,0);	
				LCD_ShowNum(30+48+8,200,gyroz/100,3,16);	  
				LCD_ShowNum(30+48+40,200,gyroz%10,1,16);	 
				
				num=pitch*10;
				if(num<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);	
					num=-num;	
				}else LCD_ShowChar(30+48,220,' ',16,0);		
				LCD_ShowNum(30+48+8,220,num/10,3,16);		    
				LCD_ShowNum(30+48+40,220,num%10,1,16);	
				
				num=roll*10;
				if(num<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		
					num=-num;	
				}else LCD_ShowChar(30+48,240,' ',16,0);	
				LCD_ShowNum(30+48+8,240,num/10,3,16);	    
				LCD_ShowNum(30+48+40,240,num%10,1,16);	
				
				num=yaw*10;
				if(num<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);	
					num=-num;	
				}else LCD_ShowChar(30+48,260,' ',16,0);	
				LCD_ShowNum(30+48+8,260,num/10,3,16);		    
				LCD_ShowNum(30+48+40,260,num%10,1,16);	 

				LED0=!LED0;//LED��˸
		}
  
}

