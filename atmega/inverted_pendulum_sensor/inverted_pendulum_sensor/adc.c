/*
 * adc.c
 *
 * Created: 2021-05-09 18:40:29
 *  Author: westr
 */ 

#include "adc.h"

void setup_adc(uint8_t single_ended_input){
	ADMUX &=  ~((1<<REFS0) | (1<<REFS1)); ///< AREF, Internal Vref turned off
	ADMUX |= single_ended_input;
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);	///< Analog-to-digital conversion enabled
}

void adc_prescaler(uint8_t PRESCALER){
	ADCSRA&= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADCSRA|=(PRESCALER);
}

void set_channel(uint8_t CHANNEL){
	ADMUX &=~(0b111);
	ADMUX |= CHANNEL;
}

void adc_interrupt_enabled(){
	ADCSRA |= (1<<ADIE);
}

void adc_interrupt_disbled(){
	ADCSRA &= ~(1<<ADIE);
}
void adc_freerun(){
	ADCSRA |= (1<<ADATE);
}
void adc_start(){
	ADCSRA |= (1<<ADSC);		///< Starting Analog-to-digital conversion
}