#ifndef DMA_H
#define DMA_H

#include "stm32f10x.h"

#define TAM_BUFFER_TERMINAL  16   // Cantidad de bytes a guardar desde la terminal en RAM

extern volatile uint16_t adc_mq2_mem;                        // Dato del ADC en RAM (vía DMA1_Channel1)
extern volatile uint8_t  rx_terminal_mem[TAM_BUFFER_TERMINAL]; // Datos de la terminal en RAM (vía DMA1_Channel5)

void DMA1_Config_Init(void);

#endif