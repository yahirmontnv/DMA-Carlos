#ifndef USART_H
#define USART_H

#include "stm32f10x.h"

void USART1_DMA_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);

#endif