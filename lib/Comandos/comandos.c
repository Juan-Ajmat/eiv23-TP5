#include <comandos.h>
#include <UART.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

//static const char* const comando = "dist?";

//static const char* const mensaje_error = "ERR";

void mensajeError(void);
void mostrarDistancia (uint32_t ancho_de_pulso);

void mensajeError(void){
    printf("Objeto fuera de Rango!!\n");
}


void mostrarDistancia (uint32_t ancho_de_pulso){
    printf("El objeto se encuentra a %li \n",ancho_de_pulso);
}