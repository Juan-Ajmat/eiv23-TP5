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
void TIM4_init(void);

void TIM4_pulso(uint32_t ciclos);

void TIM4_deInit(void);

void TIM4_setOC1M(int modo);

void MedirDistancia(void);



#endif