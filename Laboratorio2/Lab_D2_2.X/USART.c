#include <xc.h>
#include "USART.h"

void conf_usart(void){
//configurar transmisor y receptor asincrono
    SPBRG = 103;         //para el baud rate de 9600
    SPBRGH = 0;
    BAUDCTLbits.BRG16 = 1; //8bits baud rate generator is used
    TXSTAbits.BRGH = 1; //high speed
    
    TXSTAbits.SYNC = 0; //asincrono
    //serial port enabled (Configures RX/DT and TX/CK pins as serial)
    RCSTAbits.SPEN = 1; 
    RCSTAbits.CREN = 1; //habilitar la recepcion
    
    TXSTAbits.TX9 = 0; //transmision de 8bits
    TXSTAbits.TXEN = 1; //enable the transmission
    RCSTAbits.RX9 = 0; //recepcion de 8 bits
      
    //PIE1bits.TXIE = 1; //porque quiero las interrupciones de la transmision
    INTCONbits.GIE = 1; //enable de global interrupts
    INTCONbits.PEIE = 1;
    //PIE1bits.RCIE = 1; //interrupciones del receptor
    PIR1bits.TXIF = 0;  //limpiar interrupciones
    PIR1bits.RCIF = 0;
    
    return;
}