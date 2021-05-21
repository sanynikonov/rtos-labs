#include "main.h"
#ifdef Drivers
#include "stm32f10x.h"

void PWM(int GPIOAWorkTime, int GPIOBWorkTime, int period)
{	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);											//Set GPIOA 4 pin
	GPIO_SetBits(GPIOB, GPIO_Pin_12);											//Set GPIOB 12 pin
	
	delay(GPIOBWorkTime);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);										//Reset GPIOB 12 pin
	
	delay(GPIOAWorkTime - GPIOBWorkTime);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);										//Reset GPIOA 4 pin
	
	delay(period - GPIOAWorkTime);
}

void GPIO_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructureA4;										//Init structure
	GPIO_InitTypeDef  GPIO_InitStructureB12;									//Init second structure

	
	GPIO_InitStructureA4.GPIO_Pin = GPIO_Pin_4;									//GPIOA pin4
	GPIO_InitStructureA4.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA4.GPIO_Speed = GPIO_Speed_50MHz;							//Set structure field GPIO_Pin as GPIO_Pin_4
	
	GPIO_InitStructureB12.GPIO_Pin = GPIO_Pin_12;								//GPIOB pin12
	GPIO_InitStructureB12.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureB12.GPIO_Speed = GPIO_Speed_50MHz;						//50 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructureA4);									//Init GPIOA Pin4
	GPIO_Init(GPIOB, &GPIO_InitStructureB12);									//Init GPIOB Pin12
}
#endif