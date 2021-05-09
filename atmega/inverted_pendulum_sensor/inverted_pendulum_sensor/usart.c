/*
 * usart.c
 *
 * Created: 2021-05-09 20:25:31
 *  Author: westr
 */ 

#include "usart.h"

void usart_init()
{
	UBRR0 = 51; // 9600=103 baud @ 16 MHz, 51=9600 @ 8 MHz
	//UBRR0L = 51;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);// | (1 << RXCIE0);		//	Enable RX, TX
	//UCSR0C &= ~(1<<USBS0);									//	1 stop bit
	//UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);						//	8 bits
	//UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);						//	8 bits
	//UCSR0C |= (3<<UCSZ00);
}

void transmit_speed(uint16_t speed)
{
	uint8_t tmp = (speed>>8);
	usart_transmit(tmp);
	tmp = speed;
	usart_transmit(tmp);
}

void usart_transmit(uint8_t tx_data)
{
	while(!(UCSR0A & (1<<UDRE0))); //Buffer upptagen
	UDR0 = tx_data;
}