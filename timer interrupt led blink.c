#include "stdint.h"
#include <stm32g4xx.h>

int main(void)
{
	RCC->APB1ENR1 |= (1 << 0);	// enable TIM2 clock on APB1 bus
	RCC->AHB2ENR |= (1 << 0);	// enable clock for GPIOA
	GPIOA->MODER &= ~(1 << 11); // set to alternating function
	GPIOA->MODER |= (1 << 10);	// ^^^

	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->PSC = 15999;		// correlates ticks to seconds, 16 mhz / 16000 for 1000 ticks/s
	TIM2->ARR = 999;		// auto reload register (threshold)
	TIM2->DIER |= (1 << 0); // interupt enable register -> fire intterupt every time counter reloads
	TIM2->CR1 |= (1 << 0);	// Enable timer itself
	while (1)
	{
	}
}
void TIM2_IRQHandler(void)
{							// ISR FUNCTION
	TIM2->SR &= ~(1 << 0);	// clear interrupt
	GPIOA->ODR ^= (1 << 5); // toggle led
}
