/*
 * File:   lab2_main.c
 * Author: diego
 *
 * Created on 24 de julio de 2021, 11:22 PM
 */

//-------------------------- Bits de configuraciÓn -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                   // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                  // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                  // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                     // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                    // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                  // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                   // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF                  // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                    // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V               // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                    // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h> 
#include <stdlib.h>
#include <pic16f887.h> 
#define _XTAL_FREQ 4000000

#include "USART.h"                          // Libreria para el USART
#include "ADC.h"                            // Libreria para el ADC
#include "LCD.h"                            // Libreria para el LCD


//--------------------------------- Variables ----------------------------------

char V1, V2;
char dividendo, centenas, residuo, decenas, unidades;
char val1;
char val;                                  
char contador = 0;

//-------------------------------- Prototipos ----------------------------------
void setup(void);
char division (char dividendo);
void setup(void);                           // Definición de funciones
void putch(char data);                      // Funcion para el valor a transmitir
void text(void);                            // Función las cadenas de texto

//------------------------------ Interrupciones --------------------------------

void __interrupt() isr(void){
    //interrupcion del ADC
    if (ADIF){
        if(ADCON0bits.CHS == 0){
            ADCON0bits.CHS = 1;
            V1 = ADRESH;
        }
        else if(ADCON0bits.CHS == 1){
            ADCON0bits.CHS = 0;
            V2 = ADRESH; 
        }
    }
    ADIF = 0;
}

//----------------------------------- Main -------------------------------------
void main(void){
    setup();
    conf_usart();
    Lcd_Init();                             // LCD esta apagada
    Lcd_Clear();                            // Se limpia la LCD
    char buffer[20];                        // Se guarda el voltaje en un string
    char buffer1[20];
    char val1;
    char val;                               // Valor que deseo almacenar en un str
    Lcd_Set_Cursor(1,1);                    // Primera linea en la posicion 1
    Lcd_Write_String("S1:  S2:  S3:");      // Se imprimen los indicadores de voltaje
    
    
    
    while(1){   
   
    val = V1*0.0196; 
    val1 = V2*0.0196;
    sprintf(buffer, "%d V   ", val); 
    sprintf(buffer1, "%d V ", val1);
    
    Lcd_Set_Cursor(2,2);                    // Segunda linea
    Lcd_Write_String(buffer);               // Mostrar el valor del string
    Lcd_Write_String(buffer1);
    Lcd_Write_String('  ');    
    
    __delay_ms(1000);                       // Delay para que la busy flag permita recibir
         
    if (ADCON0bits.GO == 0){                // Ee apaga 
            __delay_us(100);                // Se enciende
            ADCON0bits.GO = 1;
        }
    

    // Se despliega el valor del pot 1
    division(V1);
    __delay_ms(250);                        // Tiempo para el despliegue de los caracteres
    printf("Valor de voltaje del POT1:\r");
    __delay_ms(250);
    TXREG = centenas;
    __delay_ms(250);
    TXREG = 46;
    __delay_ms(250);
    TXREG = decenas;
    __delay_ms(250);
    TXREG = unidades;
    __delay_ms(250);
    printf("\r");
    
    // Se despliega el valor del pot 2
    division(V2);
    __delay_ms(250);                        //Tiempos para el despliegue de los caracteres
    printf("Valor de voltaje del POT2:\r");
    __delay_ms(250);
    TXREG = centenas;
    __delay_ms(250);
    TXREG = 46;
    __delay_ms(250);
    TXREG = decenas;
    __delay_ms(250);
    TXREG = unidades;
    __delay_ms(250);
    printf("\r");
    
    division(contador);
    Lcd_Write_Char(centenas);
    Lcd_Write_Char(decenas);
    Lcd_Write_Char(unidades);
    
    // Se pregunta si se quiere aumentar o disminuir el contador
    __delay_ms(250);
    printf("\r + para umento del contador \r");
    __delay_ms(250);
    printf("\r - para decremento del contador\r");
    __delay_ms(250);
    printf("\r * para sber el voltaje de los pots\r");
    while(RCIF == 0);
    char answer = RCREG;
    if (answer == '+'){
        contador = contador + 1;
    }
    else if (answer == '-'){
        contador = contador - 1;
    }
    else if (answer == '*'){
        NULL;
    }
    }
        
    return;
}

//--------------------------------- Funciones ----------------------------------

void putch(char data){                      // Funcion de stdio.h
    while(TXIF == 0);
    TXREG = data;                           
    return;
}

char division (char valor){
    centenas = valor/100;                   // Para las centenas
    residuo = valor%100;                    // Para las decenas
    decenas = residuo/10; 
    unidades = residuo%10;                  // Para las unidades
    centenas = centenas + 48;
    decenas = decenas + 48;
    unidades = unidades + 48;
} 

//------------------------------ Configuraciones -------------------------------
void setup(void){
    // Configuracion de pines analógicos
    ANSEL = 0b00000011;
    
    // Configuración de inputs 
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    
    // Outputs para controlar LCD
    TRISD = 0x00;
    TRISE = 0x00;
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 0;                   // 4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;                     
    
    //configuracion de interrupciones
    PIE1bits.ADIE = 1;                      // Habilita de la int del ADC
    PIR1bits.ADIF = 0;                      // Limpia la interrupcion del ADC
    INTCONbits.GIE = 1;                     // Habilita las interrupciones globales
    INTCONbits.PEIE = 1;                    // Habilita las periferical interrupts
    PIE1bits.RCIE = 0;                      // Interrupcion rx
    PIE1bits.TXIE = 0;                      // Interrupcion TX
    
    // Se limpian los puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    // Configuracion de ADC
    config_ADC(1);
    
    // Configuraciones TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 25;
    SPBRGH = 1;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TX9 = 0;                      // 8 bits
    TXSTAbits.TXEN = 1;
}