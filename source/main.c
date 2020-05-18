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
	TCCR0A = (1 << COM0A0 | 1 << WGM00);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}






double tone[9] = { 329.63,293.66,261.63,293.66,329.63,329.63,329.63,261.63,293.66,293.66,329.63,293.66,261.63 };
unsigned char counter = 13;
unsigned char onFlag = 0;
enum music { Start, INIT, PLAY, WAIT } state;

void button_Tick() {
	switch (state) { // Transitions
	case off:
		if (!PINA * 0x01) {
			state = play;
			j = 0;
		}
		else
			state = off;
		break;
	case play:
		if (j < 13) {
			state = play;
		}
		else {
			state = off;
		}

		break;
	}
	switch (state) { // State actions
	case off:
		set_PWM(0);
		break;
	case play:
		set_PWM(notes[i]);
		set_PWM(0);
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
	PWM_on();
	TimerSet(400);
	TimerOn();
	while (1)
	{

		Tick();
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
