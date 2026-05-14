#include "stdint.h"
#include <stm32g4xx.h>

int main(void)
{
    RCC->AHB2ENR |= (1 << 0);
    GPIOA->MODER &= ~(1 << 11);
    GPIOA->MODER |= (1 << 10);

    while(1)
    {
        GPIOA->ODR ^= (1 << 5);
        for(volatile int i = 0; i < 500000; i++);
    }
}
