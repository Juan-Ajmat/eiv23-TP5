#include <comandos.h>
#include <UART.h>
#include <ctype.h>
#include <stdbool.h>


static const char* const comando = "dist?";

static const char* const mensaje_error = "ERR";

typedef struct {
    enum Estado { ESPERA, COINCIDIENDO, ERR } estado;
    unsigned coincidencias;
} Interprete;

static Interprete interprete;

static void transmitirMensajeDeError(void) {
    for (unsigned i = 0; mensaje_error[i] != '\0'; i++) {
        esperarYTransmitir(mensaje_error[i]);
    }
}

void Interprete_init(void) {
    interprete.coincidencias = 0;
    interprete.estado = ESPERA;
}

static bool coincideChar(char c) {
    unsigned i = interprete.coincidencias;
    if (comando[i] != '\0') {
        if (tolower(c) == comando[i]) {
            interprete.coincidencias++;
            return true;
        }
    } else {
        if (c == ' ' || c == '\n' || c == '\r') {
            return true;
        }
    }
    return false;
}

void Interprete_procesa(char c) {
    switch (interprete.estado) {
        case ESPERA:
            interprete.coincidencias = 0;
            if (c == ' ' || c == '\n' || c == '\r') {
                interprete.estado = ESPERA;
            } else if (coincideChar(c)) {
                interprete.estado = COINCIDIENDO;
            } else {
                interprete.estado = ERR;
            }
            break;

        case COINCIDIENDO:
            if (coincideChar(c)) {
                if (c == '\n') {
                    Comando_dist();
                    interprete.estado = ESPERA;
                }
            } else {
                interprete.estado = ERR;
            }
            break;

        case ERR:
            if (c == '\n') {
                transmitirMensajeDeError();
                interprete.estado = ESPERA;
            }
            break;
    }
}