#include "main.h"

volatile uint32_t ticks_delay;

int main()
{
	initGPIO();
	initTIM2();	
	
	osKernelInitialize();
	thread1 = osThreadNew(PWM1, GPIOA, NULL);
	thread2 = osThreadNew(PWM2, GPIOA, NULL);
	thread3 = osThreadNew(PWM3, GPIOA, NULL);
	thread4 = osThreadNew(PWM4, GPIOA, NULL);
	osKernelStart();
	flag1Set = osThreadFlagsSet(thread1, 0x00);
	while(1)
	{
		
	}
}

void initGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	
	GPIO_InitTypeDef GPIO_InitStructure;														//Init structure pin initializaion 
	
	GPIO_StructInit(&GPIO_InitStructure);														//Fill structure's fields with default values	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;					//GPIO 0,1,2,3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//Push&Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;								//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);													//Init GPIO A0, A1, A2, A3
}

void PWM1(void* argum)
{
	while(1)
	{
		flag1Wait = osThreadFlagsWait(0x00,osFlagsWaitAny,0);
		GPIO_SetBits(argum, GPIO_Pin_0);												
		delay(20);
		GPIO_ResetBits(argum, GPIO_Pin_0);
		delay(10);
		flag2Set = osThreadFlagsSet(thread2, 0x01);
		osThreadFlagsClear(0x00);
	}
}

void PWM2(void* argum)
{
	while(1)
	{
		flag2Wait = osThreadFlagsWait(0x01,osFlagsWaitAny,0);
		GPIO_SetBits(argum, GPIO_Pin_1);												
		delay(10);
		GPIO_ResetBits(argum, GPIO_Pin_1);
		delay(20);
		flag3Set = osThreadFlagsSet(thread3, 0x02);
		osThreadFlagsClear(0x01);
	}
}

void PWM3(void* argum)
{
	while(1)
	{
		flag3Wait = osThreadFlagsWait(0x02,osFlagsWaitAny,0);
		GPIO_SetBits(argum, GPIO_Pin_2);												
		delay(15);
		GPIO_ResetBits(argum, GPIO_Pin_2);
		delay(15);
		flag4Set = osThreadFlagsSet(thread4, 0x03);
		osThreadFlagsClear(0x02);
	}
}

void PWM4(void* argum)
{
	while(1)
	{
		flag4Wait = osThreadFlagsWait(0x03,osFlagsWaitAny,0);
		GPIO_SetBits(argum, GPIO_Pin_3);												
		delay(10);
		GPIO_ResetBits(argum, GPIO_Pin_3);
		delay(20);
		osThreadFlagsClear(0x03);
	}
}


void initTIM2()
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//Enable TIM2 clock
	
	TIM_TimeBaseInitTypeDef timer;																	//Time Base Init structure
		
	TIM_TimeBaseStructInit(&timer);																	//Fill structures's fields with default values
	
	timer.TIM_Prescaler = SystemCoreClock/10000 - 1;								
	timer.TIM_Period = 9;																						//Specifies the period value to be loaded into the active Auto-Reload Register					
	timer.TIM_ClockDivision = 1;																		//Specifies the clock division
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);																	//Init TIM2
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);																					//Start TIM2
	
	NVIC_InitTypeDef initNVIC;																			//Init Nested Vector Interrupted Controller structure
	initNVIC.NVIC_IRQChannel = TIM2_IRQn;														//Specify Interrupt ReQuest channel
	initNVIC.NVIC_IRQChannelPreemptionPriority = 0x00;							//Priority
	initNVIC.NVIC_IRQChannelSubPriority = 0x00;											//Subpriority
	initNVIC.NVIC_IRQChannelCmd = ENABLE;														//Enable IRQ channel
	NVIC_Init(&initNVIC);																						//init
}

void TIM2_IRQHandler()
{
	ticks_delay++;																									//Add 1 when interruption happend
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);											//Clear interruption bit
}

void delay(uint32_t ms)
{
	uint32_t start = ticks_delay;																		//time function was called
  while((ticks_delay - start) < ms);															//compare time that passed with delay
}
