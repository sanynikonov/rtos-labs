#include "stm32f10x.h"
#include "cmsis_os2.h"

void initGPIO(void);
void initTIM2(void);
void TIM2_IRQHandler(void);
void delay(uint32_t ms);
void PWM1(void* argum);
void PWM2(void* argum);
