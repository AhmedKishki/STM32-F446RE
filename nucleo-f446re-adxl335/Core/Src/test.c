#include "stm32f4xx.h"

void _initLED(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~(3 << 10);
    GPIOA->MODER |=  (1 << 10);
}

void _toggleLED(void) {
    GPIOA->ODR ^= (1 << 5);
}

void _initUART(void) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA2 = TX
    GPIOA->MODER &= ~(3 << (2 * 2));
    GPIOA->MODER |=  (2 << (2 * 2));
    GPIOA->AFR[0] |=  (7 << (2 * 4));

    USART2->CR1 &= ~USART_CR1_OVER8; // Oversampling by 16
    USART2->BRR = 139;               // 16 MHz / 115200 ≈ 139
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void _sendString(const char *str) {
    while (*str) {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *str++;
    }
}

int main(void) {
    _initLED();
    _initUART();

    while (1) {
        _toggleLED();
        _sendString("Hello, UART!\r\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}
