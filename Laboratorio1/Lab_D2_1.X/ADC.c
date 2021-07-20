#include <xc.h> 
#include <stdint.h> 
#include <pic16f887.h>
#include "ADC.h"

void ADC_canal0(void)
{
    ADCON0bits.CHS = 0;         // Canal 
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1; 
    ADCON0bits.ADON = 1; 
    
    ADCON1bits.ADFM = 0;        // Justificacion a la izquierda
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;       //Vss y Vcc
}