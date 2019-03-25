#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

int ovrflow = 0; // global ovrflow counter

int main(void)
{
	DDRB = (1<<DDB2); // Make PB2 Output
	DDRC = (0<<DDC2); // Make PC2 Input
	PORTB = (1<<DDB2); // Turn off LED
	PORTC = (1<<DDC2); // Turn on pull-up transistor
	TIMSK0 |= (1<<TOIE0); // Set up interrupt
	TCCR0A = 0; // Normal Mode
	sei();
	TCCR0B = 5; // Set prescaler to 1024
	while (1)
	{
		if (!(PINC & (1 << PINC2))) { // if button pressed
			PORTB ^= (1 << DDB2); // Turn on LED
			TCNT0 = 0;
			ovrflow = 0;
			while (!(PORTB & (1<<PORTB2))) {} // while on	
		}
		ovrflow = 0;
		
	}
}

ISR (TIMER0_OVF_vect) {
	ovrflow++; //increment ovrflow
	if (ovrflow == 76) {
		TCNT0 = 0; 
		while (TCNT0 < 75) {}
		PORTB ^= (1<<DDB2); // Turn off LED
	}
	TCNT0 = 0; //reset counter
}