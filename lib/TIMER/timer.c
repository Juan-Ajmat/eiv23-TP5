#include "timer.h"
#include <stdint.h>
#include <stm32f1xx.h>

#define PREESCALER 46UL 


typedef enum TIM_MODO{
    MODO_FROZEN            ,
    MODO_HIGH_ON_MATCH   ,
    MODO_LOW_ON_MATCH ,
    MODO_TOGGLE_ON_MATCH   ,
    MODO_CERO              ,
    MODO_UNO               ,
    MODO_PWM_1             ,
    MODO_PWM_2             
}TIM_MODO;

void TIM4_init (){
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //Habilito el clock del timer 4  
    TIM4_reset(); 
    TIM4->PSC = PREESCALER;                 //Configuro el tiempo del prescaler
    TIM4->CR1 |= TIM_CR1_CEN;               //Habilito el contador
    TIM4_setOC1M(MODO_CERO);
    TIM4_CH2_config();                    
    TIM4->CCER |= TIM_CCER_CC1E;            //Habilito modo Capture/Compare en el canal 1 del timer 4
    TIM4->CCER |= TIM_CCER_CC2E;            //Habilito modo Capture/Compare en el canal 2 del timer 4                         
    pinConfig ();
}

 


static void TIM4_reset () {
    RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
    RCC->APB1RSTR = 0; 
}

static void TIM4_CH2_config (void) {
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_CC2S)) | (0b01 << TIM_CCMR1_CC2S_Pos); //Configuro el canal 2 del timer 4 como entrada
}

static void TIM4_setOC1M(TIM_MODO modo){
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_OC1M)) | (modo << TIM_CCMR1_OC1M_Pos);
}

/*Una rutina que inicia el timer puede iniciar los canales y dejarlos en un estado conocido. 
El trigger queda en comparacionpero con forzadoa a 0. Para dispara forzar a 1 
reiniciar contador y preescaler
en el registro de generacion de eventos, escribimos 1, y reinicia el contrador
config el canal como forzado en alto y despues cambiamos la config para q vaya a 0 con una cuenta de 2

o samos la interrupcion, primero genero el disparo y con otra rutina, configuro el canal para que capture el flanco ascendete
una vez guardado el valor lo configuran para tomar el flanco descendente y hace la diferencia 
entre el valor primero y el segundo

lazo de espera/bloqueo


*/
