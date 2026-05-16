#include "stdint.h"
#include <stm32g4xx.h>

int main(void)
{
	while (1)
	{
		RCC->APB1ENR1 |= (1 << 17);
		RCC->AHB2ENR |= (1 << 0);
		USART2->BRR = 1667;
		USART2->CR1 |= (1 << 2) | (1 << 3);
		USART2->CR1 |= (1 << 0)
						   GPIOA->MODER |= (1 << 5);
		GPIOA->MODER &= ~(1 << 4);
		GPIOA->MODER |= (1 << 7);
		GPIOA->MODER &= ~(1 << 6);
		GPIOA->AFR[0] |= (7 << 8) | (7 << 12);
		char msg[] = "Hello\n";
		for (int i = 0; i < 6; i++)
		{
			while (!(USART2->ISR & (1 << 7)))
				;
			USART2->TDR = msg[i];
		}
	}
}
