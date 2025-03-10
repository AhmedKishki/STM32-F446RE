#include "stm32f446xx.h"
#include "Timer.h"

void initSPI(void);
void initClocks(void);
void configSPI1Pins(void);
void setPinMode(void);
void setAlternateFunction(void);
void configSPI(void);
uint8_t transferSPI(uint8_t tx_data);


int main(void){
	// MPU9250 ADDRESS
	uint8_t data[4] = {187, 188, 189, 190};

	// STORE DATA RECEIVED FROM MPU)9250
	uint8_t rxd = 0;

	// SET UP TIMER
	initTim2();

	// SETUP SPI MASTER
	initSPI();

	while(1){
		// WRITE TO SPI
		rxd = transferSPI();
	}
}


void initClocks(void){
    //
    // Clock initialization is required to enable peripheral hardware blocks:
    // - GPIO ports (GPIOA and GPIOB)
    // - SPI1 peripheral
    //

    // Enable clocks for GPIO ports (AHB1 peripheral bus)
    RCC->AHB1ENR |= ((1 << 0)       // Enable GPIOA clock
                    |(1 << 1));     // Enable GPIOB clock

    // Enable clock for SPI1 peripheral (APB2 peripheral bus)
    RCC->APB2ENR |= (1 << 12);      // SPI1 clock enabled (bit 12)
}

uint8_t transferSPI(uint8_t tx_data){
	uint8_t rx_data = 0;

	// Enable SPI
	SPI1->CR1 |= (1 << 6);

	// Write data and dummy byte to data register
	SPI1->DR = (uint16_t)(tx_data << 8);

	// Wait until SPI is not BUSY and RX BUFFER is not empty
	while(((SPI1->SR) & (1 << 7)) || (!((SPI1->SR) & (1 << 0))));

	// Read a byte from the RX BUFFER
	rx_data = (uint8_t)SPI1->DR;

	// Disable SPI
	SPI1->CR1 &= ~(1 << 6);

	return rx_data;
}

void configSPI(void){
    // Step 1: Configure GPIO for MOSI, MISO, and SCK pins
    // (This step should be done separately in GPIO configuration code)

    // Step 2: Write to the SPI_CR1 register
    // Clearing specific bits to reset settings before configuration
    SPI1->CR1 &= ~(
                  (1 << 15)  // BIDIMODE: Set Full Duplex mode (0 = Full Duplex, 1 = Half Duplex)
                 |(1 << 13)  // CRCEN: Disable CRC (0 = CRC Calculation Disabled)
                 |(1 << 11)  // DFF: Clear Data Frame Format (Configuring below)
                 |(1 << 10)  // RXONLY: Disable Simplex mode (0 = Full Duplex)
                 |(1 << 9)   // SSM: Disable Software Slave Management (0 = Hardware NSS management)
                 |(1 << 7)   // LSBFIRST: Set MSB First (0 = MSB first, 1 = LSB first)
                 |(7 << 3)   // BR[2:0]: Clear baud rate control bits (used for SPI clock speed division)
                 );

    // Step 3: Write to SPI_CR2 register
    SPI1->CR2 &= ~(
                  (7 << 5)   // Clear Interrupt Bits
                 |(1 << 4)   // SPI in Motorola format (0 = Motorola, 1 = TI)
                 |(1 << 3)   // No consecutive transfer (0 = Standard SPI)
                 |(3 << 0)   // No DMA (DMA is not used)
                 );

    // Setting specific configuration bits
    SPI1->CR1 |= (
                  (1 << 11)  // DFF: 16-bit data transfer mode (Step 2h)
                 |(5 << 3)   // BR[2:0]: Set SPI clock prescaler to divide by 64 (Step 2a)
                 |(1 << 2)   // MSTR: Enable Master mode (Step 2g)
                 |(1 << 1)   // CPOL: Clock Polarity = 1 (Idle High) (Step 2b)
                 |(1 << 0)   // CPHA: Clock Phase = 1 (Capture on second edge) (Step 2b)
                 );

    // Step 3a: Configure SSOE (Slave Select Output Enable)
    SPI1->CR2 |= (1 << 2); // Enable SSOE (Step 3a)

    // Steps 4 and 5 (CRC polynomial and DMA configuration) are not included
    // since CRC and DMA are not used in this configuration.
}

void setPinMode(void){
    //
    // Setting up SPI1 requires configuring specific pins to Alternate Function mode:
    //
    // PA4  - SPI1 SSEL (Chip Select - Active Low)
    // PA6  - SPI1 MISO (Master In Slave Out - Data received by Master)
    // PA7  - SPI1 MOSI (Master Out Slave In - Data sent by Master)
    // PB3  - SPI1 SCLK (SPI Clock generated by Master)
    //

    // RESET PIN MODES: First clear the existing pin mode configuration
    GPIOA->MODER &= ~(
    				 (3 << (2 * 4))   // Clear bits [9:8]   for PA4
                    |(3 << (2 * 6))   // Clear bits [13:12] for PA6
                    |(3 << (2 * 7))   // Clear bits [15:14] for PA7
                    );
    GPIOB->MODER &= ~(3 << (2 * 3));   // Clear bits [7:6] for PB3

    // CONFIGURE PIN MODES: Set pins to Alternate Function (AF) mode (binary '10')
    GPIOA->MODER |= (
    				 (2 << (2 * 4))    // Set PA4 (SSEL) to AF mode
                    |(2 << (2 * 6))    // Set PA6 (MISO) to AF mode
                    |(2 << (2 * 7))    // Set PA7 (MOSI) to AF mode
                    );
    GPIOB->MODER |= (2 << (2 * 3));    // Set PB3 (SCLK) to AF mode
}

void setAlternateFunction(void){
    //
    // Configuring Alternate Function (AF) for SPI1:
    //
    // PA4  - SPI1 SSEL (Chip Select - Active Low)   -> AF5
    // PA6  - SPI1 MISO (Master In Slave Out)        -> AF5
    // PA7  - SPI1 MOSI (Master Out Slave In)        -> AF5
    // PB3  - SPI1 SCLK (Clock Signal)               -> AF5
    //

    // RESET ALTERNATE FUNCTION SETTINGS:
    // Each pin's alternate function is configured using 4-bit fields in the AFR register.
    // Clearing these bits ensures we start with a clean state before setting the desired AF.
    GPIOA->AFR[0] &= ~(
                         (0xF << (4 * 4))   // Clear 4 bits for PA4 (AFR[0] controls pins 0-7)
                        |(0xF << (4 * 6))   // Clear 4 bits for PA6
                        |(0xF << (4 * 7))   // Clear 4 bits for PA7
                        );
    GPIOB->AFR[0] &= ~(0xF << (4 * 3));   // Clear 4 bits for PB3

    // CONFIGURE ALTERNATE FUNCTION (AF5) FOR SPI1:
    // AF5 (binary 0101, decimal 5) is the alternate function used for SPI1.
    GPIOA->AFR[0] |= (
                         (5 << (4 * 4))   // Set PA4 to AF5 (SPI1_SSEL)
                        |(5 << (4 * 6))   // Set PA6 to AF5 (SPI1_MISO)
                        |(5 << (4 * 7))   // Set PA7 to AF5 (SPI1_MOSI)
                        );
    GPIOB->AFR[0] |= (5 << (4 * 3));   // Set PB3 to AF5 (SPI1_SCLK)
}


void initSPI(void){

}
