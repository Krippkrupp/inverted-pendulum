/*
 * adc.h
 *
 * Created: 2021-05-09 18:40:39
 *  Author: westr
 */ 

#ifndef _ADC_H_
#define _ADC_H_

#include <avr/io.h>


#define ADC0   0	///< Analog-to-Digital input on PORTA0, see datasheet for ATMEGA1284
#define ADC1   1	///< Analog-to-Digital input on PORTA1, see datasheet for ATMEGA1284
#define ADC2   2	///< Analog-to-Digital input on PORTA2, see datasheet for ATMEGA1284
#define ADC3   3	///< Analog-to-Digital input on PORTA3, see datasheet for ATMEGA1284
#define ADC4   4	///< Analog-to-Digital input on PORTA4, see datasheet for ATMEGA1284
#define ADC5   5	///< Analog-to-Digital input on PORTA5, see datasheet for ATMEGA1284
#define ADC6   6	///< Analog-to-Digital input on PORTA6, see datasheet for ATMEGA1284
#define ADC7   7	///< Analog-to-Digital input on PORTA7, see datasheet for ATMEGA1284

#define PRESCALER1		0b000
#define PRESCALER2		0b001
#define PRESCALER4		0b010
#define PRESCALER8		0b011
#define PRESCALER16		0b100
#define PRESCALER32		0b101
#define PRESCALER64		0b110
#define PRESCALER128	0b111	 


/**
*	\brief Setups the necessary bits in the ADMUX  and ADCSRA
*	registers for single ended input analog-to-digital
*	conversion with AREF as reference voltage. Enables
*	@param single_ended_input is used to set which pin 
*	should be used as single ended input for the
*	Analog-to-Digital converter. Use the predefined
*	macros ADC0-ADC7 corresponding to the desired pin.
**/
void setup_adc(uint8_t single_ended_input);

/**
*	\brief Set which pin should be used to convert an analog signal
*	to a digital signal. Note that only one channel can be used at onece.
*	@param is used to set which channel. Use the predefined macros in adc.h
**/
void set_channel(uint8_t CHANNEL);

/**
*	\brief Sets the prescaler for the AD converter
*	@param is used to set the AD converters prescaler. User the predefined macros
*	PRESCALER1-128, defined in adc.h. Only one input can be used at once.
*	The first conversion after a switch may give a wrong reading. Therefore it's
*	better to disregard the first reading after a switch.
**/
void adc_prescaler(uint8_t PRESCALER);

/**
*	\brief Enables Analog-to-Digital conversion complete interrupts.
*	@param PRESCALER is used to set the prescaler. Use the predefined macros
**/

void adc_interrupt_enabled();
/**
*	\brief Disables Analog-to-Digital conversion complete interrupts.
**/
void adc_interrupt_disbled();
/**
	\brief Starts ADC conversion
**/
void adc_start();
/**
*	\brief Starts ADC conversion in freerunning mode
**/
void adc_freerun();

#endif