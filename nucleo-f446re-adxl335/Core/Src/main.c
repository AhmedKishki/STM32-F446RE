#include "clock.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "uart.h"



int main(void) {
    initCLK();
    initTIM();
    initLED();
    initUART();
    initADC();

    float values[3] = {0., 0., 0.};

    while (1){
		sendString("hello ADXL from UART!\r\n");
		msDelay(500);
		toggleLED();
		msDelay(500);
		readADC(values);
		printADC(values);
    }
}
