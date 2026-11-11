#include "usart.h"

void USART1_DMA_Init(void) {
    /* 1. Habilitar relojes para AFIO (bit 0), GPIOA (bit 2) y USART1 (bit 14) en APB2 */
    RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 14);

    /* 2. Configurar PA9 (TX) como Salida Alternativa Push-Pull 50MHz (0xB)
     *    y PA10 (RX) como Entrada Flotante (0x4) en GPIOA->CRH */
    GPIOA->CRH &= ~((0xF << 4) | (0xF << 8));
    GPIOA->CRH |=  ((0xB << 4) | (0x4 << 8));

    /* 3. Configurar Baud Rate a 9600 bps:
     *    - Si tu SystemInit() corre a 72 MHz: BRR = 72,000,000 / 9600 = 7500 (0x1D4C)
     *    - (Nota: Si tu proyecto corre a 8 MHz HSI, cambia 0x1D4C por 0x0341)
     */
    USART1->BRR = 0x1D4C;

    /* 4. ¡PASO CLAVE! Habilitar DMA en Recepción (Bit 6: DMAR en USART1->CR3)
     *    Con esto, cada byte enviado desde la terminal dispara el DMA1_Channel5 hacia la RAM. */
    USART1->CR3 |= (1 << 6);

    /* 5. Habilitar USART1 (UE = bit 13), Transmisor (TE = bit 3) y Receptor (RE = bit 2) */
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & (1 << 7))); // Esperar a que TXE (Bit 7) sea 1
    USART1->DR = c;
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}