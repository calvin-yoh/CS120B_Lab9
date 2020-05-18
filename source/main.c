/*	Author: cyoh001
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

enum States { Start, ON, OFF, Wait } state;
unsigned char tempA = 0x00;

void set_PWM(double frequency)
{
	static double current_frequency;
	if (frequency != current_frequency)
	{
		if (!frequency) TCCR3B &= 0x08;
		else TCCR3B |= 0x03;
		if (frequency < 0.954) OCR3A = 0xFFFF;
		else if (frequency > 31250) OCR3A = 0x0000;
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void Tick()
{
	tempA = ~PINA & 0x07;
	switch (state) // transitions
	{
	case Start:
	{
		state = Wait;
		break;
	}
	case Wait:
	{
		if (tempA != 0x00)
		{
			state = ON;
			break;
		}
		else
		{
			state = Wait;
			break;
		}
	}
	case ON:
	{
		if (tempA != 0x00)
		{
			state = ON; break;
		}
		else
		{
			state = OFF; break;
		}
	}
	case OFF:
	{
		state = Wait;
		break;
	}
	default:
		break;
	}
	switch (state)
	{
	case Wait:
	{
		set_PWM(0);
		break;
	}
	case ON:
	{
		if (tempA == 0x01)
		{
			set_PWM(261.63);
			break;
		}
		else if (tempA == 0x02)
		{
			set_PWM(293.66);
			break;
		}
		else if (tempA == 0x04)
		{
			set_PWM(329.63);
			break;
		}
		break;
	}
	case OFF:
	{
		set_PWM(0);
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

	while (1)
	{
		Tick();
	}
}
