#include <stm32f10x.h>
#include "leds.h"

/*
	Select PA0 as the input for external interrupt.
	On each rising edge, an interrupt is generated.
	Interrupt service routine toggles Led on PC13(IRQHandler).
*/
volatile int8_t int0_flag = 0;

void EXTI0_IRQHandler(){
	EXTI->PR |= (1 << 0); //clear pending flag
	int0_flag = 1;
	led_on_off();
	}

void init_exti_pin(void){
	RCC->APB2ENR |= ((1 << 2) | (1 << 0));	//GPIOA clock enable & AFIO	
	GPIOA->CRL &= ~((1 << 2) | (1 << 1) | (1 << 0));	//input mode reset
	GPIOA->CRL |= (1 << 3);	//PA0 pin as input pull up
	GPIOB->ODR |= (1 << 0);	//activate PA0 pull up
}
	
void init_exti(void){
/*Note 1
	EXTICR is defined as array structure EXTICR[4]
	so 	EXTICR[0] <=> EXTICR1 in reference manual
			EXTICR[1] <=> EXTICR2 in reference manual
			EXTICR[2] <=> EXTICR3 in reference manual
			EXTICR[3] <=> EXTICR4 in reference manual	*/
/*Note 2
	Select pin for external interrupt, example PA0
	identify (pag 210 RM0008) external interrupt line (EXTI0 for PA0)
	and register numer EXTICR1 for EXTI0
	so we have to use AFIO->EXTICR[0] at programming level*/
	
	//Configure EXTI******************
	//select PA0 source input for EXTI0 (pag 191 RM0008)
	AFIO->EXTICR[0] &= ~((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
	//select trigger on falling edge (pag 212 RM0008)
	EXTI->FTSR |= (1 << 0);
	//enable EXTI0 
	EXTI->IMR |= (1 << 0);
	//clear pending flag
	EXTI->PR |= (1 << 0);	
	
	//enable the interrupt line at NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
}