#include "stm32f446xx.h"
#include "Timer.h"

/*****************************************************************
* initTim2
*
* This function initialises timer 2.
* The STM32L432KC runs at 4MHz by default so 4,000,000 has been
* written into the ARR register to reset the counter every second.
*****************************************************************/
void initTim2(void)
{
	// 1. Enable TIM2 clock (APB1)
	RCC->APB1ENR |= (1 << 0);  // TIM2EN = 1

	// 2. Set prescaler (PSC)
	// TIM2 clock = 84 MHz (APB1 Timer Clock) / (PSC + 1)
	// Target: 1 ms tick => PSC = 8399 (since 84MHz / 8400 = 10kHz => 1 tick = 0.1ms)
	TIM2->PSC = 8399;

	// 3. Set Auto-Reload Register (ARR) for 1 second delay
	// Since 1 tick = 0.1ms, to get 1000ms: ARR = 10000
	TIM2->ARR = 10000;

	// 4. Enable counter
	TIM2->CR1 |= (1 << 0);  // CEN = 1
}

// Function to wait until 1 second passes
void delay1Sec(void)
{
    // Wait for update flag (UIF) to be set
    while (!(TIM2->SR & (1 << 0)));  // Wait for UIF = 1

    // Clear update flag (UIF)
    TIM2->SR &= ~(1 << 0);
}


void delay(unsigned int ms)
{
    // Calculate the number of ticks needed
    uint32_t startTick = TIM2->CNT;
    uint32_t ticksRequired = ms * 10000; // 1 ms = 10,000 ticks (0.1ms per tick)

    while ((TIM2->CNT - startTick) < ticksRequired)
    {
        // Wait until required time has elapsed
    }
}


void delayUs(unsigned int us)
{
    // Get initial TIM2 counter value
    uint32_t startTick = TIM2->CNT;

    // Calculate the required number of ticks
    uint32_t ticksRequired = us * 10; // 1 µs = 10 ticks (at 10 MHz timer clock)

    // Wait until the required delay has passed
    while ((TIM2->CNT - startTick) < ticksRequired)
    {
        // Busy wait until time has elapsed
    }
}

