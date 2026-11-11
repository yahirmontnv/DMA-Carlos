#include <stm32f10x.h>
#include "delay.h"

void init_led_pin(void){
	RCC->APB2ENR |= (1 << 4); //GPIOC clock enable
	GPIOC->CRH &= ~((1 << 23) | (1 << 22) | (1 << 20)); //general purpose output push-pull
	GPIOC->CRH |= (1 << 21); //2 Mhz
}

void led_on_off(void){
	for (int x=0; x<5; x++){
	GPIOC ->ODR |= 1 << 13;
	delay_ms(200);
	GPIOC ->ODR &= ~(1 << 13);
	delay_ms(200);
	}
}

