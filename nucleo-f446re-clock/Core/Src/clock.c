#include "clock.h"

// Define PLL configuration values
#define PLLM    4   // PLLM: Division factor for the main PLL input clock
#define PLLN    180 // PLLN: Main PLL multiplication factor
#define PLLP    0   // PLLP: Main PLL division factor (PLLP = 2 when PLLP = 0 in register)

/*
### Procedure Explanation:
1. **Enable HSE**: The STM32F446RE has multiple clock sources (HSE, HSI, PLL). HSE (High-Speed External) uses an external crystal, which provides better accuracy.
2. **Enable power interface clock**: This step is required to modify power settings via the PWR peripheral.
3. **Configure voltage regulator**: The microcontroller must be set to high-performance mode (Scale 1) to operate at 180 MHz.
4. **Enable Flash cache and prefetch**: Helps optimize instruction execution by reducing latency.
5. **Set Flash latency**: At high speeds (180 MHz), flash memory requires wait states to ensure data is read properly.
6. **Configure bus prescalers**:
   - AHB is left at full speed.
   - APB1 (for peripherals like UART, I2C) is divided by 4 (max 45 MHz).
   - APB2 (for high-speed peripherals like SPI) is divided by 2 (max 90 MHz).
7. **Configure PLL**: The PLL (Phase-Locked Loop) is configured to multiply the input clock frequency up to 180 MHz.
8. **Select HSE as PLL source**: The PLL must be based on a stable reference clock, which is why HSE is chosen.
9. **Enable PLL and wait for it to stabilize**: The PLL takes some time to lock onto the desired frequency.
10. **Switch system clock to PLL**: This step directs the microcontroller to use the newly configured PLL output as its main clock.
11. **Wait for confirmation**: Ensures that the system is running on PLL before proceeding.

### Purpose of this function:
- This function **sets up the system clock** to run at **180 MHz** using an external 8 MHz crystal oscillator and the PLL.
- The system bus and peripheral clocks are set to appropriate speeds.
- The microcontroller is configured for high performance and stable operation.

### Why is this necessary?
- STM32F446RE has a default system clock (HSI) of **16 MHz**, which is too slow for demanding applications.
- Running at **180 MHz** allows for faster execution of instructions, better real-time performance, and higher peripheral speeds.
- The correct prescaler settings ensure that peripherals do not exceed their maximum operating frequencies.
*/
void configSysClock(void){
    // 1. Enable High-Speed External (HSE) oscillator
    RCC->CR |= (1 << 16);
    // Wait until HSE is ready
    while(!(RCC->CR & (1 << 17)));

    // 2. Enable power interface clock
    RCC->APB1ENR |= (1 << 28);

    // 3. Configure voltage regulator for high performance
    PWR->CR |= (3 << 14); // Scale 1 mode (highest performance)

    // 4. Enable Flash instruction and data caches and prefetch buffer
    FLASH->ACR |= (7 << 8);

    // 5. Configure Flash latency (5 wait states for 180 MHz system clock)
    FLASH->ACR &= ~(15 << 0); // Clear latency bits
    FLASH->ACR |= (5 << 0);   // Set 5 wait states

    // 6. Configure bus prescalers
    // AHB prescaler (not divided)
    RCC->CFGR &= ~(8 << 4);
    // APB1 prescaler (divided by 4)
    RCC->CFGR &= ~(7 << 10);
    RCC->CFGR |= (5 << 10);
    // APB2 prescaler (divided by 2)
    RCC->CFGR &= ~(7 << 13);
    RCC->CFGR |= (2 << 13);

    // 7. Configure PLL settings
    RCC->PLLCFGR &= ~((1 << 15) - 1); // Clear PLLM and PLLN
    RCC->PLLCFGR &= ~(3 << 16);       // Clear PLLP
    RCC->PLLCFGR |= (PLLM << 0) | (PLLN << 6) | (PLLP << 16); // Set PLLM, PLLN, and PLLP

    // 8. Select HSE as PLL source
    RCC->PLLCFGR |= (1 << 22);

    // 9. Enable PLL
    RCC->CR |= (1 << 24);
    // Wait until PLL is locked
    while(!(RCC->CR & (1 << 25)));

    // 10. Select PLL as system clock source
    RCC->CFGR &= ~(3 << 0); // Clear system clock bits
    RCC->CFGR |= (2 << 0);  // Set PLL as system clock (SW = 10)

    // 11. Wait for system clock switch confirmation
    while((RCC->CFGR & 15) != 10);
}
