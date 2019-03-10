#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1<<2); // PB2 set to output
	PORTB |= (1<<2); // PB2 set to HIGH which means LED OFF
	DDRD &= (0<<2);	// PD2 set to input
	PORTD |= (1<<2); // PD2 Turn Up Resistor
	
	EICRA = 0x00; //Low mode
	EIMSK = 1<<INT0; // Masking
	sei();	// Turn on global interrupt
	
    while (1) 
    {
		PORTB |= (1<<2); // Turn off LED
    }
}

ISR (INT0_vect)
{
	PORTB ^= (1<<2); // Toggle PB2
	_delay_ms(1250); // Delay for 1.25s
}
