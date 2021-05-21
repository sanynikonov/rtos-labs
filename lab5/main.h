#include "stm32f10x.h"
#include "cmsis_os2.h"

void initGPIO(void);
void initTIM2(void);
void TIM2_IRQHandler(void);
void PWM1(void* argum);
void PWM2(void* argum);
void PWM3(void* argum);
void PWM4(void* argum);

osThreadId_t thread1;
osThreadId_t thread2;
osThreadId_t thread3;
osThreadId_t thread4;
