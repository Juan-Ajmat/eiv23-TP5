#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdbool.h>

void USART_init (void);

bool USART_lectura (uint32_t *palabra);

bool USART_escritura (uint8_t c);

void transmitirConRetardo(char c);

void USART_deInit ();

#endif