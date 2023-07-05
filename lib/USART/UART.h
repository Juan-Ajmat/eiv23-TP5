#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdbool.h>

void USART_init (void);

bool USART_lectura (uint32_t *palabra);

bool USART_escritura (uint8_t c);

void transmitirConRetardo(char c);

void USART_deInit ();

void USART_mostrarCaracter(char * string);

void USART_convertirACaracter(uint32_t numero);

uint32_t toString(int numero, char * char_n,uint32_t size );
/*Ayudanolosex*/

void USART_SendNumber(uint32_t num);
void USART_SendChar(uint8_t ch);
void USART_SendString(const char* str);
void USART_SendNumber(uint32_t num);


#endif