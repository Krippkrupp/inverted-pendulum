/*
 * timer0.c
 *
 * Created: 2021-05-09 18:30:14
 *  Author: westr
 */ 


#include "timer0.h"

void timer0_setup(){
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0); // Clear OC0A on Compare Match
	
	
}