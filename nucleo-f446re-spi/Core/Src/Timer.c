#include "stm32f446xx.h"
#include "Timer.h"

void initTim2(void)
{
    // 1. Enable TIM2 clock (APB1)
    RCC->APB1ENR |= (1 << 0);  // TIM2EN = 1

    // 2. Set prescaler (PSC)
    // TIM2 clock = 84 MHz (APB1 Timer Clock) / (PSC + 1)
    // Target: 1 MHz tick frequency (1 µs per tick)
    TIM2->PSC = 83;  // 84 MHz / (83 + 1) = 1 MHz

    // 3. Set Auto-Reload Register (ARR) for 1 millisecond delay
    TIM2->ARR = 1000 - 1;  // Overflow every 1 ms

    // 4. Enable counter
    TIM2->CR1 |= (1 << 0);  // CEN = 1
}

void delay1Sec(void)
{
    // Wait for 1000 update events (each occurs every 1 ms)
    for (uint16_t i = 0; i < 1000; i++)
    {
        while (!(TIM2->SR & (1 << 0))); // Wait for UIF flag
        TIM2->SR &= ~(1 << 0); // Clear UIF flag
    }
}

void delay(unsigned int ms)
{
    uint32_t startTick = TIM2->CNT;
    uint32_t ticksRequired = ms * 1000;  // 1 ms = 1000 ticks (1 tick = 1 µs)

    while ((TIM2->CNT - startTick) < ticksRequired)
    {
        // Wait until required time has elapsed
    }
}

void delayUs(unsigned int us)
{
    uint32_t startTick = TIM2->CNT;

    // Since the timer is running at 1 MHz, 1 tick = 1 µs
    while ((TIM2->CNT - startTick) < us)
    {
        // Busy wait until time has elapsed
    }
}
