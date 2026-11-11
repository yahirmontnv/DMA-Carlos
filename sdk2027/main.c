#include "stm32f10x.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "delay.h"   // O usa el retardo incluido abajo si tu delay.h tiene otro prototipo

/* Función auxiliar para convertir e imprimir el voltaje del MQ-2 en la terminal */
void Mostrar_Voltaje_MQ2(uint16_t valor_adc) {
    /* Cálculo en milivolts: mV = (valor_adc * 3300) / 4095 */
    uint32_t milivolts = ((uint32_t)valor_adc * 3300) / 4095;
    uint32_t entero    = milivolts / 1000;          // Parte entera (0 a 3 V)
    uint32_t decimal   = (milivolts % 1000) / 10;   // Dos decimales (00 a 99)

    USART1_SendString("MQ2 ADC: ");
    
    /* Imprimir valor crudo del ADC (4 dígitos: 0000 - 4095) */
    USART1_SendChar((valor_adc / 1000) + '0');
    USART1_SendChar(((valor_adc / 100) % 10) + '0');
    USART1_SendChar(((valor_adc / 10) % 10) + '0');
    USART1_SendChar((valor_adc % 10) + '0');

    /* Imprimir el voltaje formateado (Ej. " | Voltaje: 1.45 V") */
    USART1_SendString(" | Voltaje: ");
    USART1_SendChar(entero + '0');
    USART1_SendChar('.');
    USART1_SendChar((decimal / 10) + '0');
    USART1_SendChar((decimal % 10) + '0');
    USART1_SendString(" V\r\n");
}

int main(void) {
    /* 1. Inicializar DMA1 (Canal 1 para ADC y Canal 5 para Terminal RX -> RAM) */
    DMA1_Config_Init();

    /* 2. Inicializar USART1 con petición DMA en RX activada */
    USART1_DMA_Init();

    /* 3. Inicializar ADC1 en PA0 con petición DMA activada */
    ADC1_DMA_Init();

    USART1_SendString("=== Monitoreo MQ-2 y Recepcion DMA Listos ===\r\n");

    while (1) {
        /* 
         * Nota como en este ciclo NUNCA leemos USART1->DR ni ADC1->DR con el CPU:
         * - Lo que escribas en la terminal se guarda solo en rx_terminal_mem[] vía DMA1_Channel5.
         * - El valor del sensor MQ-2 se actualiza solo en adc_mq2_mem vía DMA1_Channel1.
         */
        Mostrar_Voltaje_MQ2(adc_mq2_mem);

        /* Retardo aproximado de ~500 ms entre impresiones en la terminal */
        for (volatile int i = 0; i < 4000000; i++);
    }
}