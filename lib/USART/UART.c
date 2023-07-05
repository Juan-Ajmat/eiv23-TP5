#include <stdint.h>
#include "UART.h"
#include "stm32f1xx.h"
#include <stdbool.h>


#define BAUDRATE 9600

void USART_init (void) {
 
    uint32_t const PA9_SALIDA_50MHZ_Msk = (0b1011 << GPIO_CRH_MODE9_Pos);
    uint32_t const PA10_ENTRADA_FLOTANTE_Msk = (0b0100 << GPIO_CRH_MODE10_Pos);
    SystemCoreClockUpdate();                                            //Revisa la configuracion de clock y suelta la frecuencia del mismo  
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;          //Activa el clock tanto del puerto A como del USART1
    GPIOA->CRH = (GPIOA->CRH & ~(0xF << GPIO_CRH_MODE9_Pos)) | PA9_SALIDA_50MHZ_Msk;      //Configura el pin 9 como un pin de salida rapida
    GPIOA->CRH = (GPIOA->CRH & ~(0xF << GPIO_CRH_MODE10_Pos)) | PA10_ENTRADA_FLOTANTE_Msk;//Configura el pin 10 como entrada flotante
    USART1->BRR = (SystemCoreClock )/ BAUDRATE;                         //Establece el valor del baudrate en el puerto comunicador serie 
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;          //Habilito el USART1 (CR1->UE = 1), el transmisor (CR1->TE = 1) y el receptor (CR1->RE = 1)                                       
}

bool USART_lectura (uint32_t *palabra) {
    if(USART1->SR & USART_SR_RXNE) {                        //Cuando llega un caracter lo coloca en el DR, y se pone en 1 el RXNE
        *palabra = USART1->DR;                                  //La palabra leida toma el valor de DR                                              
        return 1;
    }
    return 0;                                           
}

/*
uint32_t toString(int numero, char * char_n,uint32_t size ){
    int max=0;
    uint32_t temp;
    if (numero ==0) {
        char_n[0] = '0';
        char_n[1] = '\0';
        return 0;
    }
    for(uint32_t i = 0 ; i<=size ; i++){
        if (numero == 0) {
            max = i - 1;
            break;
        }
        char_n[i] = (char) (numero % 10 + 48);
        numero = numero/10;
    }
    for (int i = 0; i<=max/2 ;i++){
        temp = char_n[i];
        char_n[i] = char_n[max - i];
        char_n[max-i] = temp;
    }
    char_n[max + 1]='\0';    
    return max;
}

void USART_convertirACaracter(uint32_t numero){
    char numero_string[10];
    toString(numero,numero_string,10);
    USART_mostrarCaracter(numero_string);
}

void USART_mostrarCaracter(char * string){
    int i = 0;
    while (string[i]!='\0'){
        USART_escritura(string[i]);
        i++;
    }
}*/






bool USART_escritura (uint8_t c) {
    if (USART1->SR & USART_SR_TXE) {
        USART1->DR = c;                                          //Escribo un caracter
        return 1;
    }
    return 0;              
                                              
}


void transmitirConRetardo(char c){
    while (!USART_escritura(c)) {
    }
}


void USART_deInit () {
    while(!(USART1->SR & (USART_SR_TC))){
    }
    USART1->CR1 = 0;   
    RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN);
    }



 /*Ayuda de Nolosex*/


void USART_SendChar(uint8_t ch) {
    // Espera hasta que el registro de estado USART_SR indique que el registro de datos USART_DR está vacío
    while (!(USART1->SR & USART_SR_TXE));
    
    // Escribe el carácter en el registro de datos USART_DR
    USART1->DR = (ch & 0xFF);
    
    // Espera hasta que el registro de estado USART_SR indique que la transmisión ha finalizado
    while (!(USART1->SR & USART_SR_TC));
}


void USART_SendString(const char* str) {
    while (*str != '\0') {
        USART_SendChar(*str++);
    }
}

void USART_SendNumber(uint32_t num) {
    char buffer[20];
    char* ptr = buffer;
    
    if (num == 0) {
        USART_SendChar('0');
        return;
    }
    
    while (num > 0) {
        *ptr++ = '0' + (num % 10);
        num /= 10;
    }
    
    while (--ptr >= buffer) {
        USART_SendChar(*ptr);
    }
}


   