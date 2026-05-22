#include "stdint.h"
#include <stm32g4xx.h>
#include <stdbool.h>
#define BUFFER_SIZE 15

typedef struct
{
	int head;
	int tail;
	int count;
	uint8_t buffer[BUFFER_SIZE];
} circular_buffer;

void init(circular_buffer *cb)
{
	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;
}

bool dequeue(circular_buffer *cb, uint8_t *old_data)
{
	if (cb->count == 0)
	{
		return false;
	}
	*old_data = cb->buffer[cb->tail];
	cb->tail = (cb->tail + 1) % BUFFER_SIZE;
	cb->count--;
	return true;
}

bool enqueue(circular_buffer *cb, uint8_t new_data)
{
	if (cb->count == BUFFER_SIZE)
	{
		return false;
	}
	cb->buffer[cb->head] = new_data;
	cb->head = (cb->head + 1) % BUFFER_SIZE;
	cb->count++;
	return true;
}

circular_buffer cb; // create cb object of circular_buffer structure global variable for all functions to utilize

int main(void)
{

	RCC->APB1ENR1 |= (1 << 17); // clock
	RCC->AHB2ENR |= (1 << 0);
	GPIOA->MODER &= ~((1 << 5) | (1 << 4)); // clear both bits first
	GPIOA->MODER |= (1 << 5);				// then set to 10
	GPIOA->MODER &= ~((1 << 7) | (1 << 6)); // clear both bits first
	GPIOA->MODER |= (1 << 7);				// then set to 10
	GPIOA->AFR[0] |= (7 << 8) | (7 << 12);	// PA2 AND PA3 set to AF7 for USART2 (do not act as gpio, hand over control to USART peripheral)

	USART2->BRR = 1667;							   // baud rate
	USART2->CR1 |= (1 << 2) | (1 << 3) | (1 << 5); // TX AND RE ENABLE // RXNEIE -> enables rx interrupt

	USART2->CR1 |= (1 << 0); // usart2 enable

	NVIC_EnableIRQ(USART2_IRQn); // nvic enabler

	init(&cb);

	while (1)
	{
		uint8_t byte;
		__disable_irq(); // race condition
		bool got_byte = dequeue(&cb, &byte);
		__enable_irq();
		if (got_byte)
		{
			while (!(USART2->ISR & (1 << 7)))
				; // polling architecture, stalling loop until data trasnfer is finished
			USART2->TDR = byte;
		}
	}
}

// bad isr practice as they should be as short as possible, however, txe flag was getting stuck due to main loop dequeueing byte
void USART2_IRQHandler(void)
{
	if (USART2->ISR & (1 << 5))
	{											 // check rxne flag to see byte has arrived and is ready to read
		uint8_t received = (uint8_t)USART2->RDR; // clears rxne, read byte from recieve data register
		enqueue(&cb, received);					 // store in circular buffer
		while (!(USART2->ISR & (1 << 7)))
			;					// wait for transmit
		USART2->TDR = received; // echo byte back over tx
	}
}
