#include "main.h"


int main()
{
	initGPIO();
	
	osKernelInitialize();
	thread1 = osThreadNew(PWM1, GPIOA, NULL);
	thread2 = osThreadNew(PWM2, GPIOA, NULL);
	thread3 = osThreadNew(PWM3, GPIOA, NULL);
	thread4 = osThreadNew(PWM4, GPIOA, NULL);
	osThreadFlagsSet(thread1, 0x1);
	osKernelStart();
	
	while(1)
	{
		
	}
}

void initGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);									//Enable GPIOA clock
	
	GPIO_InitTypeDef GPIO_InitStructure;													//Init structure pin initializaion 
	
	GPIO_StructInit(&GPIO_InitStructure);													//Fill structure's fields with default values	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;		//GPIO 0,1,2,3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;										//Push&Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;										//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);													//Init GPIO A0, A1, A2, A3
}

void PWM1(void* argum)
{
	while(1)
	{
		osThreadFlagsWait(0x1, osFlagsWaitAll, osWaitForever);
		GPIO_SetBits(argum, GPIO_Pin_0);												
		osDelay(20);
		GPIO_ResetBits(argum, GPIO_Pin_0);
		osDelay(10);
		osThreadFlagsSet(thread2, 0x1);
	}
}

void PWM2(void* argum)
{
	while(1)
	{
		osThreadFlagsWait(0x1, osFlagsWaitAll, osWaitForever);
		GPIO_SetBits(argum, GPIO_Pin_1);												
		osDelay(10);
		GPIO_ResetBits(argum, GPIO_Pin_1);
		osDelay(20);
		osThreadFlagsSet(thread3, 0x1);
	}
}

void PWM3(void* argum)
{
	while(1)
	{
		osThreadFlagsWait(0x1, osFlagsWaitAll, osWaitForever);
		GPIO_SetBits(argum, GPIO_Pin_2);												
		osDelay(15);
		GPIO_ResetBits(argum, GPIO_Pin_2);
		osDelay(15);
		osThreadFlagsSet(thread4, 0x1);
	}
}

void PWM4(void* argum)
{
	while(1)
	{
		osThreadFlagsWait(0x1, osFlagsWaitAll, osWaitForever);
		GPIO_SetBits(argum, GPIO_Pin_3);												
		osDelay(10);
		GPIO_ResetBits(argum, GPIO_Pin_3);
		osDelay(20);
		osThreadFlagsSet(thread1, 0x1);
	}
}
