//LCD Functions Developed by electroSome
#include "LCD.h"

void Lcd_Port(char a) {
    PORTD = a;        //Para que tome el valor del puerto

}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        
        Lcd_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0xF0; //esta linea cambie para que vaya a la segunda linea
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Init(void) { //guiarse con el datasheet initalization 8bits
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30); //se cambia de 03 a 30 para iniciar en 8 bits
    __delay_ms(5);
    Lcd_Cmd(0x30); //se cambia de 03 a 30 para iniciar en 8 bits
    __delay_us(200);
    Lcd_Cmd(0x30); //se cambia de 03 a 30 para iniciar en 8 bits
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x3C); // Lines and character font
    //Lcd_Cmd(0x02); //
    Lcd_Cmd(0x08); // Display off
    //Lcd_Cmd(0x00); // 
    Lcd_Cmd(0x01); // Display clear
    //Lcd_Cmd(0x00); //
    Lcd_Cmd(0x06); //Entry mode set
}

void Lcd_Write_Char(char a) {
    char temp, y;
    temp = a;// & 0x0F; comento todo esto para que ya no haga los corrimientos
    y = temp;// & 0xF0;
    RS = 1;// => RS = 1 es para que lea caracteres
    Lcd_Port(y);// >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
//    Lcd_Port(temp);
//    EN = 1;
//    __delay_us(40);
//    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}