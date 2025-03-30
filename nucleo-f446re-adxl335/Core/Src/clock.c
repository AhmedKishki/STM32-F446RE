/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 180
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */

#include "clock.h"

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

void initCLK(void){
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************

	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set

	********************************************************/

	// Define PLL configuration values
	#define PLLM    4   // PLLM: Division factor for the main PLL input clock
	#define PLLN    180 // PLLN: Main PLL multiplication factor
	#define PLLP    0   // PLLP: Main PLL division factor (PLLP = 2 when PLLP = 0 in register)

	// 1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;


	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	// APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;


	// 5. Configure the MAIN PLL
	RCC->PLLCFGR = (PLLM <<0) | (PLLN << 6) | (PLLP <<16) | (RCC_PLLCFGR_PLLSRC_HSE);

	// 6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
