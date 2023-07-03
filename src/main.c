#include "UART.h"
#include "timer.h"
#include "stm32f1xx.h"
#include <soporte_placa.h>
#include <stdbool.h>
#include <stdint.h>
uint32_t buffer;

int main(){
    
    USART_init();
    TIM4_init();
    for(;;){
        bool read = USART_lectura(&buffer);
        if(read) {
            transmitirConRetardo(buffer);
            TIM4_pulso(2);
        }
    }
}