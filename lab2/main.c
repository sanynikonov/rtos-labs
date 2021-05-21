#include "main.h"

int main()
{
	runGPIO();
	runSystick();
	runTIM2();
	while(1)
	{
		if(!(GPIOB->IDR & GPIO_IDR_IDR4))				
		{	
			SysTick->CTRL ^= SysTick_CTRL_ENABLE_Msk;
			delay(1000);
		}
		else if(!(GPIOB->IDR & GPIO_IDR_IDR5))				 
		{
			increaseBrightness();
			delay(500);
		}
	}
}
void runGPIO(void)
{
	RCC->APB2ENR|= RCC_APB2ENR_IOPAEN;										//Enable GPIOA clock
	RCC->APB2ENR|= RCC_APB2ENR_IOPBEN;										//Enable GPIOB clock
	//SysTick
	GPIOB->CRL &= ~GPIO_CRL_MODE4;											 //Input mode
	GPIOB->CRL &= ~GPIO_CRL_CNF4;										     //Push&Pull
	GPIOB->CRL |= GPIO_CRL_CNF4_1;											 //Push&Pull
	GPIOB->BSRR |= GPIO_BSRR_BS4;											   //Pull up +3.3V
	//General timer
	GPIOB->CRL &= ~GPIO_CRL_MODE5;											 //Input mode
	GPIOB->CRL &= ~GPIO_CRL_CNF5;											   //Push&Pull
	GPIOB->CRL |= GPIO_CRL_CNF5_1;											 //Push&Pull
	GPIOB->BSRR |= GPIO_BSRR_BS5;											   //Pull up +3.3V
	//SysTick signal
	GPIOA->CRL |= (GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1);//50 MHz out mode for GPIOA4
	GPIOA->CRL &= ~GPIO_CRL_CNF4;												//Push&Pull
	//Alternate function for TIM2
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                 //Alternate function IO clock enable
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_1;
	//TIM2 signal
	GPIOA->CRL |= (GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1);//50 MHz out mode for GPIOA3 (Alternate function TIM2 channel 4)
	GPIOA->CRL &= ~GPIO_CRL_CNF3;												//Push&Pull
	GPIOA->CRL |= GPIO_CRL_CNF3_1;											//Push&Pull
}
void runSystick(void)
{
	 uint32_t TimerTick = (SystemCoreClock/1000)-1;
   SysTick->LOAD=TimerTick;      // Load value
   SysTick->VAL=TimerTick;      // Reset flags and timers
   SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);  //IRQ and enable
}
void SysTick_Handler(void) 
{
  static int tick = 0;
	if(tick == 15)
		GPIOA->BRR |= GPIO_BRR_BR4;											 
	else if(tick == 0)
		GPIOA->BSRR |= GPIO_BSRR_BS4;			
	if(tick == 19)
		tick = 0;
	else
		tick++;
}
void runTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
	TIM2->CCMR2 |= (TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2); //2nd PWM, 4th channel
	TIM2->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P;	// CC4E - Capture/Compare 4 output enable, CC4P - Capture/Compare 4 output polarity
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event = 100Hz
	//SystemCoreClock = 72MHz, TIM2,TIM3,TIM4 = 32MHz
	TIM2->PSC = 11; // Prescaler
	TIM2->ARR = 59999; // Period
	TIM2->CCR4 = ((TIM2->ARR+1)/100)-1;	//Initial value
	TIM2->CR1 |= TIM_CR1_CEN; // Counter enable TIM2
}
void increaseBrightness(void)									 		 
{
	if(TIM2->CCR4 == (((TIM2->ARR+1)/100*90)-1))
		TIM2->CCR4 = ((TIM2->ARR+1)/100)-1;
	else
		TIM2->CCR4 += ((TIM2->ARR+1)/200);
}
void delay(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; i++)
		for(uint32_t j = 0; j < 10000; j++) {}
}
