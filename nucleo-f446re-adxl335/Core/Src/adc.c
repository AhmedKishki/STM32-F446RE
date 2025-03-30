#include "stm32f4xx.h"  // Adjust include if needed
#include "adc.h"

// ---------------- Constants for ADC and sensor ---------------- //
#define ADC_MAX         4095.0f   // 12-bit ADC => 0..4095
#define V_REF           3.3f      // ADC reference voltage
#define ZERO_G_OFFSET   1.65f     // 0-g voltage from sensor
#define SENSITIVITY     0.3f      // V/g for ±3g sensor

void initADC(void)
{
    // 1) Enable ADC1 and GPIOA clocks
    RCC->APB2ENR |= (1<<8); // for ADC1
    RCC->AHB1ENR |= (1<<0); // for GPIOA

    // 2) Set ADC prescaler (PCLK2 / 4)
    //    - Clear bits first, then set prescaler
    ADC->CCR &= ~(3U << 16);  // Clear prescaler bits
    ADC->CCR |=  (1U << 16);  // Set prescaler = /4

    // 3) CR1: 12-bit resolution (bits[25:24] = 00), disable scan mode (bit 8=0)
    //    - All default after reset, but let's be explicit.
    ADC1->CR1 = 0;

    // 4) CR2: Single conversion (CONT=0), EOC after each conversion (EOCS=1),
    //         right alignment (ALIGN=0).
    //    - Reset value is 0x0000, so set EOCS=1 (bit 10).
    ADC1->CR2 = (1 << 10);

    // 5) Sampling time = 3 cycles for channels 0,1,4.
    //    SMPR2 uses 3 bits per channel:
    //      channel 0 -> bits [2:0]
    //      channel 1 -> bits [5:3]
    //      channel 4 -> bits [14:12]
    //    "000" means 3 cycles on STM32F4.
    ADC1->SMPR2 &= ~((7U << 0) | (7U << 3) | (7U << 12));

    // 6) SQR1: sequence length = 1 (L=0)
    ADC1->SQR1 = 0;

    // 7) Set GPIOA pins to analog mode:
    //    PA0 => channel 0
    //    PA1 => channel 1
    //    PA4 => channel 4
    GPIOA->MODER |= (3U << (0*2))  // PA0 analog (bits 1:0 = 11)
                |  (3U << (1*2))  // PA1 analog (bits 3:2 = 11)
                |  (3U << (4*2)); // PA4 analog (bits 9:8 = 11)

    // 8) Enable the ADC (ADON bit in CR2)
    ADC1->CR2 |= ADC_CR2_ADON;  // (1 << 0)

    // 9) Wait a short time for ADC to stabilize (~10µs)
    for(volatile int i=0; i<1000; i++);
}

/**
 * @brief readADC
 * Reads channels 0, 1, and 4 (one at a time in single-conversion mode)
 * and converts each raw value to acceleration in g.
 *
 * @param accels Pointer to array of 3 floats, where:
 *               accels[0] = X in g (from channel 0)
 *               accels[1] = Y in g (from channel 1)
 *               accels[2] = Z in g (from channel 4)
 */
void readADC(float *accels)
{
    // Channels to read, in order
    uint8_t channels[3] = { 0, 1, 4 };

    for (int i = 0; i < 3; i++)
    {
        // 1) Select the channel in SQR3 (one conversion in the sequence)
        ADC1->SQR3 = 0;
        ADC1->SQR3 = (channels[i] & 0x1F); // channel # in bits [4:0]

        // 2) Start the conversion by setting SWSTART
        ADC1->CR2 |= (1 << 30);

        // 3) Wait for EOC (end of conversion)
        while (!(ADC1->SR & (1U << 1)))
        {
            // spin-wait
        }

        // 4) Read the raw 16-bit ADC result
        uint16_t raw = (uint16_t)(ADC1->DR & 0xFFFF);

        // ---- Convert the raw ADC value into acceleration (in g) ----
        float voltage = ( (float)raw / ADC_MAX ) * V_REF;
        float offset  = voltage - ZERO_G_OFFSET;
        float accel_g = offset / SENSITIVITY;

        // Store result in the output array
        accels[i] = accel_g;
    }
}
