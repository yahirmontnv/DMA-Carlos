#include "dma.h"

/* Variables ubicadas en la memoria SRAM donde el DMA escribirá directamente sin usar el CPU */
volatile uint16_t adc_mq2_mem = 0;
volatile uint8_t  rx_terminal_mem[TAM_BUFFER_TERMINAL] = {0};

void DMA1_Config_Init(void) {
    /* 1. Habilitar el reloj del controlador DMA1 en el bus AHB (Bit 0: DMA1EN) */
    RCC->AHBENR |= (1 << 0);

    /* =========================================================================
     * CANAL 5 DEL DMA1: Terminal Serial (USART1_RX) -> Memoria SRAM (Sin CPU)
     * ========================================================================= */
    DMA1_Channel5->CCR &= ~(1 << 0);                         // Asegurar que el Canal 5 esté deshabilitado antes de configurar
    DMA1_Channel5->CPAR = (uint32_t)&(USART1->DR);           // Dirección origen: Registro de datos de USART1 (0x40013804)
    DMA1_Channel5->CMAR = (uint32_t)rx_terminal_mem;         // Dirección destino: Arreglo en memoria SRAM
    DMA1_Channel5->CNDTR = TAM_BUFFER_TERMINAL;              // Cantidad de bytes a transferir (16 bytes)

    /* Configuración de DMA1_Channel5->CCR:
     * - Bit 4  (DIR)   = 0 : Dirección Periférico -> Memoria (de USART1 a RAM)
     * - Bit 5  (CIRC)  = 1 : Modo circular (al llenar los 16 bytes, vuelve al inicio automáticamente)
     * - Bit 6  (PINC)  = 0 : No incrementar dirección del periférico (siempre lee de USART1->DR)
     * - Bit 7  (MINC)  = 1 : Incrementar dirección de memoria (avanza en rx_terminal_mem[0], [1], [2]...)
     * - Bits 9:8 (PSIZE) = 00: Tamaño de dato del periférico = 8 bits (1 carácter ASCII)
     * - Bits 11:10 (MSIZE)= 00: Tamaño de dato en memoria = 8 bits
     * - Bits 13:12 (PL)  = 10: Prioridad Alta
     * - Bit 0  (EN)    = 1 : Habilitar el Canal 5 del DMA1
     */
    DMA1_Channel5->CCR = (1 << 5) | (1 << 7) | (2 << 12) | (1 << 0);

    /* =========================================================================
     * CANAL 1 DEL DMA1: Sensor MQ-2 (ADC1) -> Memoria SRAM (Sin CPU)
     * ========================================================================= */
    DMA1_Channel1->CCR &= ~(1 << 0);                         // Deshabilitar Canal 1 antes de configurar
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);             // Dirección origen: Registro de datos de ADC1 (0x4001244C)
    DMA1_Channel1->CMAR = (uint32_t)&adc_mq2_mem;            // Dirección destino: Variable en memoria SRAM
    DMA1_Channel1->CNDTR = 1;                                // 1 transferencia continua de 16 bits

    /* Configuración de DMA1_Channel1->CCR:
     * - Bit 4  (DIR)   = 0 : Periférico -> Memoria (de ADC1 a RAM)
     * - Bit 5  (CIRC)  = 1 : Modo circular continuo
     * - Bit 6  (PINC)  = 0 : Dirección periférica fija
     * - Bit 7  (MINC)  = 0 : Dirección de memoria fija (una sola variable)
     * - Bits 9:8 (PSIZE) = 01: Tamaño de periférico = 16 bits
     * - Bits 11:10 (MSIZE)= 01: Tamaño en memoria = 16 bits
     * - Bit 0  (EN)    = 1 : Habilitar el Canal 1 del DMA1
     */
    DMA1_Channel1->CCR = (1 << 5) | (1 << 8) | (1 << 10) | (1 << 0);
}