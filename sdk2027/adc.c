#include "adc.h"

void ADC1_DMA_Init(void) {
    /* 1. Habilitar reloj para GPIOA (bit 2) y ADC1 (bit 9) en APB2 */
    RCC->APB2ENR |= (1 << 2) | (1 << 9);

    /* 2. Preescalador del reloj del ADC: PCLK2 dividido entre 6 (72MHz/6 = 12MHz, debe ser <= 14MHz) */
    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |=  (2 << 14);

    /* 3. Configurar PA0 (Canal 0 del ADC1) como entrada analógica (MODE0 = 00, CNF0 = 00) */
    GPIOA->CRL &= ~(0xF << 0);

    /* 4. Configurar tiempo de muestreo del Canal 0 a 239.5 ciclos (111 en SMP0) para estabilidad con el MQ-2 */
    ADC1->SMPR2 |= (7 << 0);

    /* 5. Seleccionar el Canal 0 (PA0) como primera conversión en la secuencia regular */
    ADC1->SQR1 = 0;          // 1 sola conversión en la secuencia (L = 0)
    ADC1->SQR3 = 0;          // Canal 0 en SQ1

    /* 6. Configurar ADC1->CR2:
     *    Bit 0 (ADON) = 1: Encender el ADC
     *    Bit 1 (CONT) = 1: Modo de conversión continua
     *    Bit 8 (DMA)  = 1: Habilitar acceso directo a memoria (DMA) para que el ADC mande el dato a RAM solo
     *    Bit 20 (EXTTRIG) y Bits 19:17 (EXTSEL = 111): Disparo por software (SWSTART)
     */
    ADC1->CR2 |= (1 << 0) | (1 << 1) | (1 << 8) | (1 << 20) | (7 << 17);

    /* 7. Calibración obligatoria del ADC1 para lecturas de voltaje precisas */
    ADC1->CR2 |= (1 << 3);               // Iniciar reset de calibración (RSTCAL)
    while (ADC1->CR2 & (1 << 3));        // Esperar a que termine

    ADC1->CR2 |= (1 << 2);               // Iniciar calibración (CAL)
    while (ADC1->CR2 & (1 << 2));        // Esperar a que termine la calibración

    /* 8. Iniciar las conversiones continuas del ADC1 */
    ADC1->CR2 |= (1 << 22);              // Bit 22: SWSTART = 1 (o segundo encendido de ADON)
}