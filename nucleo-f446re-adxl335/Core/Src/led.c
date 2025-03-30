#include "led.h"
/*
	controls the LED on PA5
*/

void initLED(void)
{
    // Enable clock for GPIOA (AHB1 peripheral)
    RCC->AHB1ENR |= (1 << 0);  // Bit 0 of AHB1ENR enables GPIOA

    // Configure PA5 as output mode (MODER register)
    GPIOA->MODER |= (1 << 10); // Set bit 10 to 1 (PA5 as output mode)
    // Note: This does not clear the corresponding bit 11, so it's effectively 0b10,
    // which sets PA5 as "General Purpose Output Mode."

    // Configure PA5 as push-pull output (OTYPER register)
    GPIOA->OTYPER &= ~(1 << 5); // Clear bit 5 to set "Push-Pull" mode

    // Configure PA5 for fast speed (OSPEEDR register)
    GPIOA->OSPEEDR |= (1 << 11); // Set bit 11 to 1, keeping bit 10 as 0 (Fast speed)

    // No pull-up, no pull-down (PUPDR register)
    GPIOA->PUPDR &= ~(3 << 10); // Clear bits 10 and 11 (No pull-up, no pull-down)
}


void toggleLED(void)
{
    GPIOA->ODR ^= (1 << 5);  // Toggle PA5
}

