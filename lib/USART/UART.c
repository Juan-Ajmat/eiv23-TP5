#include <stdint.h>
#include <UART.h>
#include <stm32f1xx.h>

static void USART_reset () {
    RCC->APB2RSTR = RCC_APB2RSTR_USART1RST;                                       
    RCC->APB2RSTR = 0;                                          
}
void USART_init (uint32_t baudrate){
    
}