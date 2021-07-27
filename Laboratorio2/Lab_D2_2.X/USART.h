/* 
 * File:   USART.h
 * Author: DIEGO
 *
 * Created on 26 de julio de 2021
 */

#ifndef USART_H
#define	USART_H

#include <xc.h> 
#include <stdint.h>

void _baudios(void);
void config_tx(void);
void config_rc(void);
void Write_USART(uint8_t a);
void Write_USART_String(char *a);
uint8_t Read_USART(void);

#endif	/* USART_H */
