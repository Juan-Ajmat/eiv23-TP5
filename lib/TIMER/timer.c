#include "timer.h"
#include <stdint.h>
#include <stm32f1xx.h>
#include "soporte_placa.h"

#define PREESCALER 46UL  
uint32_t valor_detectado;

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


static void TIM4_resetCounterAndUpdate(void);
static void TIM4_reset ();
static void TIM4_setOC1M(TIM_MODO modo);
static void pinConfig (void);
static void TIM4_setCCR1(uint16_t valor);
static void TIM4_resetCounterAndUpdate(void);
static void TIM4_CC2S_config (void);

static void TIM4_reset () {
    RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
    RCC->APB1RSTR = 0; 
}

static void TIM4_CC2S_config (void) {
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_CC2S)) | (0b01 << TIM_CCMR1_CC2S_Pos); //Configuro el canal 2 del timer 4 como entrada
}

static void TIM4_setOC1M(TIM_MODO modo){
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_OC1M)) | (modo << TIM_CCMR1_OC1M_Pos);
}

static void pinConfig (void) {
    SP_Pin_setModo(SP_PB6,SP_PIN_SALIDA_AF);
    SP_Pin_setModo(SP_PB7,SP_PIN_ENTRADA);
}

void TIM4_pulso (uint32_t ciclos){
    TIM4_resetCounterAndUpdate();
    TIM4_setOC1M(MODO_UNO);
    TIM4_setCCR1(TIM4->CNT + ciclos);
    TIM4_setOC1M(MODO_LOW_ON_MATCH);
}

void TIM4_IRQHandler (void) {
    if ((TIM4->SR & TIM_SR_CC2IF))
    {
    valor_detectado = TIM4->CCR2;
    }
}

static void TIM4_setCCR1(uint16_t valor){
    TIM4->CCR1 = valor;
}

static void TIM4_resetCounterAndUpdate(void){
    TIM4->EGR = TIM_EGR_UG;
}

void TIM4_deInit (){
    TIM4_reset();   
    RCC->APB1ENR &= ~(RCC_APB1ENR_TIM4EN);  //Deshabilito el clock del Timer 2
}

void TIM4_init (){
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //Habilito el clock del timer 4  
    TIM4_deInit(); 
    TIM4->PSC = PREESCALER;                 //Configuro el tiempo del prescaler
    TIM4->CR1 |= TIM_CR1_CEN;               //Habilito el contador
    TIM4_setOC1M(MODO_CERO);
    TIM4_CC2S_config();                    
    TIM4->CCER |= TIM_CCER_CC1E;            //Habilito modo Capture/Compare en el canal 1 del timer 4
    TIM4->CCER |= TIM_CCER_CC2E;            //Habilito modo Capture/Compare en el canal 2 del timer 4                         
    pinConfig ();
    TIM4->DIER |= TIM_DIER_CC2IE;
    __NVIC_EnableIRQ(TIM4_IRQn);
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
