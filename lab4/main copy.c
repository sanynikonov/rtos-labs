#include "main.h"

const osThreadAttr_t thread1_attr = {
  .priority = osPriorityAboveNormal                           
};

osThreadId_t idThread1, idThread2;
osTimerId_t idTimer1, idTimer2;

void pwm_thread_func(void *argument);
void timer_callback(void *argument);

int main(void)
{
	initGPIO();
	
	osKernelInitialize();
	
	idThread1 = osThreadNew(pwm_thread_func, GPIOA, &thread1_attr);
	idThread2 = osThreadNew(pwm_thread_func, GPIOB, NULL);
	
	idTimer1 = osTimerNew(timer_callback, osTimerPeriodic, GPIOA, NULL);
	idTimer2 = osTimerNew(timer_callback, osTimerPeriodic, GPIOB, NULL);
	
	osTimerStart(idTimer1, 15U);
	osTimerStart(idTimer2, 15U);
	
	osKernelStart();
}

void timer_callback(void *argument)
{
	((GPIO_TypeDef *)argument)->ODR ^= GPIO_Pin_8
}

void initGPIO(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructure;														//Init pin initializaion structure
	
	GPIO_StructInit(&GPIO_InitStructure);														//Fill structure's fields with default values	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8;					//GPIO pin1 & pin9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//Push&Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;								//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);													//Init GPIOA Pin1 & Pin9
	GPIO_Init(GPIOB, &GPIO_InitStructure);													//Init GPIOB Pin1 & Pin9	
}

void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_SetBits(port, pin);												
	osDelay(dutyCycle);
	GPIO_ResetBits(port, pin);
	osDelay(period - dutyCycle);
}

void pwm_thread_func(void* argument)
{
	while(1){
		PWM(10, 20, argument, GPIO_Pin_1);
	}
}

