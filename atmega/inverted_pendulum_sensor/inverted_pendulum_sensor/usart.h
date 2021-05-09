/*
 * usart.h
 *
 * Created: 2021-05-09 20:25:37
 *  Author: westr
 */ 

#ifndef _USART_H_
#define _USART_H_

#include <avr/io.h>

/**
*	\brief Initiates USART with the baudrate 9600 baud, for a 8 MHz micro controller
*	with 8 bits and 1 stop bit. 
*	Receive on PD0, RXD0. Transmit on PD1, TXD0. (Atmega644PA, see datasheet for other MCUs)
**/
void usart_init();

/**
*	\brief Transmits one byte of information as soon as the buffer is ready.
*	\param tx_data is the data that should be sent through USART.
**/
void usart_transmit(uint8_t tx_data);
#endif