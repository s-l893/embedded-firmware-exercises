#include "stdint.h"
#include <stm32g4xx.h>
#include <stdbool.h>
#define BUFFER_SIZE 15

typedef struct{
	int head;
	int tail;
	int count;
	int buffer[BUFFER_SIZE];
} circular_buffer;

void init (circular_buffer*cb){
	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;
}

bool dequeue(circular_buffer *cb, uint8_t *old_data){
	if (cb->count == 0){
		return false;
	}
	*old_data = cb->buffer[cb->tail];
	cb->tail = (cb->tail + 1) % BUFFER_SIZE;
	cb->count--;
	return true;
}

bool enqueue(circular_buffer *cb, uint8_t new_data){
	if (cb->count == BUFFER_SIZE){
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
	RCC->APB1ENR1 |= (1<<17); // clock
	RCC->AHB2ENR |= (1<<0);
	GPIOA->MODER |= (1<<5) | (1<<7); // alternating function
	GPIOA->MODER &= ~((1<<4) | (1<<6)); // alternating function
	GPIOA->AFR[0] |= (7<<8) | (7<<12);

	USART2->BRR = 1667; // baud rate

	USART2->CR1 |= (1<<5); // RXNEIE -> enables rx interupt
	USART2->CR1 |= (1<<2) | (1<<3); //TX AND RE ENABLE
	USART2->CR1 |= (1<<0); // usart2 enable

	NVIC_EnableIRQ(USART2_IRQn); // nvic enabler


	init (&cb);
	while(1){
		uint8_t byte;
		if (dequeue(&cb, &byte)){
			while(!(USART2->ISR & (1<<7))); //polling architecture, stalling loop until data trasnfer is finished
			USART2->TDR = byte;

		}
	}

	}

void USART2_IRQHandler(void){ // ISR MUST BE NAMED THIS
	if (USART2->ISR & (1<<5)){
		enqueue(&cb, (uint8_t)USART2->RDR);
		USART2->ISR &= ~(1<<5);

	}
}
