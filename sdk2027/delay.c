#include <stm32f10x.h>

void delay_ms(uint16_t t){
	volatile unsigned long j=0;
	for(uint16_t i = 0; i < t;i++)
	for(j = 0;j < 6000;j++){
	}
}