/*
 * inverted_pendulum_sensor.c
 *
 * Created: 2021-05-09 17:58:52
 * Author : westr
 */ 


#define F_CPU	8000000UL
#define H1_SENS 0
#define H2_SENS 1
#define TARGET 700
#define ADC_SWITCH 2
#define DELTA_TIME 0.0004			//		For prescaler 32
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"

volatile int adc_val;
volatile int H1 = 0;
volatile int H2 = 0;
volatile int H1_prev = 0;
volatile int H2_prev = 0;

int e_val_1 = 0;
int e_val_2 = 0;

volatile uint8_t sensor = H1_SENS;
volatile uint8_t adc_switch_counter = 0;

int main(void)
{
	//	ADC
	setup_adc(0);
	adc_interrupt_enabled();
	adc_freerun();
	adc_start();
	adc_prescaler(PRESCALER32);
	//	USART
	usart_init();
	
	//	Remove later, for testing delta Time
	DDRD |= (1<<7);
	
	sei();
	//sei();
    while (1) 
    {
		

    }
}

ISR(ADC_vect){
	adc_val = (ADCH<<8)| ADCL;
	adc_switch_counter++;
	PORTD ^= (1<<7);
	if(adc_switch_counter==ADC_SWITCH){
		
		//	Switch reading to other sensor
		if(sensor==H1_SENS){
			sensor=H2_SENS;
			H1=TARGET-adc_val;
			}else{
			sensor=H2_SENS;
			H2=TARGET-adc_val;
		}
		adc_switch_counter=0;
	}
}



