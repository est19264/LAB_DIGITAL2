#include <xc.h> 
#include <stdint.h>  
#include "ADC.h"

#define _XTAL_FREQ 4000000

uint8_t configADC(uint8_t ch) {
    ADCON0bits.ADCS = 1;    // Conversion a FOSC/8
    ADCON1 = 0;
    
    switch (ch) {   //configura los pines en analogicos y seleciona el canal
        case 0:   // CANAL 0
            ANSELbits.ANS0 = 1;
            ADCON0bits.CHS = 0;
            break;
        case 1: // CANAL1
            ANSELbits.ANS1=1; 
            ADCON0bits.CHS=1; 
            break;
        case 2: // CANAL 2
            ANSELbits.ANS2=1; 
            ADCON0bits.CHS=2; 
            break;
        case 3: // CANAL 3
            ANSELbits.ANS3=1; 
            ADCON0bits.CHS=3; 
            break;
        case 4: // CANAL 4
            ANSELbits.ANS4=1; 
            ADCON0bits.CHS=4; 
            break;
        case 5: // CANAL 5
            ANSELbits.ANS5=1; 
            ADCON0bits.CHS=5; 
            break;
        case 6: // CANAL 6
            ANSELbits.ANS6=1; 
            ADCON0bits.CHS=6; 
            break;
        case 7: // CANAL 7 
            ANSELbits.ANS7=1; 
            ADCON0bits.CHS=7; 
            break;
        case 8: // CANAL 8
            ANSELHbits.ANS8=1; 
            ADCON0bits.CHS=8; 
            break;
        case 9: // CANAL 9
            ANSELHbits.ANS9=1; 
            ADCON0bits.CHS=9; 
            break;
        case 10: // CANAL 10
            ANSELHbits.ANS10=1; 
            ADCON0bits.CHS=10; 
            break;
        case 11: // CANAL 11
            ANSELHbits.ANS11=1; 
            ADCON0bits.CHS=11; 
            break;
        case 12: // CANAL 12
            ANSELHbits.ANS12=1; 
            ADCON0bits.CHS=12; 
            break;
        case 13: // CANAL 13
            ANSELHbits.ANS13=1; 
            ADCON0bits.CHS=13; 
            break;
        default: 
            return 0;
    }
 
    ADCON0bits.ADON = 1;    // ADC esta activiado
    __delay_ms(0.25);
    ADCON0bits.GO = 1; // conversion A/D esta en proceso
    
    resul:
    if (ADCON0bits.GO_DONE == 1){
        goto resul; 
    }     
    else{
        ADCON0bits.ADON = 0;
        return ADRESH;  //retorna el valor ADRESH
    }
}