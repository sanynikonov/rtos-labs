#include "main.h"

void delay(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; i++)
		for(uint32_t j = 0; j < 10000; j++) {}
}

int main()
{
	GPIO_init();
	
	int period = 20;																					//20 ms period
	int GPIOAWorkTime = 15;																		//15 ms GPIOA4
	int GPIOBWorkTime = 10;																		//10 ms GPIOB12

	while(1)
	{
		PWM(GPIOAWorkTime, GPIOBWorkTime, period);
	}
}
