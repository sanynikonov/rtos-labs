#include "main.h"

struct callback_param {
	volatile uint16_t* counter;
	GPIO_TypeDef* port;
};

struct callback_param param1;
struct callback_param param2;

volatile uint16_t counter_1 = 0;
volatile uint16_t counter_2 = 0;

int main(void)
{
	param1.counter = &counter_1;
	param1.port = GPIOA;
	
	param2.counter = &counter_2;
	param2.port = GPIOB;
	
	osThreadAttr_t thread1_attr;	
	thread1_attr.priority = osPriorityAboveNormal;

	osThreadId_t idThread1, idThread2;
	osTimerId_t idTimer1, idTimer2;
	
	GPIO_init();
	
	osKernelInitialize();
	
	idThread1 = osThreadNew(PWM_Thread, GPIOA, &thread1_attr);				//First thread has higher priority
	idThread2 = osThreadNew(PWM_Thread, GPIOB, NULL);
	
	idTimer1 = osTimerNew(callback, osTimerPeriodic, &param1, NULL);
	idTimer2 = osTimerNew(callback, osTimerPeriodic, &param2, NULL);
	
	osTimerStart(idTimer1, 10U);
	osTimerStart(idTimer2, 10U);
	
	osKernelStart();
}

void GPIO_init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructure;														//Init pin initializaion structure
	
	GPIO_StructInit(&GPIO_InitStructure);														//Fill structure's fields with default values	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_12;					//GPIO pin4 & pin12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//Push&Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;								//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);													//Init GPIOA Pin4 & Pin12
	GPIO_Init(GPIOB, &GPIO_InitStructure);													//Init GPIOB Pin4 & Pin12	
}

void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_SetBits(port, pin);												
	osDelay(dutyCycle);
	GPIO_ResetBits(port, pin);
	osDelay(period - dutyCycle);
}

void PWM_Thread(void* argument)
{
	while(1){
		PWM(15, 20, argument, GPIO_Pin_4);
	}
}

void callback(void *argument)
{
	struct callback_param *param_ptr = argument;
	
	if (*(param_ptr->counter) == 0)																				//First 10 ms - on
	{
		GPIO_SetBits(param_ptr->port, GPIO_Pin_12);
		*(param_ptr->counter) = 1;
	}
	else																																	//Second 10 ms - off
	{
		GPIO_ResetBits(param_ptr->port, GPIO_Pin_12);
		*(param_ptr->counter) = 0;
	}
}
