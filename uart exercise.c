#include "stdint.h"
#include <stm32g4xx.h>

int main(void)
{
	RCC->APB1ENR1 |= (1 << 17);			   // enable usart2 clock on APB1 bus
	RCC->AHB2ENR |= (1 << 0);			   // enable GPIOA clock on AHB2 bus
	USART2->BRR = 1667;					   // set baud rate (16 mhz/9600)
	USART2->CR1 |= (1 << 2) | (1 << 3);	   // Enable te and re first
	USART2->CR1 |= (1 << 0);			   // enable usart after
	GPIOA->MODER |= (1 << 5);			   // PA2 moder bit 5 (alternate)
	GPIOA->MODER &= ~(1 << 4);			   // PA2 moder bit 4 = 0 (function mode)
	GPIOA->MODER |= (1 << 7);			   // PA 3 moder bit 7 = 1 (alternate)
	GPIOA->MODER &= ~(1 << 6);			   // PA3 moder bit 6 = 0 (function mode)
	GPIOA->AFR[0] |= (7 << 8) | (7 << 12); // PA2 AND PA3 set to AF7 for USART2

	while (1)
	{
		char msg[] = "Hello\n";
		for (int i = 0; i < 6; i++)
		{
			while (!(USART2->ISR & (1 << 7)))
				;
			USART2->TDR = msg[i];
		}
	}
}
