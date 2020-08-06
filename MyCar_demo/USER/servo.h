#include  "stm32f10x.h" 

#ifndef __SERVO_H
#define __SERVO_H

#define      SERVO_LEFT        25
#define      SERVO_FRONT       15
#define      SERVO_RIGHT       7

#define      ServoDelayTime    700 ///<�ȴ����ת����λ��ʱ�䣬��λΪ����


void ServoInit(void);

void ServoTurn(int dir);


#endif


