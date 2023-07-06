#include <comandos.h>
#include <UART.h>
#include <ctype.h>
#include <stdbool.h>
#include <timer.h>
#include <stdio.h>
#include "soporte_placa.h"


//static const char* const comando = "dist?";
static const char* comando = "distancia";
//static const char* const mensaje_error = "ERR";

static bool controlDeComandos(char c);
void Interprete_procesa(char c);


typedef enum {
    COINCIDE,
    ERROR,
    ESPERA
}estado;

typedef struct comandos {
    estado status;
    int indice;
} comandos;


static comandos interprete; //Que es esto?

void Interprete_procesa(char c){
    switch(interprete.status){
    case ESPERA:
        interprete.indice = 0;
        if (c == ' ' || c == '\n' || c == '\r') {
            interprete.status = ESPERA;
        }
        else {
            if (controlDeComandos(c)) {
                interprete.status = COINCIDE;
            }
            else {
                interprete.status = ERROR;
            }
        }
    break;case COINCIDE:
        if (controlDeComandos(c)) {
            if (c == '\n') {
                MedirDistancia();
                interprete.status = ESPERA;
            }
        }
        else {
            interprete.status = ERROR;
        }
    break;case ERROR:
        if(c == '\n'){
            enviarError();
            interprete.status = ESPERA;
        }
    break;default:
        interprete.status = ERROR;
    }
}


/*Comparador de caracteres*/

static bool controlDeComandos(char c){

    int i = interprete.indice;
    if (comando[i] != 0) {
        if (tolower(c) == comando[i]) {
            interprete.indice++;
            return true;
        }
    }
    else {
        if (c == ' ' || c == '\n' || c == '\r') {
            return true;
        }
    }
    return false;
    // Si comando[i] != 0
    //  Compara tolower(c) con comando[i], si coincide avanza coincidencias y retorna true, si no retorna false
    // si comando[i] == 0
    //  Si c es espacio en blanco \n o \r retorna true, si no retorna false
}

void enviarError(void){
    USART_SendString("Error!!!\n");
}

