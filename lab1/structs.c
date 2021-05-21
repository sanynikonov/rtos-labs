#include "main.h"
#ifdef Structs
#include "stm32f10x.h"

void PWM(int GPIOAWorkTime, int GPIOBWorkTime, int period)
{	
	GPIOA->BSRR |= GPIO_BSRR_BS4;											//Set GPIOA 4 pin
	GPIOB->BSRR |= GPIO_BSRR_BS12;											//Set GPIOB 12 pin
	
	delay(GPIOBWorkTime);
	GPIOB->BRR |= GPIO_BRR_BR12;										//Reset GPIOB 12 pin
	
	delay(GPIOAWorkTime - GPIOBWorkTime);
	GPIOA->BRR |= GPIO_BRR_BR4;											//Reset GPIOA 4 pin
	
	delay(period - GPIOAWorkTime);
}

void GPIO_init()
{
	RCC->APB2ENR|= RCC_APB2ENR_IOPAEN;										//Enable GPIOA clock
	RCC->APB2ENR|= RCC_APB2ENR_IOPBEN;										//Enable GPIOB clock
	
	
	GPIOA->CRL |= (GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1);					//50 MHz out mode for GPIOA4 [11] bits
	GPIOA->CRL &= ~GPIO_CRL_CNF4;											//Push&Pull [00] bits
	
	GPIOB->CRH |= (GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1);					//50 MHz out mode GPIOB12
	GPIOB->CRH &= ~GPIO_CRH_CNF12;											//Push&Pull
}
#endif