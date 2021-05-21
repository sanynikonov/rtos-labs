#define Structs
#include "stm32f10x.h"

void delay(uint32_t ms);
void PWM(int GPIOAWorkTime, int GPIOBWorkTime, int period);
void GPIO_init(void);
