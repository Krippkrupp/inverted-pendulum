/*
 * inverted_pendulum_sensor.c
 *
 * Created: 2021-05-09 17:58:52
 * Author : westr
 */ 


#define		F_CPU		8000000UL
#define		H1_SENS		0
#define		H2_SENS		1


#define		ADC_SWITCH	2

		//		PID REGULATOR
#define		DELTA_TIME	0.0004			//		For prescaler 32
#define		P			1				//		Proportional value
#define		Ki			0				//		Integral	constant
#define		Kd			0				//		Derivative constant
#define		TARGET		700				//		Target value for degree = 0. Gör om till variabel så kan vi köra den också?

#define BACKWARDS 0
#define FORWARD (1<<7)


#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"

volatile int adc_val;
volatile int H1 = 0;
volatile int H2 = 0;

volatile int e1 = 0;	// Change start values???
volatile int e2 = 0;	// Change start values???

volatile int H1_prev = 0;
volatile int H2_prev = 0;

int e_val_1 = 0;
int e_val_2 = 0;

int speed = 0;
uint8_t motor_direction=0;

volatile uint8_t sensor = H1_SENS;
volatile uint8_t adc_switch_counter = 0;

// Functions, move later
double getDegree();

int main(void)
{
	//	ADC
	setup_adc(0);
	adc_interrupt_enabled();
	adc_freerun();
	adc_start();
	adc_prescaler(PRESCALER128);
	//	USART
	usart_init();
	sei();
	//	Remove later, for testing delta Time
	DDRD |= (1<<7);
	
	
    while (1) 
    {
		
	}
}

ISR(ADC_vect){
	adc_val = ADC;
	adc_switch_counter++;
	//	PORTD ^= (1<<7);							//	For testing deltaTime
	if(adc_switch_counter==ADC_SWITCH){
		
													//	Switch reading to other sensor
		if(sensor==H1_SENS){
			H1_prev = H1;
			H1 = adc_val;
			e1=TARGET-adc_val;						//	e_1(t)
			// Switch to read H2
			set_channel(ADC1);						//	Set ADC channel to PA1					
			sensor=H2_SENS;
			}else{
			H2_prev = H2;
			H2 = adc_val;
			e2=TARGET-adc_val;						// e_2(t)
			// Switch to read H1
			set_channel(ADC0);						//	Set ADC channel to PA0
			sensor=H1_SENS;							
		}
		adc_switch_counter=0;
		getDegree();
	}
	
}

/*
*	\brief Determines degree depending on which sensor has the most correct reading
*	at the current place in time.
*/
double getDegree(){	// Dåligt namn. Byt det ditt äckel
	if(H1>H2){										//	Leaning towards H2, need to drive forward, e1 should be used
		speed = -(P*e1+Ki*(H1-H1_prev)*DELTA_TIME+Kd*(H1-H1_prev)*DELTA_TIME);			//	Negative multiplication due to the nature of TARGET-adc_val
		motor_direction = FORWARD;
	}else{											//	Leaning towards H1, need to drive backwards, e2 should be used
		speed = -(P*e2+Ki*(H2-H2_prev)*DELTA_TIME+Kd*(H2-H2_prev)*DELTA_TIME);			//	Negative multiplication due to the nature of TARGET-adc_val
		motor_direction = BACKWARDS;
	}
	
	return 0;
}

v