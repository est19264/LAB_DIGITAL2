/*
 * File:   Lab01.c 
 * Author: Diego
 * 
 */

//-------------------------- Bits de configuraciÓn -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 250000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  
#include "ADC.h"
#include "display7seg.h"

//------------------------------ Variables ------------------------------------- 
int multi;
int ADC;
int bandera;
char display;
uint8_t nibble2;
uint8_t nibble1;

//----------------------------- Prototipos ------------------------------------- 
void setup(void);
char nibbles_setup (char display);
void alarma(void);

//--------------------------- Interrupciones -----------------------------------
void __interrupt() isr(void)
{
     // Interrupcion del ADC
       if(PIR1bits.ADIF == 1)                   // Se revisa la bandera del ADC
       {
           if(ADCON0bits.CHS == 0) {            // Desplegar el canal 0 al portc
               ADC = ADRESH;
           }
           PIR1bits.ADIF = 0;                   // Se apaga la bandera del ADC
       }
    if(T0IF == 1)                               // Verifica la bandera del timer0
    {   
        RE0 = 0;
        RE1 = 0;
        if (bandera == 1){
            PORTEbits.RE0 = 1; 
            PORTD = tabla(nibble2);
            bandera = 0;
        }
        else {
            PORTEbits.RE1 = 1;
            PORTD = tabla(nibble1);
            bandera = 1;
        }
       
        INTCONbits.T0IF = 0;                    // Se Limpia la interrupcion del timer0
        TMR0 = 255;                             // Valor de reinicio del timer0
        
    }
    if (RBIF == 1)  // Verifica bandera de la interrupcion del puerto b
    {
        if (PORTBbits.RB0 == 0)                 // Si oprimo el boton 1
        {
            PORTC = PORTC + 1;                  // incrementa 1 al puerto
        }
        if  (PORTBbits.RB1 == 0)                // Se oprimo el boton 2
        {
            PORTC = PORTC - 1;                  // Decrementa 1 al puerto
        }
        INTCONbits.RBIF = 0;                    // Limpia la bandera de la interrupcion
    }
}

//---------------------------------- Main --------------------------------------
void main(void) {
    
    setup();                                    // Llama a la configuracion general

    while(1)    // Equivalete al loop
    {
        
        nibbles_setup(ADC);                     // Llama a la configuración del 7 segmentos
        if (GO == 0){
            __delay_us(100);
            GO = 1;
        }
        alarma();                               // Llama a la configuración de la alarma
    }
}

//------------------------------ Funciones -------------------------------------
// Función de configuraciones generales
void setup(void){
    bandera = 1;
    // Configuraciones de puertos digitales y analógicos
    ANSEL = 0b00000001;
    ANSELH = 0x00;
    
    // Configuraciones de bits de outputs o inputs
    // Input para el potenciómetro del puerto A
    TRISAbits.TRISA0 = 1;
    
    // Input para los botones y output para la alarma del puerto B
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB7 = 0;
    
    // Outputs para los otros puertos
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    // Se limpian los puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    // Configuración del oscilador
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;                       // Frecuencia de 250kHz
    OSCCONbits.SCS = 1;
    
    // Configuración del Timer0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    
    // Configuración de los pull-up internos
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    // Configuación de las interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.RBIF = 1;                        // Para que el boton funcione a la primera
    INTCONbits.RBIE = 1;                        // Interrupcion Puerto B
    INTCONbits.PEIE = 1;                        // Periferical interrupt
    INTCONbits.T0IF = 0; 
    PIE1bits.ADIE = 1;                          // Activar la interrupcion del ADC
    PIR1bits.ADIF = 0;                          // Bandera del ADC
    
    // Configuracion del ADC
    ADC_canal0();                               // Se manda a llamar de la libreria ADC

}

// Función para la configuración del cambio de nibble para el 7 seg
char nibbles_setup (char display){              // Funcion para recibir datos del ADC
    nibble1 = display&0x0F;                     // Paso a bits menos significativos
    nibble2 = (display>>4)&0x0F;                // paso a bits más significativos
    return nibble1, nibble2;
}

// Función para la configuración de la alarma
void alarma (void){
    if (ADC < PORTC){                           // Se verifica la desigualdad
        RB7 = 0;                                // Se apaga si es menor al valor de referencia
    }
    else {
        RB7 = 1;                                // Se enciende si es menor al valor de referencia
    }
}