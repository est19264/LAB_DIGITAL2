/* 
 * File:   LCD.h
 * Author: diego
 *
 * Created on 25 de julio de 2021
 */
 #ifndef LCD
#define	LCD

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif
// Se redefinen los bits segun las conexiones del proyecto actual
#ifndef RS
#define RS PORTEbits.RE0 //se quita el PORTDbits para que no los tome todos en ese puerto
#endif

#ifndef EN
#define EN PORTEbits.RE2
#endif

#ifndef D0
#define D0 PORTDbits.RD0
#endif

#ifndef D1
#define D1 PORTDbits.RD1
#endif

#ifndef D2
#define D2 PORTDbits.RD2
#endif

#ifndef D3
#define D3 PORTDbits.RD3
#endif

#ifndef D4
#define D4 PORTDbits.RD4
#endif

#ifndef D5
#define D5 PORTDbits.RD5
#endif

#ifndef D6
#define D6 PORTDbits.RD6
#endif

#ifndef D7
#define D7 PORTDbits.RD7
#endif

#include <xc.h> // include processor files - each processor file is guarded.  

//LCD Functions Developed by electroSome


void Lcd_Port(char a);

void Lcd_Cmd(char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(char a, char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);

#endif