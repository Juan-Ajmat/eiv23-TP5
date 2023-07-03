#include "UART.h"
#include "timer.h"
#include "stm32f1xx.h"
#include <soporte_placa.h>
#include <stdbool.h>
#include <stdint.h>

int main(){
    uint32_t buffer;
    USART_init();
    for(;;){
        bool read = USART_lectura(&buffer);
        if(read) {
            transmitirConRetardo(buffer);
        }
    }
}