#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include "soporte_placa.h"

typedef enum{
    ASCENDENTE,
    DESCENDENTE
}flanco;

/**
 * @brief Inicializa el Timer 4...
 */
void TIM4_init();

void TIM4_pulso (uint32_t ciclos);

void TIM4_IRQHandler (void);

void TIM4_deInit ();
#endif