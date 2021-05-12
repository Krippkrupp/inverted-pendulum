/*
 * inverted_pendulum_sensor.c
 *
 * Created: 2021-05-09 17:58:52
 * Author : westr
 */ 


#define		F_CPU		16000000UL
#define		H1_SENS		0
#define		H2_SENS		1


#define		ADC_SWITCH	2

#define		NOTHING_VAL 0				//		Don tmove if e1 and e2 are less than this, likely an error 

		//		PID REGULATOR
#define		DELTA_TIME	0.0004			//		For prescaler 32
#define		P			30				//		Proportional value
#define		Ki			0.0000001				//		Integral	constant
#define		Kd			20				//		Derivative constant
//#define		TARGET_1	698				//		Target value for degree = 0. Gör om till variabel så kan vi köra den också?
//#define		TARGET_2	712

#define		MOVING_AVARAGE		60	//10 tidigare, funkar

#define BACKWARDS 0
#define FORWARD (1<<7)

#define RASPBERRY_PI_DELAY 1	//Nej: 50, 100, 250 Ja(?): 500, 400 ja, 350 verkar bra

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"



volatile int adc_val;
volatile int H1 = 0;
volatile int H2 = 0;
volatile uint16_t buffer = 0;

volatile int e1 = 0;	// Change start values???
volatile int e2 = 0;	// Change start values???

volatile int H1_prev = 0;
volatile int H2_prev = 0;

volatile uint8_t moving_avarage_counter = 0;

int H1_MAX, H1_MIN, H2_MAX;
int H2_MIN;
int TARGET_1, TARGET_2;



int e_val_1 = 0;
int e_val_2 = 0;

volatile uint8_t prev_transmit;

//volatile uint16_t speed = 0;
volatile float speed = 0;
uint8_t motor_direction=0;

volatile uint8_t sensor = H1_SENS;
volatile uint8_t adc_switch_counter = 0;

volatile uint16_t transmit_counter = 0; /// Test

int max_speed_e1, max_speed_e2;

// Functions, move later
void getDegree();
void send_motorspeed();
void startSeq();				//	Starting sequence... skönt att vi inte valde rotationsenkoder
void reset_counters();

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
	cli();
	
	DDRD |= (1<<7);	//	Remove later, for testing delta Time
	
	startSeq();
	
	//	REMOVE LATER
	
    while (1) 
    {
		PORTD ^= (1<<PORTD7);
		//usart_transmit(50);
	}
}

ISR(ADC_vect){
	
	adc_val = ADC;
	adc_switch_counter++;
	//	PORTD ^= (1<<7);							//	For testing deltaTime
	if(adc_switch_counter<ADC_SWITCH){				//	Read twice after change
		return;
		}
	
	
	if(transmit_counter<=RASPBERRY_PI_DELAY){ // 250 tidigare
		transmit_counter++;
		return;}
	
		
	
	/*
	uint8_t tmp = adc_val-TARGET_1;	// REMOVE LATER; 
	usart_transmit(tmp);
	return;
	*/
	moving_avarage_counter++;											
	if(sensor==H1_SENS){
		buffer+=adc_val;						//	Max value 65535 for uint16_t, FIX quit if buffer+adc_val>65535?
		if(moving_avarage_counter<MOVING_AVARAGE){
			return;
		}else{
			H1_prev = H1;
			H1 = buffer/MOVING_AVARAGE;
			e1=H1-TARGET_1;						//	e_1(t)
			//	Switch reading to other sensor, H2
			set_channel(ADC1);						//	Set ADC channel to PA1
			sensor=H2_SENS;	
			moving_avarage_counter=0;
			buffer=0;
			

			
			getDegree();
			}
		}
		else{
			buffer+=adc_val;
			if(moving_avarage_counter<MOVING_AVARAGE){
				return;
			}else{
				H2_prev = H2;
				H2 = buffer/MOVING_AVARAGE;
				//e2=abs(TARGET_2-H2);						// e_2(t)
				e2= H2-TARGET_2;
				//	Switch reading to other sensor, H1
				set_channel(ADC0);						//	Set ADC channel to PA0
				sensor=H1_SENS;
				moving_avarage_counter=0;
				buffer=0;
				

				getDegree();
			}
		}
		/*
		if(e1>e2){// test, remove later
			usart_transmit((uint8_t) e1);
			return;						
		}
		usart_transmit((uint8_t) e2);
		return;
		*/	//OK CORRECT VALUE
		
	
	//	adc_switch_counter=0;

		
		//Testar för att fixa så att den kan skicka rätt
	/*
		transmit_counter++;
		if(transmit_counter==500){ // 250 tidigare
			transmit_counter=0;
			getDegree();
		}*/


		
	//}	
}


/*
*	\brief Determines degree depending on which sensor has the most correct reading
*	at the current place in time.
*/
void getDegree(){	// Dåligt namn. Byt det ditt äckel
	
	
	//TEST, byter H1>H2 mot e1>e2
	//if(H1>H2){										//	Leaning towards H2, need to drive forward, e1 should be used
	/*
	uint8_t tmp;
	speed = P*e1+Kd*(H1-H1_prev);
	if(e1<=0){
		motor_direction=BACKWARDS;
	}else{
		motor_direction=FORWARD;
	}
	if(speed>0){
		if(speed>=127){speed=126;}
			tmp=speed;
	}else{
		if(speed<=-127){speed=126;}
			tmp=speed;
	}
	tmp|=motor_direction;
		usart_transmit(tmp);
	reset_counters();
	
	return;
	*/
	/*
	if(e1 < NOTHING_VAL && e2 < NOTHING_VAL){
		e1=0;
		e2=0;
		speed = 0;
		send_motorspeed();
		return;
	}*/
	
	/*
	if(e1==e2 || (e1 < NOTHING_VAL && e2 < NOTHING_VAL)){
		e1=0;
		e2=0;
		speed = 0;
		send_motorspeed();
		return;
	}
	*/
	
	/// TA bort ovan
	
	if(e1>e2){
		//speed = -(P*e1);
		speed = abs(P*e1+Ki*(TARGET_1-(H1+H1_prev)/2)+Kd*(H1-H1_prev));//+Ki*(H1-H1_prev)*DELTA_TIME+Kd*(H1-H1_prev)*DELTA_TIME);			//	Negative multiplication due to the nature of TARGET-adc_val
		/*double tmp = speed;
		tmp/=max_speed_e1;
		tmp*=125;
		speed = tmp;*/
		motor_direction = FORWARD;
	}else{											//	Leaning towards H1, need to drive backwards, e2 should be used
		speed = abs(P*e2+Ki*(TARGET_2-(H2+H2_prev)/2)+Kd*(H2-H2_prev));//+Ki*(H2-H2_prev)*DELTA_TIME+Kd*(H2-H2_prev)*DELTA_TIME);			//	Negative multiplication due to the nature of TARGET-adc_val
		/*speed/=max_speed_e2;
		speed*=127;*/
		/*double tmp = speed;
		tmp/=max_speed_e1;
		tmp*=125;
		speed = tmp;*/
		
		motor_direction = BACKWARDS;
	}
	
	send_motorspeed();

	
}

void send_motorspeed()
{
	volatile uint8_t data;
	if(speed>=127){speed=126;}
	data = speed;
	
	data|=motor_direction;
	// Trying to send only when needed, but this causes an error when the motors dont get updated in 
	// Inverted_pendulum_driver.py
	/*if(data!=prev_transmit){
	usart_transmit(data);
	}
	prev_transmit=data;*/
	
	usart_transmit(data);
	reset_counters();
}

void reset_counters(){
		adc_switch_counter = 0;							//	Already done at least two readings, keep going.
		transmit_counter=0;		// Fix: flytta dessa två till en egen "reset_counters" ska ske efter transmits
}

void startSeq(){
	while(!(PINA&(1<<PINA7)));
	//Ta en miljard värden
	int buffer = 0;
	uint8_t nbr_avg = 10;
	uint8_t delay_time = 5;
	
	H1_MIN = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	H1_MIN = buffer/nbr_avg;
	buffer=0;
	set_channel(ADC1);
	_delay_ms(20);
	
	H2_MAX = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	H2_MAX = buffer/nbr_avg;
	buffer=0;
	_delay_ms(20);
	
	
	
	_delay_ms(500);
	while(!(PINA&(1<<PINA7)));
	
	
	H2_MIN = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	H2_MIN = buffer/nbr_avg;
	buffer=0;
	set_channel(ADC0);
	_delay_ms(20);
	
	
	H1_MAX = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	H1_MAX = buffer/nbr_avg;
	buffer=0;
	_delay_ms(20);
		
	
	_delay_ms(500);
	while(!(PINA&(1<<PINA7)));
	
	
	TARGET_1 = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	TARGET_1 = buffer/nbr_avg;
	buffer=0;
	set_channel(ADC1);
	_delay_ms(20);
	
	TARGET_2 = ADC;
	_delay_ms(20);
	for(int i = 0; i<nbr_avg; i++){
		buffer+=ADC;
		_delay_ms(delay_time);
	}
	TARGET_2 = buffer/nbr_avg;
	buffer=0;
	set_channel(ADC0);
	_delay_ms(20);
		
	
	max_speed_e1 = abs(1*(TARGET_1-H1_MAX));		// FIX! FIND CORRECT VALUE FOR (H1-H1_prev)*DELTA_TIME+Kd*(H1-H1_prev)*DELTA_TIME). Measure what H1-H1_PREV could be at a maximum
	max_speed_e2 = abs(1*(TARGET_2-H2_MAX));		// FIX! FIND CORRECT VALUE FOR (H2-H2_prev)*DELTA_TIME+Kd*(H2-H2_prev)*DELTA_TIME). Measure what H1-H1_PREV could be at a maximum
	sei();
}

