int main(void)
{
	/* Loop forever */
	RCC->AHB2ENR |= (1 << 0);
	GPIOA->MODER &= ~(1 << 11);
	GPIOA->MODER |= (1 << 10);

	while (1)
	{
		GPIOA->ODR ^= (1 << 5);
		for (volatile int i = 0; i < 100000; i++)
			;
	}
}
