#include <xc.h> 
#include <stdint.h>
#include "display7seg.h"


// Se crea la variable tab con los respectivos binarios para la conversión a hex
char tab[16] = {0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,
0b01101101,0b01111101,0b00000111,0b01111111,0b01101111,0b01110111,0b01111100,
0b00111001,0b01011110,0b01111001,0b01110001};

// Variable para guardar el resultado de la conversión
char trad;

// Función para la conversión binario-hexadecimal
char tabla(char hex){
    trad = tab[hex];
    return trad;
}