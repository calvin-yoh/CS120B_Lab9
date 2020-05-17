/*	Author:
 *  	Partner(s) Name:
 *   	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */




#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
#include "io.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks
void TimerOn()
{

	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff()
{
	// bit3bit1bit0=000: timer off
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}







void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		if (frequency < 0.954) OCR3A = 0xFFFF;
		else if (frequency > 31250) OCR3A = 0x0000;
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

double tone[9] = { 2610.63, 2930.66, 3290.63, 3490.23, 3920.00, 4400.00, 4930.88, 5230.25,0 };
unsigned char counter = 9;
unsigned char onFlag = 0;
enum music { Start, INIT, PLAY, WAIT } state;

void Tick()
{
	switch (state)
	{
	case Start:
	{
		state = INIT;
		break;
	}
	case INIT:
	{
		if ((~PINA & 0x01) == 0x01)
		{
			state = PLAY;
			break;
		}
		else
		{
			state = INIT;
			break;
		}
	}
	case PLAY:
	{
		break;
	}
	case WAIT:
	{
		state = PLAY;
	}
	default:
		break;
	}
	switch (state)
	{
	case Start:
		break;
	case INIT:
	{
		set_PWM(0);
		break;
	}
	case PLAY:
	{
		if (counter > 0)
		{
			set_PWM(tone[counter]);
			counter--;
			state = WAIT;
			break;
		}

	}
	case WAIT:
	{
		break;
	}
	default:
		break;

	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
	PWM_on();
	TimerSet(50);
	TimerOn();
	while (1)
	{

		Tick();
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
