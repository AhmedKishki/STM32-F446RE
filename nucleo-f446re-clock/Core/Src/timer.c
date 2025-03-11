#include "timer.h"

void initTIM6(void) {
    // Enable TIM6 clock
    RCC->APB1ENR |= (1 << 4);

    // Set prescaler (PSC)
    TIM6->PSC = 90 - 1; // 90MHz / 90 = 1MHz (1 tick = 1µs)

    // Set ARR to max (not needed for polling delay)
    TIM6->ARR = 0xFFFF;

    // Enable counter (CEN bit)
    TIM6->CR1 |= (1 << 0);

    // Wait for update event to ensure the prescaler value is loaded
    TIM6->EGR |= (1 << 0);  // Generate an update event
    while (!(TIM6->SR & (1 << 0))); // Wait for UIF to be set
    TIM6->SR &= ~(1 << 0);  // Clear UIF flag
}

void usDelay(uint16_t us){
	TIM6->CNT = 0;
	while(TIM6->CNT < us);
}

void msDelay(uint16_t ms){
	for (uint16_t i=0; i<ms; i++){
		usDelay(1000); // delay of 1 ms
	}
}
