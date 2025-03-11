#include "clock.h"
#include "timer.h"
#include "led.h"

int main(void){
	initSysClock();
	initTIM6();
	initLED();
	while(1){
		toggleLED();
		msDelay(1000);
	}
}
