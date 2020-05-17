/*	Author: lab
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned short max = 0x333;
unsigned short x = 0x00;
unsigned short threshold = 0x00;

int main(void)
{
	threshold = (max / 8);
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	ADC_init();

	while (1)
	{
		x = ADC;
		if (x <= threshold)
		{
			PORTB = 0x01;
		}
		else if (x <= (2 * threshold))
		{
			PORTB = 0x03;
		}
		else if (x <= (3 * threshold))
		{
			PORTB = 0x07;
		}
		else if (x <= (4 * threshold))
		{
			PORTB = 0x0F;
		}
		else if (x <= (5 * threshold))
		{
			PORTB = 0x1F;
		}
		else if (x <= (6 * threshold))
		{
			PORTB = 0x3F;
		}
		else if (x <= (7 * threshold))
		{
			PORTB = 0x7F;
		}
		else
		{
			PORTB = 0xFF;
		}
	}
}